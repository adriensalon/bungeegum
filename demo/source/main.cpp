#include <bungeegum/bungeegum.hpp>
#include <bungeegum_widgets/bungeegum_widgets.hpp>

#include <iostream>
#include <sstream>

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

struct multiDelegateTest {

    Size getSize(const BoxConstraints constraints)
    {
        Size _wantedSize(200.f, 200.f);
        Size _constrainedSize = constraints.constrain(_wantedSize);
        return _constrainedSize;
    }

    void performLayout(const Size size, LayoutCommand<std::string>& command)
    {
        (void)size;
        (void)command;
    }
};

struct Composed {

    HOTSWAP_CLASS(Composed)

    HOTSWAP_METHOD Composed& build()
    {
        bungeegum::adopt(this, _box.get());
        bungeegum::widget_reference<ColoredBox> _box3 = std::move(_box2);
        _box2 = std::move(_box3);
        bungeegum::adopt(_box.get(), _box2.get());
        return *this;
    }

    // Composed()
    // {
    //     bungeegum::adopt(this, _box.get().color(0xFF8899AA));
    //     bungeegum::adopt(_box, _box2.get().color(0xFF8899AA));
    // }

    HOTSWAP_METHOD void draw(bungeegum::draw_command& command)
    {
        // bungeegum::float2 _min_point = bungeegum::float2 { 500.f, 500.f };
        // bungeegum::float2 _max_point = _min_point + bungeegum::float2 { 500.f, 500.f };
        // command.draw_rect_filled(_min_point, _max_point, { 1.f, 0.5f, 0.5f, 1.f });

        _box.get().color(0xFF8899FF);
        _box2.get().color(0xFF8899AA);

        // bungeegum::log_error("lolol");
        // volatile int* pInt = 0x00000000;
        // *pInt = 20;
        // throw "lolol";

        // float& okok = bungeegum::make_property<float>(this);
        // std::cout << "color box  = " << 44.f << std::endl;
    }

    // ColoredBox _box;
    // ColoredBox _box2;
    bungeegum::widget_reference<ColoredBox> _box = bungeegum::make_reference<ColoredBox>();
    bungeegum::widget_reference<ColoredBox> _box2 = bungeegum::make_reference<ColoredBox>();
};

int main()
{
    // bungeegum::detail::chronometer<30u> _chrono;

    // #if BUNGEEGUM_USE_HOTSWAP
    //     bungeegum::detail::setup_global();
    // #endif
    constexpr auto ii = bungeegum::is_finite<bungeegum::float2>({ 44.f, 0.f });
    static std::wostringstream _osstream2;

    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_ENABLE_DIRECTX");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_ENABLE_BACKTRACE");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_ENABLE_HOTRELOAD");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_ENABLE_STANDALONE");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_ENABLE_OVERLAY");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_USE_DIRECTX 1");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_USE_BACKTRACE 1");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_USE_HOTRELOAD 1");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_USE_STANDALONE 1");
    // bungeegum::hotreload_defines().push_back("BUNGEEGUM_USE_OVERLAY 1");

    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/external/glm/");
    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/external/entt/src");
    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/external/cereal/include/");
    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/include");
    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/extensions/mem/include");
    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/runtime/include/");

    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/runtime/Release/bungeegum_runtime.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/widgets/Release/bungeegum_widgets.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/Release/hscpp.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/extensions/mem/Release/hscpp-mem.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/imgui/Release/imgui.lib");

    bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/runtime/Debug/bungeegum_runtime.lib");
    bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/widgets/Debug/bungeegum_widgets.lib");
    bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/Debug/hscpp.lib");
    bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/extensions/mem/Debug/hscpp-mem.lib");
    bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/imgui/Debug/imgui.lib");

    bungeegum::include_directories().push_back("C:/Users/adri/dev/bungeegum/widgets/include/");
    bungeegum::source_directories().push_back("C:/Users/adri/dev/bungeegum/widgets/source/widgets/");
    bungeegum::force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/widgets/source/core/Color.cpp");
    bungeegum::force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/widgets/source/widgets/ColoredBox.cpp");

    bungeegum::launch(bungeegum::make<Title>()

                          .title("my title !!!")
                          .child(bungeegum::make<Align>()

                                     .alignment(Alignment::center())
                                     .heightFactor(2.f) // LOL NE MARCHE PAS
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
                                                               if (value) {
                                                                   static bungeegum::animation<float> _myanim;
                                                                   _myanim.duration<std::chrono::seconds>(10).min(0.f).max(88.f).start();
                                                                   static auto fff = std::async([]() {
                                                                       std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                                                       _osstream2 << "Helloooooo4466" << std::endl;
                                                                       bungeegum::set_contextual<float>("myfloat", 2.f);
                                                                   });
                                                                   (void)fff;
                                                                   return bungeegum::make<WideStreamBuilder>()
                                                                       .initialData(L"heyyy")
                                                                       .builder([](const std::wstring& message) -> bungeegum::runtime_widget {
                                                                           //    std::wcout << message;
																			
                                                                           return bungeegum::make<OverflowBox>()
                                                                               .maxHeight(40.f)
                                                                               .minHeight(10.f)
                                                                               .maxWidth(500.f)
                                                                               .minWidth(500.f)
                                                                               .child(bungeegum::make<ColoredBox>()
                                                                                          .color(0xFF6611FF)
                                                                                   //   .okok()
                                                                               );
                                                                       })
                                                                       .stream(_osstream2);
                                                               }
                                                               return bungeegum::make<Padding>()
                                                                   .padding(EdgeInsets::fromLTRB(44.f, 10.f, 15.f, 5.f))
                                                                   .child(bungeegum::make<OverflowBox>()
                                                                              .maxHeight(400.f)
                                                                              .minHeight(100.f)
                                                                              .maxWidth(50.f)
                                                                              .minWidth(50.f)
                                                                              .child(bungeegum::make<ColoredBox>()
                                                                                         .color(0xFF6611FF)
                                                                                  // .child(bungeegum::make<CustomMultiChildLayout<std::string>>()
                                                                                  //            .delegate<multiDelegateTest>()
                                                                                  //            .children({
                                                                                  //                bungeegum::make<LayoutId<std::string>>()
                                                                                  //                    .id("1st")
                                                                                  //                    .child(bungeegum::make<ColoredBox>()
                                                                                  //                               .color(0xFF8899FF)),
                                                                                  //                bungeegum::make<LayoutId<std::string>>()
                                                                                  //                    .id("2nd")
                                                                                  //                    .child(bungeegum::make<ColoredBox>()
                                                                                  //                               .color(0xFF8899FF)),
                                                                                  //                bungeegum::make<LayoutId<std::string>>()
                                                                                  //                    .id("3rd")
                                                                                  //                    .child(bungeegum::make_and_get<ColoredBox>()
                                                                                  //                               .color(0xFF8899FF)),
                                                                                  //            }))

                                                                                  )

                                                                   );
                                                           }))

                                             ))

        //   )
    );

    return 0;
}