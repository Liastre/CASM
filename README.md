# Description
CASM is Crossplatform Audio Stream Manager library. The main purpose is translating audio from one existing end point (device or file) to another. This library allows you to play, record and redirect audio data in any possible way. 

# Capabilities
- Simple API to control all system audio streams
- Different ways to control data stream from file
- Simple extensible with your own codec using common interface

# Features
- [x] Single-thread read/write to endpoint
- [ ] Multi-thread read/write to endpoint
- [ ] Audio stream transformations (resampling)
- [ ] Creating virtual audio device

# In-built codec support
- [x] PCM (.wav)
- [ ] Vorbis (.ogg)

# API support
- [x] Windows WASAPI
- [ ] Windows DirectSound
- [ ] Windows ASIO
- [ ] Linux ASIO