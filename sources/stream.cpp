/// @file stream.cpp
/// @brief definition of Stream class

#include <CASM/file.hpp>
#include <CASM/stream.hpp>
#include <iostream>

namespace CASM {

Stream::Stream(Stream&& stream) noexcept {
    this->operator=(std::move(stream));
}

Stream&
Stream::operator=(Stream&& stream) noexcept {
    _endPointIn = std::move(stream._endPointIn);
    _endPointOut = std::move(stream._endPointOut);
    _buffer = std::move(stream._buffer);
    _requestedBufferDuration = std::move(stream._requestedBufferDuration);
    _startTime = std::move(stream._startTime);
    _isActive = stream._isActive.load();
    _transferThread = std::move(stream._transferThread);
    _dataReadThread = std::move(stream._dataReadThread);
    _onCopyCallback = stream._onCopyCallback;
    return *this;
}

Stream::~Stream() {
    join();
    _endPointIn.reset();
    _endPointOut.reset();
}

bool
Stream::start(Duration const& delay) {
    if (isActive())
        return false;

    _buffer = std::make_unique<Buffer>();
    // TODO: refactor, do not init buffer inside
    if (!_endPointIn->openCaptureStream(_requestedBufferDuration, *_buffer)) {
        return false;
    }
    if (!_endPointOut->openRenderStream(*_buffer)) {
        return false;
    }
    // this should never be happen, since StreamManager cares about to pass correct data
    auto inputWaveProperties = Stream::_endPointIn->getStreamWaveProperties();
    auto outputWaveProperties = Stream::_endPointOut->getStreamWaveProperties();
    if (inputWaveProperties != outputWaveProperties) {
        // TODO: add some logger
        // Unable to connect two points with different WaveProperties
        // Implement converter
        return false;
    }

    _startTime = std::chrono::steady_clock::now();
    _transferThread = std::make_unique<std::thread>(&CASM::Stream::_doTransferThread, this, delay);
    _dataReadThread = std::make_unique<std::thread>(&CASM::Stream::_doDataReadThread, this);
    _isActive = true;

    return true;
}

bool
Stream::stop(Duration const& delay) {
    if (!_isActive)
        return false;

    _doStopCallbackThread(delay);
    //_stopCallbackThread = std::make_unique<std::thread>(&CASM::Stream::_doStopCallbackThread, this, delay);
    // TODO: use proper check
    return true;
}

void
Stream::join() {
    if (_transferThread && _transferThread->joinable()) {
        _transferThread->join();
    }
    if (_dataReadThread && _dataReadThread->joinable()) {
        _dataReadThread->join();
    }
}

bool
Stream::isActive() const {
    return _isActive;
}

Duration
Stream::getUptime() const {
    return (std::chrono::steady_clock::now() - _startTime);
}

void
Stream::setCopyCallback(void (*onCopyCallbackPtr)(Buffer&)) {
    _onCopyCallback = onCopyCallbackPtr;
}

void
Stream::_doDataReadThread() {
    try {
        BufferStatus status(CASM::BufferStatus::BufferFilled);
        // waiting till stream or data become over
        while (_isActive && status != CASM::BufferStatus::DataEmpty) {
            std::lock_guard<std::mutex> lock(_isCopying);
            status = _endPointIn->read(*_buffer);
            // TODO: rework data tresspass callback
            _onCopyCallback(*_buffer);
            std::this_thread::yield();
        }
        _endPointIn->closeCaptureStream();
        _isActive = false;
    } catch (std::exception& e) {
        // TODO: log
        std::cout << e.what();
    }
}

void
Stream::_doTransferThread(Duration delay) {
    try {
        auto bufferDuration = _buffer->getDuration();
        // write data
        while (_endPointOut->isAvailable() && _isActive) {
            std::this_thread::sleep_for(delay);
            std::lock_guard<std::mutex> lock(_isCopying);
            _endPointOut->write(*_buffer);
        }

        _endPointOut->closeRenderStream();
        _isActive = false;
    } catch (std::exception& e) {
        // TODO: log
        std::cout << e.what();
    }
}

void
Stream::_doStopCallbackThread(Duration delay) {
    std::this_thread::sleep_for(delay);
    if (_isActive) {
        _isActive = false;
    }
}

} // namespace CASM
