#pragma once

#include <bungeegum/bungeegum.hpp>

namespace bungeegum {
namespace widgets {

#if defined(BUNGEEGUM_WIDGETS_DEFAULT_LAYOUT_ID_TYPE)
    using LayoutIdDefaultType = BUNGEEGUM_WIDGETS_DEFAULT_LAYOUT_ID_TYPE;

#else
    // Change this to your prefered fallback type
    using LayoutIdDefaultType = std::string;
#endif

    template <typename id_t = LayoutIdDefaultType>
    struct LayoutId {

        LayoutId& child(const runtime_widget& value)
        {
            if (_childWidget.has_value())
                abandon(this, _childWidget.value());
            _childWidget = runtime_widget(value);
            adopt(this, _childWidget.value());
            return *this;
        }

        LayoutId& id(const id_t& value)
        {
            _idValue = value;
            return *this;
        }

        LayoutId& id(id_t&& value)
        {
            _idValue = std::move(value);
            return *this;
        }

    private:
        friend struct access;
        void resolve(resolve_command& command)
        {
            // TODO
        }

        std::optional<runtime_widget> _childWidget = std::nullopt;
        std::optional<id_t> _idValue = std::nullopt;
    };
}
}