// dear imgui: Renderer Backend for DirectX11
// This needs to be used along with a Platform Backend (e.g. Win32)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'ID3D11ShaderResourceView*' as ImTextureID. Read the FAQ about ImTextureID!
//  [X] Renderer: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this. 
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#pragma once
#include "imgui.h"      // IMGUI_IMPL_API
#include "../examples/example_win32_directx11/maddox_utils.h"
#include <d3d11.h>

namespace maddox {
    void blur(int x, int y, int w, int h, const ImU32 colTexture, int pass);
}

struct ID3D11Device;
struct ID3D11DeviceContext;

IMGUI_IMPL_API bool     ImGui_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);
IMGUI_IMPL_API void     ImGui_ImplDX11_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplDX11_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API void     ImGui_ImplDX11_InvalidateDeviceObjects();
IMGUI_IMPL_API bool     ImGui_ImplDX11_CreateDeviceObjects();

class CBlurProcessor : public ABSingleton<CBlurProcessor>
{
public:
    CBlurProcessor() = default;

    // Get
    static void Start(const ImDrawList* pDrawList, const ImDrawCmd* pCmd)
    {
        Get().BeginBlur(static_cast<ID3D11Device*>(pCmd->UserCallbackData));
    }

    static void End(const ImDrawList* pDrawList, const ImDrawCmd* pCmd)
    {
        Get().EndBlur(static_cast<ID3D11Device*>(pCmd->UserCallbackData));
    }

    static void First(const ImDrawList* pDrawList, const ImDrawCmd* pCmd)
    {
        Get().FirstBlurPass(static_cast<ID3D11Device*>(pCmd->UserCallbackData));
    }

    static void Second(const ImDrawList* pDrawList, const ImDrawCmd* pCmd)
    {
        Get().SecondBlurPass(static_cast<ID3D11Device*>(pCmd->UserCallbackData));
    }

    // Values
    /* first pass blur texture */
    ID3D11Texture2D* pBlurFirst = nullptr;
    /* second pass blur texture */
    ID3D11Texture2D* pBlurSecond = nullptr;
    /* full backbuffer width */
    unsigned int uBackBufferWidth = 0U;
    /* full backbuffer height */
    unsigned int uBackBufferHeight = 0U;

private:
    // Main
    void BeginBlur(ID3D11Device* pDevice);
    void EndBlur(ID3D11Device* pDevice) const;
    void FirstBlurPass(ID3D11Device* pDevice) const;
    void SecondBlurPass(ID3D11Device* pDevice) const;

    // Values
    /* saved render target to restore it after our modifications */
    ID3D11Texture2D* pSurfaceBackup = nullptr;
    /* horizontal blur shader */
    ID3D11PixelShader* pBlurShaderX = nullptr;
    /* vertical blur shader */
    ID3D11PixelShader* pBlurShaderY = nullptr;
    /* backbuffer texture downsample */
    static constexpr int nBlurDownsample = 4;
};
