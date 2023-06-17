#include <bungeegum/widgets/Title.hpp>

namespace bungeegum {
namespace widgets {

    Title& Title::color(const Color value)
    {
        bungeegum_app::color(value);
        return *this;
    }

    Title& Title::title(const std::string& value)
    {
        bungeegum_app::title(value);
        return *this;
    }

}
}