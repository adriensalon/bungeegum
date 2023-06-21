#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

using namespace bungeegum::widgets;

struct delegateTest {

    BoxConstraints getConstraintsForChild(const BoxConstraints constraints)
    {
        Size _wantedSize(200.f, 200.f);
        Size _constrainedSize = constraints.constrain(_wantedSize);
        return BoxConstraints(_constrainedSize, _constrainedSize);
    }

    Offset getPositionForChild(const Size size, const Size childSize)
    {
        return Offset(0.f, 0.f);
    }

    Size getSize(const BoxConstraints constraints)
    {
        Size _wantedSize(200.f, 200.f);
        Size _constrainedSize = constraints.constrain(_wantedSize);
        return _constrainedSize;
    }
};

int main()
{
    // FutureBuilder<Color> _fb;
    // _fb.initialData(55.f);
    // _fb.builder<Padding>([](const float ff) -> auto& {
    //     return bungeegum::make<Padding>()
    //         .padding(EdgeInsets::fromLTRB(5.f, 10.f, 15.f, 5.f))
    //         .child(bungeegum::make<ColoredBox>()
    //                    .color(0xFF6699FF));
    // });
    // _fb.builder<Padding>(bbb);
    // _fb.future(std::async([]() {
    //     return 42.f;
    // }));

    bungeegum::launch(bungeegum::make<Title>()
                          .title("my title !!!")
                          .child(
                              bungeegum::make<Align>()
                                  .alignment(Alignment::center())

                                  //   .heightFactor(2.f) // LOL NE MARCHE PAS
                                  //   .widthFactor(2.f) // LOL NE MARCHE PAS
                                  .child(bungeegum::make<CustomSingleChildLayout>()
                                             .delegate<delegateTest>()

                                             //  .constraints(BoxConstraints::tight(Size(500.f, 500.f)))
                                             //  .height(100.f)
                                             //  .width(100.f)
                                             //  .expand()
                                             .child(bungeegum::make<FutureBuilder<Color>>()
                                                        .initialData(Color(0xFF6699FF))
                                                        .future(std::async([]() {
                                                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                                            return Color(0xFF6611FF);
                                                        }))
                                                        .builder<Padding>([](const Color& color) -> auto& {
                                                            return bungeegum::make<Padding>()
                                                                .padding(EdgeInsets::fromLTRB(5.f, 10.f, 15.f, 5.f))
                                                                .child(bungeegum::make<ColoredBox>()
                                                                           .color(color));
                                                        }))

                                          ))

        //   )
        ,
        [&]() {
            // bungeegum::must_resolve();
            // bungeegum::must_draw();
            // rendre officiel j'imagine
        });

    return 0;
}