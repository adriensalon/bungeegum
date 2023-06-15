#pragma once

#include <cstdint>
#include <mutex>

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/image.hpp>

namespace bungeegum {
namespace widgets {

    struct container {

        // image _image_child;

        container()
        {
            curve mycurve(0.f, 1.f, { float2 { 0.6f, 0.88f } });
            auto& myanim = make_animation<float>()
                               .max(100.f)
                               .min(20.f)
                               .duration(3)
                               .shape(mycurve)
                               .on_tick([this](const float& _value) {
                                   _tick_value = _value;
                                   std::mutex _mut;
                                   _mut.lock();
                                   must_draw(this);
                                   _mut.unlock();
                               })
                               .start();
            // adopt(this, _image_child);
        }

        container& ok()
        {
            // adopt(this, _image_child);
            // std::uintptr_t _raw_widget = detail::get_raw_widget<container>(*this);
            // detail::register_widget(*this, _raw_widget);
            return *this;
        }

        template <typename child_widget_t>
        container& child(child_widget_t& child_widget)
        {
            adopt(this, child_widget);
            // build(this, child_widget);
            // testanim.assign(this, _tick_value);
            return *this;
        }

        inline void interact(interact_command& command)
        {
            // std::cout << "~~~~~~ interacting... \n";
            command.block();
            switch (command.type()) {
            case interaction_type::window_resized: {
                const window_resized_interaction& _interaction_wr = command.window_resized().value();
                std::cout << "window_resized" << std::endl;
                // throw_error("okok");
                // detail::throw_error<detail::error_type::bad_usage>("okok");
                // unmake(_image_child);
                must_resolve(this);
                break;
            }
            case interaction_type::mouse_moved: {
                const mouse_moved_interaction& _interaction_mm = command.mouse_moved().value();
                std::cout << "mouse_moved" << std::endl;
                must_draw(this);
                break;
            }
            default:
                break;
            }
        }

        inline void resolve(resolve_command& command)
        {
            // std::cout << "resolving..." << std::endl;
            // return command.constraint().biggest();
            // return { 600.f, 0.f };
            must_draw(this);
        }

        inline void draw(draw_command& command)
        {
            // std::cout << "x = " << size.x() << std::endl;
            command.draw_rect(
                { 20.f, 20.f },
                { 600.f, 8 * _tick_value },
                { _tick_value * 0.01f, 0.33f, 0.89f, 1.f },
                12,
                40);
            command.draw_rect(
                { 400.f, 400.f },
                { 500.f, 420 + _tick_value },
                { 0.81f, 0.53f, _tick_value * 0.0089f, 1.f },
                10,
                10);

            // detail::throw_library_bad_implementation("helloww");
            // detail::throw_library_bad_usage("helloww");
            // throw_error("my user error");
        }

    private:
        float _tick_value = 20.f;
        // animation<float> myanim;
    };
}
}