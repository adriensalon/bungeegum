#pragma once

#include <bungeegum/core/resolve.fwd>

namespace bungeegum {

/// @brief
struct resolve_command {

    /// @brief
    [[nodiscard]] float2 get_min_size() const;

    /// @brief
    [[nodiscard]] float2 get_max_size() const;

    /// @brief
    /// @param size
    void resize(const float2 size);

    /// @brief
    /// @param child_widget
    /// @param constraint
    float2 resolve_child(const widget_id& child_widget, const float2 min_size, const float2 max_size) const;

    /// @brief 
    /// @param min_size 
    /// @param max_size 
    /// @return 
    std::vector<float2> resolve_children(const float2 min_size, const float2 max_size);

    /// @brief
    /// @param child_widget
    /// @param position
    void position_child(const widget_id& child_widget, const float2 position);

    /// @brief 
    /// @param position 
    void position_children(const float2 position);

private:
    detail::resolve_command_data _data;
    friend struct detail::process_manager;
#if BUNGEEGUM_USE_OVERLAY
    friend struct detail::pipelines_manager; // to set clean typename
    friend struct detail::widget_inspector;
#endif
};

/// @brief
/// @param widget
void must_resolve(const widget_id& id);
}
