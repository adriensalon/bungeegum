#pragma once

#include <bungeegum_widgets/bungeegum_widgets.hpp>

using namespace bungeegum;

struct IkkkOk {

	IkkkOk();

    // bungeegum::font myfont = { "C:/Users/adri/dev/bungeegum/demo/static/Lobster.ttf", 17.f };
    // bungeegum::texture mytexture = { "C:/Users/adri/dev/bungeegum/demo/static/Image.jpg" };
    bungeegum::font myfont = { "D:/bungeegum/demo/static/Lobster.ttf", 17.f };
    bungeegum::texture mytexture = { "D:/bungeegum/demo/static/Image.jpg" };

    virtual void draw(draw_command& command);


    int gg = 44;
    std::string ijijij = "llllllll";

// private:
    HOTSWAP_CLASS(IkkkOk, gg, ijijij)
}; 