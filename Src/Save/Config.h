#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <Misc/String.h>

enum class WindowType {
    Windowed,
    Fullscreen
};

class Config {
private:
    const PGE::String file = "options.ini";

    WindowType windowType;
    int width;
    int height;
    float aspectRatio;

    bool vsync;

public:
    static void initialize();

    Config();

    void setResolution(int width, int height);
    int getWidth() const;
    int getHeight() const;
    float getAspectRatio() const;

    float isVsync() const;
};

extern Config config;

#endif // CONFIG_H_INCLUDED