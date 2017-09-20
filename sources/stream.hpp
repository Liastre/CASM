// =============== DESCRIPTION ===============
// Created on 27-Aug-17.
// ===========================================

#ifndef CASM_STREAM_HPP
#define CASM_STREAM_HPP

#include <CASM/device.hpp>
#include <CASM/file.hpp>
#include <thread>
#include <chrono>
#include <atomic>

namespace CASM {

class Stream {
public:
    Stream(EndPointInterface& endPointIn, EndPointInterface& endPointOut, std::chrono::duration<double> duration);
    Stream(Device& endPointIn, File& endPointOut, std::chrono::duration<double> duration);

    ~Stream() = default;

    void start(std::chrono::duration<double> delay = std::chrono::duration<double>::zero());

    void stop(std::chrono::duration<double> delay = std::chrono::duration<double>::zero());

    // getters
    std::chrono::steady_clock::duration getUptime() const;

    // setters
    /// @brief set bufferDuration (it is minimal by default)
    void setBufferDuration(std::chrono::duration<float> bufferDuration);
    /// @brief callback to change buffer data with requested way
    void setCopyCallback(void* onCopyCallbackPtr);

private:
    void doThread();
    void doClock();

    void* onCopyCallback;

    EndPointInterface* endPointIn;
    File* endPointOut;
    Buffer buffer;
    std::atomic<std::chrono::steady_clock::duration> uptime;
    std::atomic_bool active;
    std::chrono::duration<double> bufferDuration;
    std::chrono::duration<double> requestedDuration;
    std::thread streamThreadId;
    std::thread clockThreadId;
};

}

#endif //CASM_STREAM_HPP
