/// @file stream.hpp
/// @brief class Stream for transferring data in between audio end points

#pragma once

#include "device.hpp"
#include "file.hpp"
#include <type_traits>
#include <thread>
#include <chrono>
#include <atomic>
#include <future>

namespace CASM {

class Stream {
public:
    template <class TEndPointIn, class TEndPointOut>
    Stream(TEndPointIn const& endPointIn, TEndPointOut const& endPointOut, Duration const& bufferDuration = Duration::zero()) {
        static_assert(std::is_base_of<EndPointInterface, TEndPointIn>::value,
          "TEndPointIn is not derived from EndPointInterface");
        static_assert(std::is_base_of<EndPointInterface, TEndPointOut>::value,
          "TEndPointOut is not derived from EndPointInterface");
        static_assert(!(std::is_same<TEndPointIn, File>::value && std::is_same<TEndPointOut, File>::value),
          "You cannot create stream between File and File");

        _endPointIn = std::make_unique<TEndPointIn>(endPointIn);
        _endPointOut = std::make_unique<TEndPointOut>(endPointOut);
        _requestedBufferDuration = bufferDuration;
        _onCopyCallback = [](Buffer&) {};
    };
    Stream(Stream const&) = delete;
    Stream(Stream&& stream) noexcept;
    Stream& operator=(Stream const&) = delete;
    Stream& operator=(Stream&& stream) noexcept;
    ~Stream();

    /// @return true if succeed, false if some errors appeared
    bool start(Duration const& delay = Duration::zero());
    bool stop(Duration const& delay = Duration::zero());
    void join();

    bool isActive() const;
    Duration getUptime() const;

    /// @brief set bufferDuration (it is minimal by default)
    void setBufferDuration(Duration bufferDuration);
    /// @brief callback to change buffer data with requested way
    void setCopyCallback(void (*onCopyCallbackPtr)(Buffer&));

private:
    /// @brief clock thread
    void _doDataReadThread();
    /// @brief stream thread
    void _doTransferThread(Duration delay);
    /// @brief delayed stop
    void _doStopCallbackThread(Duration delay);

private:
    std::unique_ptr<EndPointInterface> _endPointIn;
    std::unique_ptr<EndPointInterface> _endPointOut;
    std::unique_ptr<Buffer> _buffer;
    Duration _requestedBufferDuration;
    std::chrono::steady_clock::time_point _startTime;
    std::atomic<bool> _isActive = { false };
    std::mutex _isCopying;
    std::unique_ptr<std::thread> _transferThread = nullptr;
    std::unique_ptr<std::thread> _dataReadThread = nullptr;
    void (*_onCopyCallback)(Buffer&) = nullptr;
};

} // namespace CASM
