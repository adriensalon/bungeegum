//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <tree.hh>

namespace staticgui {
namespace glue {

    template <typename node_t, typename node_allocator_t = std::allocator<kbtree::tree_node_<node_t>>>
    struct tree {
        tree();
        tree(const node_t& head);
        tree(node_t&& head);
        tree(const tree& other);
        tree& operator=(const tree& other);
        tree(tree&& other);
        tree& operator=(tree&& other);
        ~tree();

        using pre_order_iterator = typename kbtree::tree<node_t, node_allocator_t>::pre_order_iterator;
        using post_order_iterator = typename kbtree::tree<node_t, node_allocator_t>::post_order_iterator;
        using sibling_iterator = typename kbtree::tree<node_t, node_allocator_t>::sibling_iterator;
        using leaf_iterator = typename kbtree::tree<node_t, node_allocator_t>::leaf_iterator;

    private:
        kbtree::tree<node_t, node_allocator_t> _tree;
    };

    template <typename node_t>
    struct tree<std::reference_wrapper<node_t>> {
        tree();
        tree(const std::reference_wrapper<node_t>& head);
        tree(std::reference_wrapper<node_t>&& head);
        tree(const tree& other);
        tree& operator=(const tree& other);
        tree(tree&& other);
        tree& operator=(tree&& other);
        ~tree();

        using pre_order_iterator = typename kbtree::tree<node_t*>::pre_order_iterator;
        using post_order_iterator = typename kbtree::tree<node_t*>::post_order_iterator;
        using sibling_iterator = typename kbtree::tree<node_t*>::sibling_iterator;
        using leaf_iterator = typename kbtree::tree<node_t*>::leaf_iterator;

    private:
        kbtree::tree<node_t*> _tree;
    };

}
}

#include <staticgui/glue/tree.inl>