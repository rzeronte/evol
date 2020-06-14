//
// Created by darkhead on 12/6/20.
//

#ifndef EVOL_COLORS_H
#define EVOL_COLORS_H

class Colors {

public:
    static unsigned long createRGB(int r, int g, int b) {
        return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    }

    static Uint32 white()      { return 0xFFFFFF; }
    static Uint32 red()        { return 0xFF0000; }
    static Uint32 green()      { return 0x00FF00; }
    static Uint32 blue()       { return 0x0000FF; }
    static Uint32 black()      { return 0x000000; }
    static Uint32 yellow()     { return 0xFFFF00; }
    static Uint32 pink()       { return 0xff00bf; }
    static Uint32 cyan()       { return 0x00FFFF; }
    static Uint32 orange()     { return 0xFF8000; }
    static Uint32 magenta()    { return 0xFF00FF; }
    static Uint32 gray()       { return 0x323232; }
    static Uint32 darkwhite()  { return 0xF5F5F5; }
    static Uint32 FOGDefault() { return 0x250000; }
};
#endif //EVOL_COLORS_H
