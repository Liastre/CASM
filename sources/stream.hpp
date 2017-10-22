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
#include <future>


namespace CASM {

class Stream {
public:
    Stream() = delete;
    Stream(const Stream &) = delete;


    template < class TEndPointIn, class TEndPointOut >
    Stream(TEndPointIn endPointIn, TEndPointOut endPointOut,
            std::chrono::duration< double > bufferDuration = std::chrono::duration< double >::zero()) {
        static_assert(std::is_base_of< EndPointInterface, TEndPointIn >::value, "TEndPointIn is not derived from EndPointInterface");
        static_assert(std::is_base_of< EndPointInterface, TEndPointOut >::value, "TEndPointOut is not derived from EndPointInterface");
        static_assert(!(
                        std::is_same< File, TEndPointIn >::value && std::is_same< File, TEndPointOut >::value),
                "You cannot create stream between File and File");

        Stream::_endPointIn = new TEndPointIn(endPointIn);
        Stream::_endPointOut = new TEndPointOut(endPointOut);
        Stream::_requestedBufferDuration = bufferDuration;
        _onCopyCallback = [](Buffer &) {};
    };
    ~Stream();
    // methods
    /// @return true if succeed, false if some errors appeared
    bool start(std::chrono::duration< double > delay = std::chrono::duration< double >::zero());
    void stop(std::chrono::duration< double > delay = std::chrono::duration< double >::zero());
    void join();
    // getters
    std::chrono::duration<double> getUptime() const;
    // setters
    /// @brief set bufferDuration (it is minimal by default)
    void setBufferDuration(std::chrono::duration< double > bufferDuration);
    /// @brief callback to change buffer data with requested way
    void setCopyCallback(void (*onCopyCallbackPtr)(Buffer &));

private:
    /// @brief clock thread
    void startClock();
    bool _doCopyDataThread();
    /// @brief stream thread
    void _doTransferThread(std::chrono::duration< double > delay);
    /// @brief delayed stop
    void _doStopCallbackThread(std::chrono::duration< double > delay);

    // fields
    EndPointInterface *_endPointIn;
    EndPointInterface *_endPointOut;
    Buffer _buffer;
    std::chrono::steady_clock::time_point _startTime;
    std::atomic_bool _isActive;
    std::chrono::duration< double > _requestedBufferDuration;

    std::future< bool > _doCopyDataThreadFuture;
    std::thread _doTransferThreadId;
    std::thread _doStopCallbackThreadId;
    void (*_onCopyCallback)(Buffer &);
};

}

#endif //CASM_STREAM_HPP
