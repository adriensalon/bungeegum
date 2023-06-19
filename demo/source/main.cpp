#include <bungeegum/bungeegum.hpp>
#include <bungeegum/bungeegum_widgets.hpp>

#include <sstream>

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

    using namespace bungeegum::widgets;

    Size sz1(0.f, 0.f);
    Offset off(1.f, 1.f);
    Offset off2(1.f, 1.f);
    // float kkk = off.direction();

    Offset offlerp = bungeegum::lerp(off, off2, 1.f);
    Offset zerooff = bungeegum::zero<Offset>;

    Title _title;
    Align _align;
    Container _container;
    ColoredBox _coloredBox;

    // Color col = Color::fromRGBO(255, 122, 100, 1.f);
    // std::cout << col.alpha() << ", " << col.red() << ", " << col.green() << ", " << col.blue() << std::endl;

    // bungeegum::launch(bungeegum::make<Title>()
    //                       .title("my title !!!")
    //                       .child(_coloredBox
    //                                  //  .color(Color::fromRGBO(255, 122, 121, 1.f)))
    //                                  .color(0xFF6699FF))
    bungeegum::launch(bungeegum::make<Title>()
                          .title("my title !!!")
                          .child(bungeegum::make<Padding>()
                                     .padding(EdgeInsets::fromLTRB(5.f, 10.f, 15.f, 200.f))
                                     .child(bungeegum::make<ColoredBox>()
                                                //  .color(Color::fromRGBO(255, 122, 121, 1.f)))
                                                .color(0xFF6699FF)))
        //   .child(_align
        //              .child(_container
        //                         .color(Color::fromRGBO(255, 122, 121, 1.f)))
        //              .alignment(Alignment::center())
        //              .heightFactor(0.5f)
        //              .widthFactor(0.5f))
        ,
        [&]() {
            bungeegum::must_resolve();
            bungeegum::must_draw();
            // rendre officiel j'imagine
        });

    return 0;
}