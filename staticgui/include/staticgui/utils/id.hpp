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

        using id_int = unsigned long long;

        struct id_generator {
            static id_int create()
            {
                id_int _retval;
                if (!_deleted.empty()) {
                    _retval = _deleted.back();
                    _deleted.pop_back();
                } else
                    _retval = _count++;
                return _retval;
            }

            static id_int next()
            {
                id_int _retval;
                if (!_deleted.empty()) {
                    _retval = _deleted.back();
                } else
                    _retval = _count + 1;
                return _retval;
            }

            static void destroy(const id_int id)
            {
                _deleted.emplace_back(id);
            }

        private:
            inline static id_int _count = 0;
            inline static std::vector<id_int> _deleted;
        };

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

        // template <typename value_t>
        // using id_tree = tree<std::pair<id_int, value_t>>;

        // namespace detail {
        //     template <typename value_t>
        //     inline id_tree<value_t>& get_static_id_tree()
        //     {
        //         static id_tree<value_t> _static_tree;
        //         return _static_tree;
        //     }
        // }

        template <typename value_t>
        void print_tree(const std::tree<value_t>& tree)
        {
            // kptree::print_subtree_bracketed<std::pair<id_int, std::optional<value_t>>>(detail::get_static_id_tree<value_t>(), detail::get_static_id_tree<value_t>().begin());
            // std::tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
            std::tree<value_t>::iterator _it = tree.begin();
            _it++;

            // auto k1 = _static_tree.append_child(_it, { 11, std::nullopt });
            // auto k2 = _static_tree.append_child(_it, { 22, std::nullopt });
            // auto k3 = _static_tree.append_child(_it, { 33, std::nullopt });
            // auto k4 = _static_tree.append_child(k2, { 44, std::nullopt });
            // _static_tree.move_ontop(k4, k3);

            std::cout << "application" << std::endl;
            // while (_it != _static_tree.end()) {
            //     _static_tree.reparent(k1, _it);
            //     _it++;
            // }

            while (_it != tree.end()) {

                int _depth = tree.depth(_it);
                for (int _k = 0; _k < _depth; _k++)
                    std::cout << "   ";
                std::cout << "|__ ";

                // if (_it->second.has_value())
                std::cout << _it->display;

                std::cout << std::endl;

                // id_int _id = _it.first;
                _it++;
            }
        }

        // template <typename value_t>
        // value_t& emplace_in_tree(const id_int value_id)
        // {
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();

        //     static bool _f = false;
        //     if (!_f) {
        //         _static_tree.set_head({ 444, {} });
        //         _f = true;
        //     }

        //     id_tree<value_t>::iterator _it = _static_tree.append_child(_static_tree.begin());
        //     _it->first = value_id;
        //     return _it->second;
        // }

        // template <typename value_t>
        // typename id_tree<value_t>::iterator prepare_in_tree_head(const id_int value_id)
        // {
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        //     return _static_tree.insert(_static_tree.begin(), { value_id, value_t {} });
        // }

        // template <typename value_t>
        // typename id_tree<value_t>::iterator find_in_tree(const id_int value_id)
        // {
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        //     id_tree<value_t>::iterator _it = _static_tree.begin();
        //     while (_it != _static_tree.end()) {
        //         id_int _id = _it->first;
        //         if (_id == value_id)
        //             return _it;
        //         _it++;
        //     }
        //     return _static_tree.end();
        // }

        // template <typename value_t>
        // typename id_tree<value_t>::iterator find_in_tree_head(const id_int value_id)
        // {
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        //     id_tree<value_t>::sibling_iterator _sibling_it = _static_tree.begin();
        //     while (_sibling_it != _static_tree.end()) {
        //         id_int _id = _sibling_it->first;
        //         if (_id == value_id)
        //             return _sibling_it;
        //         _sibling_it++;
        //     }
        //     std::cout << "!!!\n";
        //     return prepare_in_tree_head<value_t>(value_id);
        //     // return _static_tree.begin();
        // }

        // // template <typename value_t>
        // // void assign_in_tree_head(const id_int value_id, value_t&& value)
        // // {
        // //     // print_tree<value_t>();
        // //     std::cout << "assign node " << value_id << " of type " << value.typeid_name << std::endl;
        // //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        // //     id_tree<value_t>::iterator _value_it = find_in_tree_head<value_t>(value_id);
        // //     _value_it->second = std::move(value);
        // // }

        // template <typename value_t>
        // void reparent_in_tree_head(const id_int parent_id, const id_int child_id)
        // {
        //     std::cout << "reparent " << child_id << " -> " << parent_id << std::endl;
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        //     id_tree<value_t>::iterator _parent_it = find_in_tree_head<value_t>(parent_id);
        //     id_tree<value_t>::iterator _child_it = find_in_tree_head<value_t>(child_id);
        //     auto k4 = _static_tree.append_child(_parent_it, { 999, value_t {} });
        //     _static_tree.move_ontop(k4, _child_it);
        // }

        // template <typename value_t>
        // void reparent_at_begin(const id_int child_id)
        // {
        //     id_tree<value_t>& _static_tree = detail::get_static_id_tree<value_t>();
        //     id_tree<value_t>::iterator _child_it = find_in_tree_head<value_t>(child_id);
        //     auto k4 = _static_tree.append_child(_static_tree.begin(), { 0, std::nullopt });
        //     _static_tree.move_ontop(k4, _child_it);
        // }
        // traverse !!

        // template <typename value_t>
        // void traverse_tree(std::function < void(std::optional<value_t> > callback))
        // {
        // }

        // void test()
        // {
        //     struct ok {
        //         int y = 4;
        //         int z = 4;
        //     };

        //     ok _ok;
        //     ok& _okr = _ok;
        //     std::any _t;
        //     _t = std::move(_ok);
        //     _okr.y++;
        //     std::cout << _okr.y << std::endl;

        //     // ok&& _ok2 = std::move(*_tr);
        //     // _okr.y++;
        //     // std::cout << _okr.y << std::endl;

        //     // _tr = _t.insert(_t.begin(), std::move(_ok));
        // }

    }
}
}