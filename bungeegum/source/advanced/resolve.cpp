#include <imgui.h>

#include <bungeegum/advanced/resolve.hpp>

namespace bungeegum {

resolve_constraint::resolve_constraint(const float2 min_size, const float2 max_size)
{
    (void)min_size;
    (void)max_size;
}

// resolve_constraint::resolve_constraint(const resolve_constraint& other)
// {
//     (void)other;
// }

// resolve_constraint& resolve_constraint::operator=(const resolve_constraint& other)
// {
//     (void)other;
//     return *this;
// }

// resolve_constraint::resolve_constraint(resolve_constraint&& other)
// {
//     (void)other;
// }

// resolve_constraint& resolve_constraint::operator=(resolve_constraint&& other)
// {
//     (void)other;
//     return *this;
// }

float2& resolve_constraint::min_size()
{
    return _data.min_size;
}

const float2 resolve_constraint::min_size() const
{
    return _data.min_size;
}

float2& resolve_constraint::max_size()
{
    return _data.max_size;
}

const float2 resolve_constraint::max_size() const
{
    return _data.max_size;
}

resolve_constraint resolve_constraint::flip()
{
    resolve_constraint _retval;

    return resolve_constraint(
        { min_size().y, min_size().x },
        { max_size().y, max_size().x });
}

resolve_constraint resolve_constraint::normalize()
{
    float _min_width = std::fmax(min_size().x, 0.f);
    float _min_height = std::fmax(min_size().y, 0.f);
    float _max_width = std::fmax(_min_width, max_size().x);
    float _max_height = std::fmax(_min_height, max_size().y);
    return resolve_constraint(
        { _min_width, _min_height },
        { _max_width, _max_height });
}

// resolve_constraint resolve_constraint::enforce(const resolve_constraint& constraint) {
//     minWidth : clampDouble(minWidth, constraints.minWidth, constraints.maxWidth),
//     maxWidth : clampDouble(maxWidth, constraints.minWidth, constraints.maxWidth),
//     minHeight : clampDouble(minHeight, constraints.minHeight, constraints.maxHeight),
//     maxHeight : clampDouble(maxHeight, constraints.minHeight, constraints.maxHeight),
// }

// resolve_constraint resolve_constraint::deflate(const float2& insets)
// {
// }

resolve_constraint resolve_constraint::loosen()
{
    return resolve_constraint(
        { 0.f, 0.f },
        max_size());
}

// resolve_constraint resolve_constraint::tighten(const float2& size)
// {
// }

bool resolve_constraint::has_bounded_height() const
{
    return (max_size().y < infinity<float>);
}

bool resolve_constraint::has_bounded_width() const
{
    return (max_size().x < infinity<float>);
}

bool resolve_constraint::has_infinite_height() const
{
    return (min_size().y >= infinity<float>);
}

bool resolve_constraint::has_infinite_width() const
{
    return (min_size().x >= infinity<float>);
}

bool resolve_constraint::has_tight_height() const
{
    return (min_size().y >= max_size().y);
}

bool resolve_constraint::has_tight_width() const
{
    return (min_size().x >= max_size().x);
}

bool resolve_constraint::is_normalized() const
{
    bool _width_normalized = min_size().x >= 0.f && min_size().x <= max_size().x;
    bool _height_normalized = min_size().y >= 0.f && min_size().y <= max_size().y;
    return (_width_normalized && _height_normalized);
}

// float2 resolve_constraint::constrain(const float2& size) const
// {
// }

// float2 resolve_constraint::biggest() const
// {
//     return constrain({ infinity<float>, infinity<float> });
// }

// float2 resolve_constraint::smallest() const
// {
//     return constrain({ 0.f, 0.f });
// }

}