//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <cstdint>
#include <mutex>

#include <bungeegum/bungeegum.hpp>
#include <bungeegum/widgets/image.hpp>

namespace bungeegum {
namespace widgets {

    struct container {

        // template <typename child_widget_t>
        container()
        {
            // register_widget(this);
            // build(this, child_widget);
            // // testanim.assign(this, _tick_value);
            // curve mycurve(0.f, 1.f, { float2 { 0.6f, 0.88f } });
            // animation<float> myanim(mycurve, 0.5f, 1.5f);
            // myanim.max(100.f).min(20.f).duration(9).shape(mycurve);
            // myanim.on_tick([this](const float& _value) {
            //     _tick_value = _value;
            //     must_draw(this);
            // });
            // myanim.start().detach();
            curve mycurve(0.f, 1.f, { float2 { 0.6f, 0.88f } });
            myanim = std::make_unique<animation<float>>(mycurve, 0.5f, 1.5f);
            myanim->max(100.f).min(20.f).duration(3).shape(mycurve);
            myanim->on_tick([this](const float& _value) {
                _tick_value = _value;
                // std::cout << "playin " << reinterpret_cast<uintptr_t>(this) << std::endl;
                std::mutex _mut;
                _mut.lock();
                must_draw(this);
                _mut.unlock();
            });

            myanim->start(); //.detach(*this);
            adopt(this, make<image>());
        }

        container& ok()
        {
            // register_widget(this);
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
            std::cout << "~~~~~~ interacting... \n";
        }

        template <typename child_widget_t>
        float2 resolve(resolve_command& command, child_widget_t& child_widget)
        {
            std::cout << "resolving..." << std::endl;
            // return command.constraint().biggest();
            return { 600.f, 0.f };
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
        std::unique_ptr<animation<float>> myanim;
    };
}
}