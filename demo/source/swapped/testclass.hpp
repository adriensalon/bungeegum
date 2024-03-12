#pragma once

#include <bungeegum_widgets/bungeegum_widgets.hpp>

using namespace bungeegum;

struct testclass {

    // bungeegum::font myfont = { "D:/bungeegum/demo/static/Lobster.ttf", 17.f };
    // bungeegum::texture mytexture = { "D:/bungeegum/demo/static/Image.jpg" };

    HOTSWAP_METHOD void draw(bungeegum::draw_command& command);


    int gg = 44;
    std::string ijijij = "llllllll";

private:
    HOTSWAP_CLASS(testclass, gg, ijijij)
}; 