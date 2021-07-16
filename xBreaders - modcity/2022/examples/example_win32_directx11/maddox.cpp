#include "maddox.h"
#include "../imgui.h"
#include "../imgui_internal.h"
#include "imgui_freetype.h"
#include "imgui_impl_dx11.h"
#include <utility>
#include <stdlib.h>
#include <algorithm>
#include <unordered_map>
#include <iostream>

typedef unsigned int     size_t;
ImFont* ficon = nullptr;
ImFont* largeFont = nullptr;
ImFont* largeFont_nb = nullptr;

const char* const KeyNames[] = {
    "???",
    "VK_LBUTTON",
    "M2",
    "Cancl",
    "M3",
    "M4",
    "M5",
    "???",
    "Back",
    "Tab",
    "???",
    "???",
    "Clear",
    "Retrn",
    "???",
    "???",
    "Shift",
    "Ctrl",
    "Menu",
    "Pause",
    "Caps",
    "VK_KANA",
    "???",
    "VK_JUNJA",
    "VK_FINAL",
    "VK_KANJI",
    "???",
    "Esc",
    "VK_CONVERT",
    "VK_NONCONVERT",
    "VK_ACCEPT",
    "VK_MODECHANGE",
    "Space",
    "VK_PRIOR",
    "VK_NEXT",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "Selct",
    "Print",
    "Exec",
    "Snap",
    "Ins",
    "Del",
    "Help",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "VK_LWIN",
    "VK_RWIN",
    "VK_APPS",
    "Unknown",
    "VK_SLEEP",
    "Num0",
    "Num1",
    "Num2",
    "Num3",
    "Num4",
    "Num5",
    "Num6",
    "Num7",
    "Num8",
    "Num9",
    "Mult",
    "Add",
    "Separ",
    "Minus",
    "Dot",
    "Slash",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Numl",
    "Scroll",
    "VK_OEM_NEC_EQUAL",
    "VK_OEM_FJ_MASSHOU",
    "VK_OEM_FJ_TOUROKU",
    "VK_OEM_FJ_LOYA",
    "VK_OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Shift",
    "Shift",
    "Ctrl",
    "Ctrl",
    "Menu",
    "Menu"
};

struct notification {
    const char* title;
    const char* notification_text;
    int timer;
    float alpha;
    float alpha_timer;
};

void maddox::rect(int x, int y, int size_x, int size_y, ImU32 col, float rounding, ImDrawFlags flags, bool is_foreground) {
    auto drawlist = is_foreground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
    drawlist->AddRect(ImVec2(x, y), ImVec2(x + size_x, y + size_y), col, rounding, flags);
}

void maddox::filledrect(int x, int y, int size_x, int size_y, ImU32 col, float rounding, ImDrawFlags flags) {
    auto drawlist = ImGui::GetWindowDrawList();
    drawlist->AddRectFilled(ImVec2(x, y), ImVec2(x + size_x, y + size_y), col, rounding, flags);
}

void maddox::gradientrect(int x, int y, int size_x, int size_y, ImU32 col_top, ImU32 col_bottom, bool is_foreground) {
    auto drawlist = is_foreground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
    drawlist->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + size_x, y + size_y), col_top, col_top, col_bottom, col_bottom);
}

void maddox::gradientrect_horizontal(int x, int y, int size_x, int size_y, ImU32 col_left, ImU32 col_right, bool is_foreground) {
    auto drawlist = is_foreground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
    drawlist->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x + size_x, y + size_y), col_left, col_right, col_right, col_left);
}

void maddox::circle(int x, int y, float radius, ImU32 col) {
    auto drawlist = ImGui::GetWindowDrawList();
    drawlist->AddCircle(ImVec2(x, y), radius, col);
}

void maddox::filledcircle(int x, int y, float radius, ImU32 col, bool foreground) {
    auto drawlist = foreground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
    drawlist->AddCircleFilled(ImVec2(x, y), radius, col, 155);
}

void maddox::text(int x, int y, const char* text, ImU32 col, bool foreground) {
    auto drawlist = foreground ? ImGui::GetForegroundDrawList() : ImGui::GetWindowDrawList();
    drawlist->AddText(ImVec2(x, y), col, text);
}

void maddox::gui::padding(int x, int y) {
    ImRect additional_padding(ImVec2(0, 0), ImVec2(x, y));
    ImGui::ItemSize(additional_padding);
}

float tab_inner_padding = 10.f;
bool in_tab_change = false;
std::string next_tab;

std::unordered_map<std::string, bool> tabs;

bool maddox::gui::tab(const char* icon, const char* title, float sidebar_value) {
    auto drawlist = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetWindowPos();
    pos = ImVec2(pos.x, pos.y + ImGui::GetCursorPosY());
    ImGui::PushFont(largeFont);
    size_t hash = std::hash<const char*>()(title);
    if (tabs[title] == NULL)
        tabs.emplace(title, false);
    bool v = tabs[title];
    bool return_true = false;

    if (tab_animation.find(hash) == tab_animation.end()) {
        tab_animation.insert({ hash, 0.f });
        tab_animation_timer.insert({ hash, 0.f });
    }

    if (v) {
        if (tab_animation.at(hash) < 1.f) {
            tab_animation_timer.at(hash) += ImGui::GetIO().DeltaTime * 3;
            tab_animation_timer.at(hash) = std::clamp<float>(tab_animation_timer.at(hash), 0.f, 1.f);
            tab_animation.at(hash) = easing::InSine(tab_animation_timer.at(hash));
        }
    }
    else {
        if (tab_animation.at(hash) > 0.f) {
            tab_animation_timer.at(hash) -= ImGui::GetIO().DeltaTime * 3;
            tab_animation_timer.at(hash) = std::clamp<float>(tab_animation_timer.at(hash), 0.f, 1.f);
            tab_animation.at(hash) = easing::OutSine(tab_animation_timer.at(hash));
        }
    }

    float alpha = tab_animation.at(hash);

    auto textsize = ImGui::CalcTextSize(title);

    if (ImGui::IsMouseHoveringRect(ImVec2(pos.x + 15, pos.y), ImVec2(pos.x + 75 + sidebar_value * 90, pos.y + 30)) && ImGui::IsMouseClicked(0)) {
        in_tab_change = true;
        next_tab = title;
        return_true = true;
    }

    if (in_tab_change && next_tab == title) {
        for (auto a : tabs)
            tabs[a.first] = false;
        tabs[title] = true;
    }

    ImGui::PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 75 + sidebar_value * 90, pos.y + 40), false);
    ImGui::PushFont(ficon);
    ImVec2 icon_size = ImGui::CalcTextSize(icon);
    drawlist->AddText(ImVec2(pos.x + (75 / 2) - icon_size.x / 2, pos.y), IM_COL32(0, 141, 255, 100 + alpha * 155), icon);
    ImGui::PopFont();
    ImGui::PushFont(largeFont);
    ImVec2 text_size = ImGui::CalcTextSize(title);
    drawlist->AddText(ImVec2(pos.x + 75, pos.y + icon_size.y / 2 - text_size.y / 2), IM_COL32(255, 255, 255, sidebar_value * 150 + alpha * 105), title);
    ImGui::PopFont();
    ImGui::PopFont();
    ImGui::PopClipRect();


    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);

    return return_true;
}

bool route_in_change = false;
int desired_route_end = 0;
float route_up = 0.f;
float route_up_anim = 0.f;
float route_down = 0.f;
float route_down_anim = 0.f;

bool maddox::gui::keybind(const char* id, int* key_index) {
    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() - 33));
    ImGuiIO& io = ImGui::GetIO(); // using imguis keysdown utility
    auto drawlist = ImGui::GetWindowDrawList();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID hash = window->GetID(id);
    ImVec2 pos = ImVec2(ImGui::GetCursorPos().x + ImGui::GetWindowPos().x + 30, ImGui::GetCursorPos().y + ImGui::GetWindowPos().y + 4);
    ImRect total_bb(pos, ImVec2(pos.x + 40, pos.y + 16));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    ImGui::ItemAdd(total_bb, hash);
    bool value_changed = false;
    bool active = false;
    if (g.ActiveId == hash)
        active = true;

    if (keybind_animation.find(hash) == keybind_animation.end()) {
        keybind_animation.insert({ hash, 0.f });
        keybind_animation_timer.insert({ hash, 0.f });
    }

    float animation = keybind_animation.at(hash);

    if (active) {
        if (keybind_animation_timer.at(hash) < 1.f) {
            keybind_animation_timer.at(hash) += ImGui::GetIO().DeltaTime * 4;
            keybind_animation_timer.at(hash) = std::clamp(keybind_animation_timer.at(hash), 0.f, 1.f);
            keybind_animation.at(hash) = easing::InCubic(keybind_animation_timer.at(hash));
        }
    }
    else {
        if (keybind_animation_timer.at(hash) > 0.f) {
            keybind_animation_timer.at(hash) -= ImGui::GetIO().DeltaTime * 3;
            keybind_animation_timer.at(hash) = std::clamp(keybind_animation_timer.at(hash), 0.f, 1.f);
            keybind_animation.at(hash) = easing::InCubic(keybind_animation_timer.at(hash));
        }
    }

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, hash, &hovered, &held);
    if (pressed)
    {
        ImGui::SetActiveID(hash, window);
        ImGui::SetFocusID(hash, window);
    }

    if (!ImGui::IsMouseHoveringRect(total_bb.Min, total_bb.Max) && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && g.ActiveId == hash)
        ImGui::ClearActiveID();

    int key = *key_index;

    if (active) {
        for (auto i = 0; i < 5; i++) {
            if (io.MouseDown[i] && i != 0) {
                switch (i) {
                case 1:
                    key = VK_RBUTTON;
                    break;
                case 2:
                    key = VK_MBUTTON;
                    break;
                case 3:
                    key = VK_XBUTTON1;
                    break;
                case 4:
                    key = VK_XBUTTON2;
                    break;
                }
                value_changed = true;
                ImGui::ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = VK_BACK; i <= VK_RMENU; i++) {
                if (io.KeysDown[i]) {
                    key = i;
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }
        }

        if (ImGui::IsKeyPressedMap(ImGuiKey_Escape)) {
            *key_index = 0;
            ImGui::ClearActiveID();
        }
        else {
            *key_index = key;
        }
    }

    char buf_display[64] = "None";

    if (*key_index != 0 && !active) {
        strcpy_s(buf_display, KeyNames[*key_index]);
    }
    else if (active) {
        strcpy_s(buf_display, "...");
    }

    /*drawlist->AddRectFilled(pos, ImVec2(pos.x + 40, pos.y + 16), IM_COL32(19, 18, 23, 255), 2.f);
    drawlist->AddRectFilled(pos, ImVec2(pos.x + animation * 40, pos.y + animation * 16), IM_COL32(239, 49, 77, animation * 255), 2.f);
    drawlist->AddRectFilled(ImVec2(pos.x + 1, pos.y + 1), ImVec2(pos.x + 39, pos.y + 15), IM_COL32(19, 18, 23, 255), 1.f);*/

    drawlist->AddRectFilled(ImVec2(pos.x, pos.y + 14), ImVec2(pos.x + animation * 40, pos.y + 16), IM_COL32(0, 140, 255, 255), 0.f);

    ImVec2 key_textsize = ImGui::CalcTextSize(buf_display);
    drawlist->AddText(ImVec2(pos.x + 20 - key_textsize.x / 2, pos.y + 1), ImGui::GetItemFlags() & ImGuiItemFlags_Disabled ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 155 + animation * 100), buf_display);

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 20, ImGui::GetCursorPosY() + 12));

    return value_changed;
}

void maddox::gui::separator() {
    ImVec2 pos = ImGui::GetCursorPos();
    pos = ImVec2(ImGui::GetWindowPos().x + pos.x, ImGui::GetWindowPos().y + pos.y + 10);
    auto drawlist = ImGui::GetForegroundDrawList();
    drawlist->AddLine(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x - 20, pos.y), IM_COL32(49, 48, 53, 255));
}

void maddox::setup() {
    if (ficon == nullptr) {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImFontConfig cfg_;
        cfg_.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_LightHinting;
        ImFontConfig cfg;
        cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_NoHinting;
        ficon = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\icons.ttf", 30.f, &cfg_, io.Fonts->GetGlyphRangesCyrillic());
        largeFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\MuseoSans.ttf", 20.f, &cfg_, io.Fonts->GetGlyphRangesCyrillic());
        largeFont_nb = io.Fonts->AddFontFromFileTTF("C:\\Users\\ryker\\AppData\\Local\\Microsoft\\Windows\\Fonts\\MuseoSans.ttf", 11, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    }
}

void maddox::line(int x, int y, int x2, int y2, ImU32 col, float size) {
    auto drawlist = ImGui::GetWindowDrawList();
    drawlist->AddLine(ImVec2(x, y), ImVec2(x2, y2), col, size);
}
