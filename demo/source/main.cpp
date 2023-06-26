#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

#include <iostream>
#include <sstream>

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

/// TEST PR MultiChildLayoutDelegate OK !!!
/// TEST PR MultiChildLayoutDelegate OK !!!
/// TEST PR MultiChildLayoutDelegate OK !!!

template <typename value_t>
struct myStruct {

    void myMethod(value_t i)
    {
        std::cout << " okok" << std::endl;
    }

    value_t v;
    int i;
};

template myStruct<int>;

template <typename container_t, typename value_t>
using myMethodFunction = decltype(std::declval<container_t>().myMethod(std::declval<value_t>()));

template <template <typename> typename container_t, typename value_t>
constexpr inline bool hasMyMethodFunction = bungeegum::detail::is_detected_exact_v<void, myMethodFunction, container_t<value_t>, value_t>;

/// TEST PR MultiChildLayoutDelegate OK !!!
/// TEST PR MultiChildLayoutDelegate OK !!!
/// TEST PR MultiChildLayoutDelegate OK !!!

int main()
{
    /// TEST PR MultiChildLayoutDelegate OK !!!
    /// TEST PR MultiChildLayoutDelegate OK !!!
    /// TEST PR MultiChildLayoutDelegate OK !!!
    myStruct<int> kkkk;
    kkkk.i = 55;

    constexpr bool okokokokok = hasMyMethodFunction<myStruct, std::string>;
    std::cout << okokokokok << std::endl;

    /// TEST PR MultiChildLayoutDelegate OK !!!
    /// TEST PR MultiChildLayoutDelegate OK !!!
    /// TEST PR MultiChildLayoutDelegate OK !!!

    std::list<bungeegum::runtime_widget> _children;
    bungeegum::get_children_with_property<float>(bungeegum::make<Align>(), "okok", _children);

    // std::ostringstream _osstream;
    // bungeegum::widgets::detail::StreamBuilderBuffer _evb(_osstream, 512u);
    // _osstream.basic_ios<char>::rdbuf(&_evb);
    // _evb.flushCallback = [](const std::string& str) {
    //     std::cout << str;
    // };
    // _osstream << "Helloooooo4466" << std::endl;

    std::wostringstream _osstream2;

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
                                                        .builder([&_osstream2](const bool value) -> bungeegum::runtime_widget {
                                                            if (value) {
                                                                static auto fff = std::async([&_osstream2]() {
                                                                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                                                    _osstream2 << "Helloooooo4466" << std::endl;
                                                                });
                                                                (void)fff;
                                                                return bungeegum::make<WideStreamBuilder>()
                                                                    .initialData(L"heyyy")
                                                                    .builder([&_osstream2](const std::wstring& message) -> bungeegum::runtime_widget {
                                                                        std::wcout << message;

                                                                        return bungeegum::make<OverflowBox>()
                                                                            .maxHeight(40.f)
                                                                            .minHeight(10.f)
                                                                            .maxWidth(500.f)
                                                                            .minWidth(500.f)
                                                                            .child(
                                                                                bungeegum::make<ColoredBox>()
                                                                                    .color(0xFF8899FF));
                                                                    })
                                                                    .stream(_osstream2);
                                                            }
                                                            return bungeegum::make<Padding>()
                                                                .padding(EdgeInsets::fromLTRB(44.f, 10.f, 15.f, 5.f))
                                                                .child(
                                                                    bungeegum::make<OverflowBox>()
                                                                        .maxHeight(400.f)
                                                                        .minHeight(100.f)
                                                                        .maxWidth(50.f)
                                                                        .minWidth(50.f)
                                                                        .child(
                                                                            bungeegum::make<ColoredBox>()
                                                                                .color(0xFF6611FF))

                                                                );
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