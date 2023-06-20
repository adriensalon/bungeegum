#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

int main()
{
    using namespace bungeegum::widgets;

    bungeegum::launch(bungeegum::make<Title>()
                          .title("my title !!!")
                          .child(
                              bungeegum::make<Align>()
                                  .alignment(Alignment::center())
                                  .child(bungeegum::make<SizedBox>()
                                             //  .constraints(BoxConstraints::tight(Size(500.f, 500.f)))
                                             .height(500.f)
                                             .width(500.f)
                                             //  .expand()
                                             .child(bungeegum::make<Padding>()
                                                        .padding(EdgeInsets::fromLTRB(5.f, 10.f, 15.f, 5.f))
                                                        .child(bungeegum::make<ColoredBox>()
                                                                   .color(0xFF6699FF)))))

        //   )
        ,
        [&]() {
            bungeegum::must_resolve();
            bungeegum::must_draw();
            // rendre officiel j'imagine
        });

    return 0;
}