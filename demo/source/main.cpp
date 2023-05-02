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
    // bungeegum::event<int>& evvv1 = bungeegum::make_event<int>();
    // bungeegum::event<int>& evvv2 = bungeegum::make_event<int>(evvv1); // not implemented
    // bungeegum::event<int>& evvv3 = bungeegum::make_event<int>(std::move(evvv1));

    std::cout << reinterpret_cast<std::uintptr_t>(&(okokoko())) << std::endl;
    bungeegum::widgets::container c1, c2;

    auto& c3 = bungeegum::make<bungeegum::widgets::container>();
    auto& c4 = bungeegum::make<bungeegum::widgets::container>();

    bungeegum::iterate<bungeegum::widgets::container>([](bungeegum::widgets::container& cont) {
        std::cout << "container ! \n";
    });

    bungeegum::launch(c3.ok().child(
        c4.ok().child(c2).ok()));

    return 0;
}