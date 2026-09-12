#include "screenshot_bypass.h"
#include <d3d11.h>
namespace ScreenshotBypass {
    using tCopyResource = void(WINAPI*)(ID3D11DeviceContext*, ID3D11Resource*, ID3D11Resource*);
    tCopyResource oCopyResource = nullptr;
    void WINAPI hCopyResource(ID3D11DeviceContext* pContext, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource) {
        oCopyResource(pContext, pDstResource, pSrcResource);
    }
    void Init() {
        // Hook D3D11 CopyResource to ensure overlay is drawn AFTER the anti-cheat takes its screenshot
    }
}
