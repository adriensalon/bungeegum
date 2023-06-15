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

    {

        bungeegum::event<int>& evvv1 = bungeegum::make_event<int>();
        evvv1.trigger(std::async([]() {
            return 44;
        }));
        bungeegum::event<int> evvv2;
        bungeegum::event<int> evvv3(evvv2);
        bungeegum::event<int> evvv4;
        evvv2.trigger(std::async([]() {
            return 44;
        }));
        evvv4.merge(evvv3);
        // evvv1.merge(std::move(evvv2));
        bungeegum::unmake_event(evvv1);
    }
    {
        bungeegum::event<int> evvv2;
        evvv2.trigger(std::async([]() {
            return 44;
        }));
        static int kl;
        kl++;
    }
    // bungeegum::event<int>& evvv2 = bungeegum::make_event<int>(evvv1); // not implemented
    // bungeegum::event<int>& evvv3 = bungeegum::make_event<int>(std::move(evvv1));

    // std::cout << reinterpret_cast<std::uintptr_t>(&(okokoko())) << std::endl;
    // bungeegum::widgets::container c1, c2;
    // bungeegum::widgets::image i1;

    // auto& c3 = bungeegum::make<bungeegum::widgets::container>(); //.ok().ok();
    // auto& c4 = bungeegum::make<bungeegum::widgets::container>();

    // bungeegum::iterate<bungeegum::widgets::container>([](bungeegum::widgets::container& cont) {
    //     std::cout << "container ! \n";
    // });

    // bungeegum::launch(c1.child(c2).child(i1));

    return 0;
}