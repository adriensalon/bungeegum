#include <bungeegum/bungeegum.hpp>

struct A {

    A()
    {

        std::cout << reinterpret_cast<std::uintptr_t>(this) << std::endl;
    }
};

struct relou {
    int f = 44;
};

relou& okokoko()
{
    static relou _rel;

    std::cout << reinterpret_cast<std::uintptr_t>(&_rel) << std::endl;
    return _rel;
}

int main()
{
    std::cout << reinterpret_cast<std::uintptr_t>(&(okokoko())) << std::endl;
    bungeegum::widgets::container c1, c2;

    auto& c3 = bungeegum::make<bungeegum::widgets::container>();
    auto& c4 = bungeegum::make<bungeegum::widgets::container>();

    bungeegum::launch(c3.ok().child(
        c4.ok().child(c2).ok()));

    return 0;
}