#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

#include <iostream>

using namespace bungeegum::widgets;

struct delegateTest {

    delegateTest()
    {
        // bungeegum::runtime_property<float> _rp2(this, "okok", 45.f);
    }

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
    std::list<bungeegum::runtime_widget> _children;
    bungeegum::get_children_with_property<float>(bungeegum::make<Align>(), "okok", _children);

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
                                             .child(bungeegum::make<FutureBuilder<bool>>()
                                                        .initialData(false)
                                                        .future(std::async([]() {
                                                            std::this_thread::sleep_for(std::chrono::milliseconds(4000));
                                                            return true;
                                                        }))
                                                        .builder([](const bool value) -> bungeegum::runtime_widget {
                                                            auto& _coloredBox = bungeegum::make<ColoredBox>();
                                                            _coloredBox.color(value ? 0xFF6699FF : 0xFF6611FF);
                                                            if (value) {
                                                                return _coloredBox;
                                                            }
                                                            return bungeegum::make<Padding>()
                                                                .padding(EdgeInsets::fromLTRB(44.f, 10.f, 15.f, 5.f))
                                                                .child(_coloredBox);
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