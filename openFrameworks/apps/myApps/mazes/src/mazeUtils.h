#pragma once

#include <string>

#include "ofMain.h"

using namespace std;

inline string to_base36(unsigned int value) {
    static constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (value == 0) return "0";

    string result;
    while (value > 0) {
        result.push_back(digits[value % 36]);
        value /= 36;
    }
    reverse(result.begin(), result.end());

    return result;
}

inline string repeat(string_view str, size_t times) {
    string result;
    result.reserve(str.size() * times);
    for (size_t i = 0; i < times; ++i)
        result += str;

    return result;
}

// MARK: - Colors

struct OKLab {
    float L, a, b;
};

inline float srgb2linear(float c) {
    return (c <= 0.04045f)
        ? c / 12.92f
        : std::pow((c + 0.055f) / 1.055f, 2.4f);
}

inline float linear2srgb(float c) {
    return (c <= 0.0031308f)
        ? 12.92f * c
        : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
}

inline OKLab rgb2oklab(const ofColor& rgb) {
    // convert 0..255 sRGB -> 0..1 linear RGB
    float r = srgb2linear(rgb.r / 255.0f);
    float g = srgb2linear(rgb.g / 255.0f);
    float b = srgb2linear(rgb.b / 255.0f);

    // linear RGB -> LMS
    float l = 0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b;
    float m = 0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b;
    float s = 0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b;

    float l_ = std::cbrt(l);
    float m_ = std::cbrt(m);
    float s_ = std::cbrt(s);

    return OKLab{
        0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
        1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
        0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_
    };
}

inline ofColor oklab2rgb(const OKLab& lab) {
    float l_ = lab.L + 0.3963377774f * lab.a + 0.2158037573f * lab.b;
    float m_ = lab.L - 0.1055613458f * lab.a - 0.0638541728f * lab.b;
    float s_ = lab.L - 0.0894841775f * lab.a - 1.2914855480f * lab.b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    float r = 4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
    float g = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
    float b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

    // linear -> sRGB -> 0..255
    return ofColor{
        std::clamp(linear2srgb(r), 0.0f, 1.0f) * 255.0f,
        std::clamp(linear2srgb(g), 0.0f, 1.0f) * 255.0f,
        std::clamp(linear2srgb(b), 0.0f, 1.0f) * 255.0f
    };
}

// returns a new color of given intensity based on the target tone color (white -> tone -> black)
inline ofColor color_intensity(const ofColor& tone, float intensity) {
    intensity = std::clamp(intensity, 0.0f, 1.0f);

    OKLab mid = rgb2oklab(tone);
    OKLab white{1.0f, 0.0f, 0.0f};
    OKLab black{0.0f, 0.0f, 0.0f};

    // calculating where to split the lightness range for the tone, since it's not always in the middle (0.5),
    // i.e. green color is already very light and its L is ~0.8,
    // which means we get very short range of light colors and wide range of dark colors,
    // which is not what we want because the maze will be mostly rich with dark tones.
    float split = 1.0f - mid.L;
    split       = std::clamp(split, 1e-4f, 1.0f - 1e-4f);

    OKLab out;
    if (intensity < split) {
        // moving from white toward the tone
        float u = intensity / split;
        out     = {ofLerp(white.L, mid.L, u), ofLerp(white.a, mid.a, u), ofLerp(white.b, mid.b, u)};
    } else {
        // passed the tone and now we're moving toward black
        float u = (intensity - split) / (1.0f - split);
        out     = {ofLerp(mid.L, black.L, u), ofLerp(mid.a, black.a, u), ofLerp(mid.b, black.b, u)};
    }
    return oklab2rgb(out);
}
