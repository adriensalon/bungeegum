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
    bungeegum::widgets::container_widget c1, c2;

    bungeegum::launch(bungeegum::create_widget<bungeegum::widgets::container_widget>().ok().child(
        bungeegum::create_widget<bungeegum::widgets::container_widget>().ok().child(c2).ok()));

    return 0;
}