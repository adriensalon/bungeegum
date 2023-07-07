#pragma once

namespace bungeegum {

template <typename property_t>
property_t& make_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
bool has_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
property_t& get_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t>
property_t& get_or_make_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}

template <typename property_t, template <typename, typename> typename container_t, typename allocator_t>
void get_children_with_property(
    const runtime_widget& widget,
    const std::string& name,
    container_t<runtime_widget, allocator_t>& container,
    const bool recursive)
{
    (void)widget;
    (void)name;
    (void)container;
    (void)recursive;
}

template <typename property_t>
bool has_parent_with_property(const runtime_widget& widget, const std::string& name,
    const unsigned int depth)
{
    (void)depth;
}

template <typename property_t>
runtime_widget get_parent_with_property(
    const runtime_widget& widget,
    const std::string& name,
    const unsigned int depth)
{
    (void)widget;
    (void)name;
    (void)depth;
}

template <typename property_t>
void destroy_property(const runtime_widget& widget, const std::string& name)
{
    (void)widget;
    (void)name;
}
}