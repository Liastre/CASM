/// @file stream.hpp
/// @brief class Stream for transferring data in between audio end points

#ifndef CASM_STREAM_HPP
#define CASM_STREAM_HPP

#include <CASM/device.hpp>
#include <CASM/file.hpp>
#include <type_traits>
#include <thread>
#include <chrono>
#include <atomic>


namespace CASM {

class Stream {
public:
    Stream() = delete;
    Stream(const Stream &) = delete;


    template < class TEndPointIn, class TEndPointOut >
    Stream(TEndPointIn endPointIn, TEndPointOut endPointOut, std::chrono::duration< double > bufferDuration = std::chrono::duration< double >::zero()) {
        static_assert(std::is_base_of< EndPointInterface, TEndPointIn >::value, "TEndPointIn is not derived from EndPointInterface");
        static_assert(std::is_base_of< EndPointInterface, TEndPointOut >::value, "TEndPointOut is not derived from EndPointInterface");
        static_assert(!(std::is_same< File, TEndPointIn >::value &&
                        std::is_same< File, TEndPointOut >::value), "You cannot create stream between File and File");

        Stream::endPointIn = new TEndPointIn(endPointIn);
        Stream::endPointOut = new TEndPointOut(endPointOut);
        Stream::requestedBufferDuration = bufferDuration;
        onCopyCallback = [](Buffer&){};
    };
    ~Stream();
    // methods
    /// @return true if succeed, false if some errors appeared
    bool start(std::chrono::duration< double > delay = std::chrono::duration< double >::zero());
    void stop(std::chrono::duration< double > delay = std::chrono::duration< double >::zero());
    void join();
    // getters
    std::chrono::steady_clock::duration getUptime() const;
    // setters
    /// @brief set bufferDuration (it is minimal by default)
    void setBufferDuration(std::chrono::duration< double > bufferDuration);
    /// @brief callback to change buffer data with requested way
    void setCopyCallback(void (*onCopyCallbackPtr)(Buffer&));

private:
    /// @brief clock thread
    void startClock();
    /// @brief stream thread
    void startThread(std::chrono::duration< double > delay);
    /// @brief delayed stop
    void stopThread(std::chrono::duration< double > delay);

    // fields
    EndPointInterface *endPointIn;
    EndPointInterface *endPointOut;
    Buffer buffer;
    std::atomic< std::chrono::steady_clock::duration > uptime;
    std::atomic_bool active;
    std::chrono::duration< double > requestedBufferDuration;
    // update to async
    std::thread streamStartThreadId;
    std::thread clockThreadId;
    std::thread streamStopThreadId;
    void (*onCopyCallback)(Buffer&);
};

}

#endif //CASM_STREAM_HPP
