#pragma once
#include "../imgui.h"
#include "../imgui_internal.h"
#include <map>
#include <Windows.h>
#include <any>
#include <filesystem>
#include <deque>
#include <vector>
#include <windows.h>

inline std::map<size_t, float> tab_animation_timer;
inline std::map<size_t, float> tab_animation;
inline std::map<size_t, float> checkbox_animation_timer;
inline std::map<size_t, float> checkbox_animation;
inline std::map<size_t, float> slider_animation_timer;
inline std::map<size_t, float> slider_animation;
inline std::map<size_t, float> route_up_timer;
inline std::map<size_t, float> route_up_animation;
inline std::map<size_t, float> route_down_timer;
inline std::map<size_t, float> route_down_animation;
inline std::map<size_t, float> keybind_animation_timer;
inline std::map<size_t, float> keybind_animation;
inline std::map<size_t, float> dropdown_animation_timer;
inline std::map<size_t, float> dropdown_animation;

namespace easing {
#define M_PI 3.14159265358979323846

    __forceinline double InSine(const double t) {
        return sin(M_PI / 2 * t);
    }

    __forceinline double OutSine(double t) {
        return 1 + sin(M_PI / 2 * (--t));
    }

    __forceinline double InOutSine(const double t) {
        return 0.5 * (1 + sin(M_PI * (t - 0.5)));
    }

    __forceinline double InQuad(const double t) {
        return t * t;
    }

    __forceinline double OutQuad(const double t) {
        return t * (2 - t);
    }

    __forceinline double InOutQuad(const double t) {
        return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
    }

    __forceinline double InCubic(const double t) {
        return t * t * t;
    }

    __forceinline double OutCubic(double t) {
        return 1 + (--t) * t * t;
    }

    __forceinline double InOutCubic(double t) {
        return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
    }

    __forceinline double InQuart(double t) {
        t *= t;
        return t * t;
    }

    __forceinline double OutQuart(double t) {
        t = (--t) * t;
        return 1 - t * t;
    }

    __forceinline double InOutQuart(double t) {
        if (t < 0.5) {
            t *= t;
            return 8 * t * t;
        }
        else {
            t = (--t) * t;
            return 1 - 8 * t * t;
        }
    }

    __forceinline double InQuint(const double t) {
        const double t2 = t * t;
        return t * t2 * t2;
    }

    __forceinline double OutQuint(double t) {
        const double t2 = (--t) * t;
        return 1 + t * t2 * t2;
    }

    __forceinline double InOutQuint(double t) {
        double t2;
        if (t < 0.5) {
            t2 = t * t;
            return 16 * t * t2 * t2;
        }
        else {
            t2 = (--t) * t;
            return 1 + 16 * t * t2 * t2;
        }
    }

    __forceinline double InExpo(const double t) {
        return (pow(2, 8 * t) - 1) / 255;
    }

    __forceinline double OutExpo(const double t) {
        return 1 - pow(2, -8 * t);
    }

    __forceinline double InOutExpo(const double t) {
        if (t < 0.5) {
            return (pow(2, 16 * t) - 1) / 510;
        }
        else {
            return 1 - 0.5 * pow(2, -16 * (t - 0.5));
        }
    }

    __forceinline double InCirc(const double t) {
        return 1 - sqrt(1 - t);
    }

    __forceinline double OutCirc(const double t) {
        return sqrt(t);
    }

    __forceinline double InOutCirc(const double t) {
        if (t < 0.5) {
            return (1 - sqrt(1 - 2 * t)) * 0.5;
        }
        else {
            return (1 + sqrt(2 * t - 1)) * 0.5;
        }
    }

    __forceinline double InBack(const double t) {
        return t * t * (2.70158 * t - 1.70158);
    }

    __forceinline double OutBack(double t) {
        return 1 + (--t) * t * (2.70158 * t + 1.70158);
    }

    __forceinline double InOutBack(double t) {
        if (t < 0.5) {
            return t * t * (7 * t - 2.5) * 2;
        }
        else {
            return 1 + (--t) * t * 2 * (7 * t + 2.5);
        }
    }

    __forceinline double InElastic(const double t) {
        const double t2 = t * t;
        return t2 * t2 * sin(t * M_PI * 4.5);
    }

    __forceinline double OutElastic(const double t) {
        const double t2 = (t - 1) * (t - 1);
        return 1 - t2 * t2 * cos(t * M_PI * 4.5);
    }

    __forceinline double InOutElastic(const double t) {
        double t2;
        if (t < 0.45) {
            t2 = t * t;
            return 8 * t2 * t2 * sin(t * M_PI * 9);
        }
        else if (t < 0.55) {
            return 0.5 + 0.75 * sin(t * M_PI * 4);
        }
        else {
            t2 = (t - 1) * (t - 1);
            return 1 - 8 * t2 * t2 * sin(t * M_PI * 9);
        }
    }

    __forceinline double InBounce(const double t) {
        return pow(2, 6 * (t - 1)) * abs(sin(t * M_PI * 3.5));
    }

    __forceinline double OutBounce(const double t) {
        return 1 - pow(2, -6 * t) * abs(cos(t * M_PI * 3.5));
    }

    __forceinline double InOutBounce(const double t) {
        if (t < 0.5) {
            return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * M_PI * 7));
        }
        else {
            return 1 - 8 * pow(2, -8 * t) * abs(sin(t * M_PI * 7));
        }
    }
}

enum tabs {
    NIGGA_AIMBOT,
    NIGGA_VISUALS,
    NIGGA_MISC
};

namespace maddox {
    void setup();

    void rect(int x, int y, int size_x, int size_y, ImU32 col, float rounding = 0, ImDrawFlags flags = NULL, bool is_foreground = false);
    void line(int x, int y, int x2, int y2, ImU32 col, float size = 1.f);
    void filledrect(int x, int y, int size_x, int size_y, ImU32 col, float rounding = 0, ImDrawFlags flags = NULL);
    void gradientrect(int x, int y, int size_x, int size_y, ImU32 col_top, ImU32 col_bottom, bool is_foreground = false);
    void gradientrect_horizontal(int x, int y, int size_x, int size_y, ImU32 col_left, ImU32 col_right, bool is_foreground = false);
    void circle(int x, int y, float radius, ImU32 col);
    void filledcircle(int x, int y, float radius, ImU32 col, bool foreground = false);
    void text(int x, int y, const char* text, ImU32 col, bool foreground = false);

    namespace gui {
        bool tab(const char* icon, const char* title, float sidebar_value);
        bool keybind(const char* id, int* key_index);
        void separator();
        void padding(int x, int y);
    }
}
