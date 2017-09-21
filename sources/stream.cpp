// =============== DESCRIPTION ===============
// Created on 27-Aug-17.
// ===========================================

#include <CASM/file.hpp>
#include "stream.hpp"


namespace CASM {

Stream::~Stream() {
    delete endPointIn;
    delete endPointOut;
}


bool Stream::start(std::chrono::duration< double > delay) {
    // initialisation
    active = true;
    buffer = endPointIn->open(std::chrono::seconds(1));
    endPointOut->open(buffer);
    // this should never be happen, since StreamManager cares about to pass correct data
    if (Stream::endPointIn->getStreamWaveProperties() != Stream::endPointOut->getStreamWaveProperties()) {
        // TODO: add some logger
        throw std::runtime_error("Unable to connect two points with different WaveProperties");
        //return false;
    }

    uptime = std::chrono::steady_clock::duration::zero();
    clockThreadId = std::thread(startClock, this);
    streamStartThreadId = std::thread(startThread, this);

    return true;
}


void Stream::stop(const std::chrono::duration< double > delay) {
    // TODO: check if thread already runned
    streamStopThreadId = std::thread(stopThread, this, delay);
}


void Stream::startClock() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    while (active) {
        uptime = (std::chrono::steady_clock::now()-startTime);
    }
}


void Stream::startThread() {
    auto bufferDuration = buffer.getDuration();
    // write data
    while (endPointIn->isAvailable() && endPointOut->isAvailable() && active) {
        std::this_thread::sleep_for(bufferDuration);
        if (endPointIn->read(buffer)) {
            // call callback method on data
            onCopyCallback(buffer);
            endPointOut->write(buffer);
        } else {
            // no more data to read
            break;
        }
    }
    // close endpoints
    endPointIn->close();
    endPointOut->close();
    active = false;
}


void Stream::stopThread(const std::chrono::duration< double > delay) {
    std::this_thread::sleep_for(delay);
    if (active) {
        active = false;
    }
}


std::chrono::steady_clock::duration Stream::getUptime() const {
    return uptime;
}


void Stream::join() {
    // TODO: check if stop thread exist
    if (streamStopThreadId.joinable()) {
        streamStopThreadId.join();
    }
    if (streamStartThreadId.joinable()) {
        streamStartThreadId.join();
    }
    if (clockThreadId.joinable()) {
        clockThreadId.join();
    }
}


void Stream::setCopyCallback(void (*onCopyCallbackPtr)(Buffer&)) {
    onCopyCallback = onCopyCallbackPtr;
}

}
