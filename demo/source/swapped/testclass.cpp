#include "testclass.hpp"

void testclass::draw(draw_command& command)
{
    float2 _min_point = float2 { 500.f, 500.f };
    float2 _max_point_1 = _min_point + float2 { 100.f, 100.f };
    float2 _max_point_2 = _min_point + float2 { 500.f, 500.f };
    command.use_shader_mask();
    command.draw_rect_filled(_min_point - float2 {10.f, 10.f }, _max_point_1, { 1.f, 1.f, 0.5f, 1.f });
    command.draw_rect_filled(_min_point + float2 {10.f, 10.f }, _max_point_1 + float2 {30.f, 30.f }, { 1.f, 1.f, 0.5f, 1.f });
    
    command.use_shader_default();
    command.draw_rect_filled(_min_point, _max_point_2, { 1.f, 1.f, 0.5f, 1.f });
    command.draw_rect_filled(_min_point + float2 {10.f, 10.f }, _max_point_1 + float2 {30.f, 30.f }, { 1.f, 1.f, 0.5f, 1.f });
    
    // command.draw_texture(mytexture, _min_point, _max_point_2);
    // command.draw_rect_filled(_min_point, _max_point_2, { 1.f, 0.5f, 0.5f, 1.f });
    // command.draw_text(myfont, 17.f, { 400, 500 }, { 1.f, 0.5f, 1.f, 1.f }, "HELLO            ");
    // log_error("yaaaaa");
    
}