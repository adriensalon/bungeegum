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

    HOTRELOAD_CLASS(Composed)

    HOTRELOAD_METHOD Composed& build()
    {
        bungeegum::adopt(this, _box.get());
        bungeegum::reference_widget<ColoredBox> _box3 = std::move(_box2);
        _box2 = std::move(_box3);
        bungeegum::adopt(_box.get(), _box2.get());
        return *this;
    }

    // Composed()
    // {
    //     bungeegum::adopt(this, _box.get().color(0xFF8899AA));
    //     bungeegum::adopt(_box, _box2.get().color(0xFF8899AA));
    // }

    HOTRELOAD_METHOD void draw(bungeegum::draw_command& command)
    {
        // bungeegum::float2 _min_point = bungeegum::float2 { 500.f, 500.f };
        // bungeegum::float2 _max_point = _min_point + bungeegum::float2 { 500.f, 500.f };
        // command.draw_rect_filled(_min_point, _max_point, { 1.f, 0.5f, 0.5f, 1.f });

        _box.get().color(0xFF8899FF);
        _box2.get().color(0xFF8899AA);

        // std::cout << "color box  = " << 44.f << std::endl;
    }

    // ColoredBox _box;
    // ColoredBox _box2;
    bungeegum::reference_widget<ColoredBox> _box = bungeegum::make_reference<ColoredBox>(56);
    bungeegum::reference_widget<ColoredBox> _box2 = bungeegum::make_reference<ColoredBox>(44);
};

int main()
{
    bungeegum::detail::indexed_map<unsigned int, float> _map = {
        { 20, 33.f },
        { 21, 33.f },
        { 19, 33.f }
    };
    _map.emplace(55, 666.f);
    _map.emplace(44, 333.f);
    _map.emplace(22, 111.f);
    _map[22] += 11.f;
    for (auto& m : _map) {
        std::cout << m << std::endl;
    }
    _map.erase(55);

    std::wostringstream _osstream2;

    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/glm/");
    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/entt/src");
    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/cereal/include/");
    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/include");
    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/extensions/mem/include");
    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/bungeegum/include/");
    bungeegum::hotreload_libraries().push_back("C:/Users/adri/dev/bungeegum/build/bungeegum/Debug/bungeegum.lib");
    bungeegum::hotreload_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/Debug/hscpp.lib");
    bungeegum::hotreload_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/extensions/mem/Debug/hscpp-mem.lib");
    bungeegum::hotreload_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/imgui/Debug/imgui.lib");

    bungeegum::hotreload_include_directories().push_back("C:/Users/adri/dev/bungeegum/bungeegum_widgets/include/");
    bungeegum::hotreload_source_directories().push_back("C:/Users/adri/dev/bungeegum/bungeegum_widgets/source/widgets/");

    bungeegum::hotreload_force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/bungeegum_widgets/source/core/Color.cpp");
    bungeegum::hotreload_force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/bungeegum_widgets/source/widgets/ColoredBox.cpp");

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
                                                                               .child(bungeegum::make<Composed>()
                                                                                          .build()
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
        ,
        [&]() {
            // bungeegum::must_resolve();
            // bungeegum::must_draw();
            // rendre officiel j'imagine
        });

    return 0;
}