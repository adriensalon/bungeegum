//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <optional>

#include <tree.hh>

namespace staticgui {
namespace internal {
    namespace id {

        using id_int = unsigned long long;

        template <typename value_t>
        using id_tree = tree<std::pair<id_int, std::optional<value_t>>>;

        namespace detail {
            template <typename value_t>
            inline id_tree<value_t>& get_static_id_tree()
            {
                static id_tree<value_t> _static_tree;
                return _static_tree;
            }
        }

        struct id_interface {
            id_interface()
                : id(_count)
            {
                _count++;
            }

        protected:
            const id_int id;

        private:
            inline static id_int _count = 0;
        };

        template <typename value_t>
        typename id_tree<value_t>::iterator prepare_in_tree_head(const id_int value_id)
        {
            id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            return _static_tree.insert(_static_tree.begin(), { value_id, std::nullopt });
        }

        template <typename value_t>
        typename id_tree<value_t>::iterator find_in_tree(const id_int value_id)
        {
            id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            id_tree<value_t>::iterator _it = _static_tree.begin();
            while (_it != _static_tree.end()) {
                id_int _id = _pair.first;
                if (_id == value_id)
                    return _it;
            }
            return _static_tree.end();
        }

        template <typename value_t>
        typename id_tree<value_t>::iterator find_in_tree_head(const id_int value_id)
        {
            id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            id_tree<value_t>::sibling_iterator _sibling_it = _static_tree.begin();
            while (_sibling_it != _static_tree.end()) {
                id_int _id = _sibling_it->first;
                if (_id == value_id)
                    return _sibling_it;
                _sibling_it++;
            }
            return prepare_in_tree_head<value_t>(value_id);
        }

        template <typename value_t>
        void assign_in_tree_head(const id_int value_id, value_t&& value)
        {
            std::cout << "assign node " << value_id << " of type " << value.typeid_name << std::endl;
            id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            id_tree<value_t>::iterator _value_it = find_in_tree_head<value_t>(value_id);
            _value_it->second = std::move(value);
        }

        template <typename value_t>
        void reparent_in_tree_head(const id_int parent_id, const id_int child_id)
        {
            std::cout << "reparent " << child_id << " -> " << parent_id << std::endl;
            id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            id_tree<value_t>::iterator _parent_it = find_in_tree_head<value_t>(parent_id);
            id_tree<value_t>::iterator _child_it = find_in_tree_head<value_t>(child_id);
            _static_tree.reparent(_parent_it, _child_it);
        }

        // traverse !!

        // void test()
        // {
        //     struct ok {
        //         int y = 4;
        //         int z = 4;
        //     };

        //     ok _ok;
        //     ok& _okr = _ok;
        //     tree<ok> _t;
        //     auto& _tr = _t.insert(_t.begin(), std::move(_ok));
        //     _okr.y++;
        //     std::cout << _okr.y << std::endl;

        //     ok&& _ok2 = std::move(*_tr);
        //     _okr.y++;
        //     std::cout << _okr.y << std::endl;

        //     _tr = _t.insert(_t.begin(), std::move(_ok));
        // }

    }
}
}