#include <lua.hpp>

struct Pixel_RGBA {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};
unsigned char checkOverflow(double num) {
    if (num < 0) {
        return 0;
    }
    else if (num > 255) {
        return 255;
    }

    return (unsigned char)num;
}

// colorize(data, width, height, r, g, b)
int colorize(lua_State* L) {
    Pixel_RGBA* pixels = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));
    int width = lua_tointeger(L, 2);
    int height = lua_tointeger(L, 3);

    int r = lua_tointeger(L, 4);
    int g = lua_tointeger(L, 5);
    int b = lua_tointeger(L, 6);

    int mode = lua_tointeger(L, 7);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = x + width * y;

            if (mode == 1) {
                double tempR = 255 - pixels[index].r;
                double tempG = 255 - pixels[index].g;
                double tempB = 255 - pixels[index].b;

                pixels[index].r = checkOverflow((tempR / 255) * (double)r + pixels[index].r);
                pixels[index].g = checkOverflow((tempG / 255) * (double)g + pixels[index].g);
                pixels[index].b = checkOverflow((tempB / 255) * (double)b + pixels[index].b);
            }
            else if (mode == 2) {
                double tempR = 255 - pixels[index].r;
                double tempG = 255 - pixels[index].g;
                double tempB = 255 - pixels[index].b;

                pixels[index].r = checkOverflow((tempR / 255) * (double)r);
                pixels[index].g = checkOverflow((tempG / 255) * (double)g);
                pixels[index].b = checkOverflow((tempB / 255) * (double)b);
            }
        }
    }

    return 0;
}

static luaL_Reg functions[] = {
    {"colorize", colorize},
    { nullptr, nullptr }
};

extern "C" {
    __declspec(dllexport) int luaopen_S_Colorize_Module(lua_State* L) {
        luaL_register(L, "S_Colorize_Module", functions);
        return 1;
    }
}