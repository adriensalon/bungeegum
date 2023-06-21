#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

using namespace bungeegum::widgets;

struct delegateTest {

    BoxConstraints getConstraintsForChild(const BoxConstraints constraints)
    {
    }

    Offset getPositionForChild(const Size size, const Size childSize)
    {
    }

    Size getSize(const BoxConstraints constraints)
    {
    }

    bool shouldRelayout(const delegateTest& oldDelegate)
    {
    }
};

int main()
{

    SingleChildLayoutDelegate<delegateTest> ii;

    bungeegum::launch(bungeegum::make<Title>()
                          .title("my title !!!")
                          .child(
                              bungeegum::make<Align>()
                                  .alignment(Alignment::center())

                                  //   .heightFactor(2.f) // LOL NE MARCHE PAS
                                  //   .widthFactor(2.f) // LOL NE MARCHE PAS
                                  .child(bungeegum::make<SizedBox>()
                                             //  .constraints(BoxConstraints::tight(Size(500.f, 500.f)))
                                             .height(100.f)
                                             .width(100.f)
                                             //  .expand()
                                             .child(bungeegum::make<Padding>()
                                                        .padding(EdgeInsets::fromLTRB(5.f, 10.f, 15.f, 5.f))
                                                        .child(bungeegum::make<ColoredBox>()
                                                                   .color(0xFF6699FF)))))

        //   )
        ,
        [&]() {
            // bungeegum::must_resolve();
            // bungeegum::must_draw();
            // rendre officiel j'imagine
        });

    return 0;
}