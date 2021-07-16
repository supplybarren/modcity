// Dear ImGui: standalone example application for DirectX 11
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_freetype.h"
#include "maddox.h"
#include <d3d11.h>
#include <D3DX11tex.h>
#include "logo.h"
#include <tchar.h>
#pragma comment(lib, "d3dx11.lib")

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

ID3D11ShaderResourceView* logo = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Main code





float m_sidebar_animation = 0;
float m_sidebar_timer = 0;





int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("modcity project"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("modcity project"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    if (logo == nullptr) {
        D3DX11_IMAGE_LOAD_INFO info;
        ID3DX11ThreadPump* pump{ nullptr };
        D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rawData, sizeof(rawData), &info, pump, &logo, 0);
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ImFontConfig font_config_bold;
    ImFontConfig font_config_normal;
    font_config_normal.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_NoHinting;
    font_config_bold.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_Bold | ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
    ImFont* icon = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\icons.ttf", 18.f, &font_config_normal, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font_bold = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\MuseoSans.ttf", 17.f, &font_config_bold, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\MuseoSans.ttf", 16.f, &font_config_normal, io.Fonts->GetGlyphRangesCyrillic());

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(30 / 255.f, 30 / 255.f, 30 / 255.f, 1.00f);

    // Main loop
    bool done = false;
    maddox::setup();
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::PushFont(font);
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            static bool open = true;
            static int selected_tab = 1;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.f);
            ImGui::SetNextWindowSize(ImVec2(550, 400));
            ImGui::Begin("modcity", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
            ImVec2 pos = ImGui::GetWindowPos();
            ImVec2 size = ImGui::GetWindowSize();
            auto drawlist = ImGui::GetWindowDrawList();
            drawlist->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y / 2), IM_COL32(13, 21, 42, 255), 10.f, ImDrawFlags_RoundCornersTop);
            drawlist->AddRectFilledMultiColor(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(11, 19, 32, 100), IM_COL32(11, 21, 56, 160), IM_COL32(11, 19, 32, 100), IM_COL32(11, 21, 56, 160));

            static bool checkbox = false;
            static int slider = 0;
            static int selected_combo_item = 0;
            static int keybind_key = 0;
            const char* items[4] = {"one", "two", "three", "four"};

            ImGui::SetCursorPos(ImVec2(75, 0));
            ImGui::PushClipRect(ImVec2(pos.x + 75 + m_sidebar_animation * 90, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), false);
            if (selected_tab == NIGGA_AIMBOT) {
                ImGui::BeginChild("aimbot_child");
                ImGui::Checkbox("checkbox", &checkbox);
                maddox::gui::keybind("i am keybind", &keybind_key);
                ImGui::SliderInt("i am slider", &slider, 0, 100);
                ImGui::Combo("i am combo", &selected_combo_item, items, IM_ARRAYSIZE(items));
                ImGui::EndChild();
            }
            ImGui::PopClipRect();


            // below is the sidebar THIS MUST BE RENDERED LAST DONT BE STUPID


            drawlist->AddRectFilled(pos, ImVec2(pos.x + 75 + m_sidebar_animation * 90, pos.y + size.y), IM_COL32(12, 21, 55, 40), 3.f, ImDrawFlags_RoundCornersLeft);
            drawlist->AddRectFilledMultiColor(ImVec2(pos.x + 75 + m_sidebar_animation * 90, pos.y), ImVec2(pos.x + 90 + m_sidebar_animation * 90, pos.y + size.y), IM_COL32(0, 0, 0, 40), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 40));
           
            if (ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x + 75 + m_sidebar_animation * 70, pos.y + size.y))) {
                if (m_sidebar_timer < 1.f) {
                    m_sidebar_timer += ImGui::GetIO().DeltaTime * 5;
                    m_sidebar_timer = std::clamp(m_sidebar_timer, 0.f, 1.f);
                    m_sidebar_animation = easing::OutCubic(m_sidebar_timer);
                }
            }
            else {
                if (m_sidebar_timer > 0.f) {
                    m_sidebar_timer -= ImGui::GetIO().DeltaTime * 5;
                    m_sidebar_timer = std::clamp(m_sidebar_timer, 0.f, 1.f);
                    m_sidebar_animation = easing::InCubic(m_sidebar_timer);
                }
            }

            drawlist->AddImage(logo, ImVec2(pos.x + (75 + m_sidebar_animation * 90) / 2 - 12.5, pos.y + 25), ImVec2(pos.x + (75 + m_sidebar_animation * 90) / 2 + 12.5, pos.y + 51));
            ImGui::SetCursorPos(ImVec2(0, 80));
            if (maddox::gui::tab("a", "Aimbot", m_sidebar_animation)) {
                selected_tab = NIGGA_AIMBOT;
            }
            maddox::gui::tab("b", "Visuals", m_sidebar_animation);
            maddox::gui::tab("d", "Misc", m_sidebar_animation);

            ImGui::End();
            ImGui::PopFont();
            ImGui::PopStyleVar(2);
        }
        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
