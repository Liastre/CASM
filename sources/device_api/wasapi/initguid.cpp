#include <initguid.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <mmreg.h>
#include "functiondiscoverykeys_devpkey.h"

#if CASM_BUILD_ENV_MSBUILD
EXTERN_C const IID DECLSPEC_SELECTANY IID_IAudioClient = __uuidof(IAudioClient);
EXTERN_C const IID DECLSPEC_SELECTANY IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
EXTERN_C const GUID DECLSPEC_SELECTANY IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
EXTERN_C const GUID DECLSPEC_SELECTANY CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
#endif
