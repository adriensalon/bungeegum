#include <sstream>

#include <bungeegum/core/global.fwd>

namespace bungeegum {
namespace detail {

// #if BUNGEEGUM_USE_HOTSWAP
    

//     std::optional<std::string> hotswap_manager::inspect_reloadable_widget(const widget_update_data& update_data)
//     {
//         std::stringstream _output_stream;
//         if (update_data.saver) {
//             {
//                 reloaded_saver _archiver(_output_stream);
//                 update_data.saver(_archiver);
//             }
//             return _output_stream.str();
//         } else {
//             return std::nullopt;
//         }
//     }

//     void hotswap_manager::update_reloadable_widget(widget_update_data& update_data, const std::string& serialized)
//     {
//         std::stringstream _input_stream;
//         _input_stream << serialized;
//         if (update_data.loader) {
//             {
//                 reloaded_loader _archiver(_input_stream);
//                 update_data.loader(_archiver);
//             }
//         }
//     }
// #endif

// #if BUNGEEGUM_USE_OVERLAY
//     std::string clean_typename(const std::string& raw_typename)
//     {
//         std::size_t _last_space = raw_typename.find_last_of(' ');
//         if (_last_space == raw_typename.npos) {
//             _last_space = 0;
//         }
//         std::size_t _last_column = raw_typename.find_last_of(':');
//         if (_last_column == raw_typename.npos) {
//             _last_column = 0;
//         }
//         std::size_t _offset = std::max(_last_space, _last_column) + 1;
//         std::size_t _length = raw_typename.length() - _offset;
//         return raw_typename.substr(_offset, _length);
//     }

//     // void hotswap_manager::set_clean_typename(widget_update_data& update_data)
//     // {
//     //     update_data.resolver_command._data.clean_typename = clean_typename(update_data.kind->name());
//     // }
// #endif
}

}