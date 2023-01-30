//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/

#include <staticgui/staticgui.hpp>

namespace staticgui {

namespace internal {
    namespace impl {

        // application& get_application()
        // {
        //     // static std::optional<application> _application = std::nullopt;
        //     static application _application;
        //     return _application;
        // }

    }
}

// application& application::title(const std::string& window_title)
// {
//     return *this;
// }

// application& application::size(const unsigned int window_width, const unsigned int window_height)
// {
//     return *this;
// }

// application& application::on_resized(std::function<void()> on_window_resized_callback)
// {
//     return *this;
// }

// application& application::debug_stream(const std::ostream& stream)
// {
//     return *this;
// }

#if defined(STATICGUI_DEBUG)

void print_build_tree()
{
    // internal::id::print_tree<internal::detail::widget_impl>();
    // internal::detail::runtime_context _rc;
    // _it->second.update_callback(_rc);
}

#endif

}