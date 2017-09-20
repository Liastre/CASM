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


void Stream::start(std::chrono::duration< double > delay) {
    // initialisation
    active = true;
    buffer = endPointIn->open(std::chrono::seconds(1));
    endPointOut->open(buffer);
    uptime = std::chrono::steady_clock::now().time_since_epoch();
    auto bufferDuration = buffer.getDuration();

    clockThreadId = std::thread(startClock, this);
    streamStartThreadId = std::thread(startThread, this);
    if (requestedDuration!=std::chrono::duration< double >::zero()) {

    }
}


void Stream::stop(const std::chrono::duration< double > delay) {
    streamStopThreadId = std::thread(stopThread, this, delay);
}


void Stream::startThread() {
    // write data
    while (endPointIn->isAvailable() && endPointOut->isAvailable() && active) {
        std::this_thread::sleep_for(bufferDuration);
        endPointIn->read(buffer);
        // TODO: call callback method on data
        endPointOut->write(buffer);
    }

    endPointIn->close();
    endPointOut->close();
}


void Stream::startClock() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    while (active) {
        uptime = (std::chrono::steady_clock::now()-startTime);
    }
}


void Stream::stopThread(const std::chrono::duration< double > delay) {
    std::this_thread::sleep_for(delay);
    if (active) {
        active = false;
    }
    if (streamStartThreadId.joinable()) {
        streamStartThreadId.join();
    }
    if (clockThreadId.joinable()) {
        clockThreadId.join();
    }
}


std::chrono::steady_clock::duration Stream::getUptime() const {
    return uptime;
}


void Stream::join() {
    if (streamStopThreadId.joinable()) {
        streamStopThreadId.join();
    }
}

}
