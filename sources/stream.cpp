// =============== DESCRIPTION ===============
// Created on 27-Aug-17.
// ===========================================

#include <CASM/file.hpp>
#include "stream.hpp"

namespace CASM {

Stream::Stream(EndPointInterface& endPointIn, EndPointInterface& endPointOut, const std::chrono::duration<double> duration) {
    /*Stream::endPointIn = &endPointIn;
    Stream::endPointOut = &endPointOut;
    requestedDuration = duration;*/
}

Stream::Stream(Device& endPointIn, File& endPointOut, const std::chrono::duration<double> duration) {
    Stream::endPointIn = &endPointIn;
    //Stream::endPointIn->open(std::chrono::seconds(4));
    Stream::endPointOut = &endPointOut;
    requestedDuration = duration;
}

void Stream::start(std::chrono::duration<double> delay) {
    // initialisation
    active = true;
    buffer = endPointIn->open(std::chrono::seconds(1));
    endPointOut->open(buffer);
    uptime = std::chrono::steady_clock::now().time_since_epoch();
    auto bufferDuration = buffer.getDuration();

    clockThreadId = std::thread(doClock, this);
    streamThreadId = std::thread(doThread, this);
}

void Stream::stop(std::chrono::duration<double> delay) {
    active = false;
    if(clockThreadId.joinable()) {
        clockThreadId.join();
    }
    if(streamThreadId.joinable()) {
        streamThreadId.join();
    }
}

void Stream::doClock() {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    while (active) {
        uptime = (std::chrono::steady_clock::now()-startTime);
    }
}

void Stream::doThread() {
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

std::chrono::steady_clock::duration Stream::getUptime() const {
    return uptime;
}

}
