/// @file stream.cpp
/// @brief definition of Stream class

#include <CASM/file.hpp>
#include "stream.hpp"


namespace CASM {

Stream::~Stream() {
    delete _endPointIn;
    delete _endPointOut;
}


bool Stream::start(std::chrono::duration< double > delay) {
    // initialisation
    _isActive = true;
    _buffer = _endPointIn->openCaptureStream(_requestedBufferDuration);
    _endPointOut->openRenderStream(_buffer);
    // this should never be happen, since StreamManager cares about to pass correct data
    if (Stream::_endPointIn->getStreamWaveProperties() != Stream::_endPointOut->getStreamWaveProperties()) {
        // TODO: add some logger
        throw std::runtime_error("Unable to connect two points with different WaveProperties");
        //return false;
    }

    _startTime = std::chrono::steady_clock::now();
    _doTransferThreadId = std::thread(_doTransferThread, this, delay);

    return true;
}


void Stream::stop(const std::chrono::duration< double > delay) {
    // TODO: check if thread already launched
    _doStopCallbackThreadId = std::thread(_doStopCallbackThread, this, delay);
}


void Stream::_doTransferThread(std::chrono::duration< double > delay) {
    // TODO: wrap with try catch
    std::this_thread::sleep_for(delay);
    auto bufferDuration = _buffer.getDuration();
    // write data
    while (_endPointIn->isAvailable() && _endPointOut->isAvailable() && _isActive) {
        _doCopyDataThreadFuture = std::async(&Stream::_doCopyDataThread, this);
        std::this_thread::sleep_for(bufferDuration);
        if(_doCopyDataThreadFuture.wait_for(std::chrono::duration<double>::zero())!=std::future_status::ready) {
            throw std::runtime_error("Execution of CopyDataThread exceeded await time");
        }

        _endPointOut->write(_buffer);
    }

    // close endpoints
    _endPointIn->closeRenderStream();
    _endPointOut->closeRenderStream();
    _isActive = false;
}


void Stream::_doStopCallbackThread(std::chrono::duration< double > delay) {
    std::this_thread::sleep_for(delay);
    if (_isActive) {
        _isActive = false;
    }
}


std::chrono::duration<double> Stream::getUptime() const {
    return (_startTime-std::chrono::steady_clock::now());
}


void Stream::join() {
    // TODO: check if stop thread exist
    if (_doStopCallbackThreadId.joinable()) {
        _doStopCallbackThreadId.join();
    }
    if (_doTransferThreadId.joinable()) {
        _doTransferThreadId.join();
    }
}


void Stream::setCopyCallback(void (*onCopyCallbackPtr)(Buffer&)) {
    _onCopyCallback = onCopyCallbackPtr;
}


bool Stream::_doCopyDataThread() {
    bool status (true);
    status = _endPointIn->read(_buffer);
    _onCopyCallback(_buffer);

    return status;
}

} // namespace CASM
