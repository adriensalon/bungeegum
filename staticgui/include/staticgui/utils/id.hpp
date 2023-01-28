//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <any>
#include <optional>

template <typename value_t>
std::ostream& operator<<(std::ostream& os, const std::optional<value_t>& o)
{
    return os << std::to_string(o.has_value());
}

template <typename value_left_t, typename value_right>
std::ostream& operator<<(std::ostream& os, const std::pair<value_left_t, value_right>& o)
{
    return os << o.first << " " << o.second << std::endl;
}

// #include <tree.hh>
// #include <tree_util.hh>

namespace staticgui {
namespace internal {
    namespace id {

        using integer = unsigned long long;

        struct id_generator {
            static integer create()
            {
                integer _retval;
                if (!_deleted.empty()) {
                    _retval = _deleted.back();
                    _deleted.pop_back();
                } else
                    _retval = _count++;
                return _retval;
            }

            static integer next()
            {
                integer _retval;
                if (!_deleted.empty()) {
                    _retval = _deleted.back();
                } else
                    _retval = _count + 1;
                return _retval;
            }

            static void destroy(const integer id)
            {
                _deleted.emplace_back(id);
            }

        private:
            inline static integer _count = 0;
            inline static std::vector<integer> _deleted;
        };

        template <typename value_t>
        void print_tree(const std::tree<value_t>& tree)
        {
            std::tree<value_t>::iterator _it = tree.begin();
            _it++;
            std::cout << "application" << std::endl;
            while (_it != tree.end()) {

                int _depth = tree.depth(_it);
                for (int _k = 0; _k < _depth; _k++)
                    std::cout << "   ";
                std::cout << "|__ ";
                std::cout << (*_it)->display_typename;
                if ((*_it)->widget_data.paint_callback)
                    std::cout << " [painter]";
                std::cout << std::endl;
                _it++;
            }
        }
    }
}
}