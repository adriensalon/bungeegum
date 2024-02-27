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

    Composed& build()
    {
        bungeegum::adopt(this, _box.get());
        bungeegum::widget_ref<ColoredBox> _box3 = std::move(_box2);
        _box2 = std::move(_box3);
        bungeegum::adopt(_box.get(), _box2.get());
        return *this;
    }

    // Composed()
    // {
    //     bungeegum::adopt(this, _box.get().color(0xFF8899AA));
    //     bungeegum::adopt(_box, _box2.get().color(0xFF8899AA));
    // }

    void draw(bungeegum::draw_command& command)
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
    bungeegum::widget_ref<ColoredBox> _box = bungeegum::make_reference<ColoredBox>();
    bungeegum::widget_ref<ColoredBox> _box2 = bungeegum::make_reference<ColoredBox>();
};


struct drawerwidget {

    void draw(bungeegum::draw_command& command)
    {
        bungeegum::float2 _min_point = bungeegum::float2 { 500.f, 500.f };
        bungeegum::float2 _max_point_1 = _min_point + bungeegum::float2 { 100.f, 100.f };
        bungeegum::float2 _max_point_2 = _min_point + bungeegum::float2 { 500.f, 500.f };
        command.use_shader_mask();
        command.draw_rect_filled(_min_point - bungeegum::float2 {10.f, 10.f }, _max_point_1, { 1.f, 1.f, 0.5f, 1.f });
        command.draw_rect_filled(_min_point + bungeegum::float2 {10.f, 10.f }, _max_point_1 + bungeegum::float2 {30.f, 30.f }, { 1.f, 1.f, 0.5f, 1.f });
        
        command.use_shader_default();
        command.draw_rect_filled(_min_point, _max_point_2, { 1.f, 0.5f, 0.5f, 1.f });
    }

};

int main()
{
    // bungeegum::detail::chronometer<30u> _chrono;

    // #if BUNGEEGUM_USE_HOTSWAP
    //     bungeegum::detail::setup_global();
    // #endif
    // constexpr auto ii = bungeegum::is_finite<bungeegum::float2>({ 44.f, 0.f });
    // static std::wostringstream _osstream2;

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

    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/glm/");
    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/entt/src");
    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/cereal/include/");
    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/include");
    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/external/hscpp/extensions/mem/include");
    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/runtime/include/");

    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/runtime/Release/bungeegum_runtime.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/widgets/Release/bungeegum_widgets.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/Release/hscpp.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/extensions/mem/Release/hscpp-mem.lib");
    // bungeegum::libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/imgui/Release/imgui.lib");

    // bungeegum::hotswap::get_libraries().push_back("C:/Users/adri/dev/bungeegum/build/runtime/Debug/bungeegum_runtime.lib");
    // bungeegum::hotswap::get_libraries().push_back("C:/Users/adri/dev/bungeegum/build/widgets/Debug/bungeegum_widgets.lib");
    // bungeegum::hotswap::get_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/Debug/hscpp.lib");
    // bungeegum::hotswap::get_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/hscpp/extensions/mem/Debug/hscpp-mem.lib");
    // bungeegum::hotswap::get_libraries().push_back("C:/Users/adri/dev/bungeegum/build/external/imgui/Debug/imgui.lib");

    // bungeegum::hotswap::get_include_directories().push_back("C:/Users/adri/dev/bungeegum/widgets/include/");
    // bungeegum::hotswap::get_source_directories().push_back("C:/Users/adri/dev/bungeegum/widgets/source/widgets/");
    // bungeegum::hotswap::get_force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/widgets/source/core/Color.cpp");
    // bungeegum::hotswap::get_force_compiled_source_files().push_back("C:/Users/adri/dev/bungeegum/widgets/source/widgets/ColoredBox.cpp");

    bungeegum::pipeline_provider _provider;
    _provider.emscripten_canvas_id = "canvas";
    _provider.native_window_ptr = nullptr;

    bungeegum::pipeline _my_pipeline;
    _my_pipeline.setup<bungeegum::renderer_backend::opengl>(_provider);
    // _my_pipeline.setup<bungeegum::renderer_backend::directx11>(_provider);

    bungeegum::texture_resource _texture_res;
    _texture_res.pixels({ 0u });
    _texture_res.size(1, 2);
    bungeegum::texture_ref _tex = bungeegum::make_texture("my tex 1", _texture_res);

    bungeegum::shader_resource _shader1_res;
    _shader1_res.fragment(R"(
		struct PSInput
		{
			float4 pos : SV_POSITION;
			float4 col : COLOR;
			float2 uv  : TEXCOORD;
		};

		Texture2D    Texture;
		SamplerState Texture_sampler;

		float4 main(in PSInput PSIn) : SV_Target
		{
			return PSIn.col * Texture.Sample(Texture_sampler, PSIn.uv);
		}
		)",
        {});
    _shader1_res.depth({});
    _shader1_res.stencil({});
    bungeegum::make_shader("my shader 1", _shader1_res);

    // bungeegum::font_resource _font1_res;
    // _font1_res.compressed(nullptr, 0);
    // _font1_res.size(13.4f);

    bungeegum::shader_ref _shader1 = bungeegum::get_shader("my shader 1");
    _shader1.uniform<float>("okok", 44.f);

    _my_pipeline.root(bungeegum::make<drawerwidget>());
    _my_pipeline.run(60, true);

    // p.root(bungeegum::widget_id( bungeegum::make<Title>()
    //            .title("my title !!!")
    //            .child(bungeegum::make<Align>()

    //                       .alignment(Alignment::center())
    //                       .heightFactor(2.f) // LOL NE MARCHE PAS
    //                       //   .widthFactor(2.f) // LOL NE MARCHE PAS
    //                       .child(bungeegum::make<CustomSingleChildLayout>()
    //                                  .delegate<delegateTest>()

    //                                  //  .constraints(BoxConstraints::tight(Size(500.f, 500.f)))
    //                                  //  .height(100.f)
    //                                  //  .width(100.f)
    //                                  //  .expand()
    //                                  .child(bungeegum::make<FutureBuilder<bool>>()
    //                                             .initialData(false)
    //                                             .future(std::async([]() {
    //                                                 std::this_thread::sleep_for(std::chrono::milliseconds(4000));
    //                                                 return true;
    //                                             }))
    //                                             .builder([](const bool value) -> bungeegum::widget_id {
    //                                                 if (value) {
    //                                                     static bungeegum::animation<float> _myanim;
    //                                                     _myanim.shape(bungeegum::curve::bounce_in());
    //                                                     _myanim.duration<std::chrono::seconds>(10).min(0.f).max(88.f).start();
    //                                                     static auto fff = std::async([]() {
    //                                                         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //                                                         _osstream2 << "Helloooooo4466" << std::endl;
    //                                                         bungeegum::set_contextual<float>("myfloat", 2.f);
    //                                                     });
    //                                                     (void)fff;
    //                                                     return bungeegum::make<WideStreamBuilder>()
    //                                                         .initialData(L"heyyy")
    //                                                         .builder([](const std::wstring& message) -> bungeegum::widget_id {
    //                                                             //    std::wcout << message;

    //                                                             return bungeegum::make<OverflowBox>()
    //                                                                 .maxHeight(40.f)
    //                                                                 .minHeight(10.f)
    //                                                                 .maxWidth(500.f)
    //                                                                 .minWidth(500.f)
    //                                                                 .child(bungeegum::make<ColoredBox>()
    //                                                                            .color(0xFF6611FF)
    //                                                                     //   .okok()
    //                                                                 );
    //                                                         })
    //                                                         .stream(_osstream2);
    //                                                 }
    //                                                 return bungeegum::make<Padding>()
    //                                                     .padding(EdgeInsets::fromLTRB(44.f, 10.f, 15.f, 5.f))
    //                                                     .child(bungeegum::make<OverflowBox>()
    //                                                                .maxHeight(400.f)
    //                                                                .minHeight(100.f)
    //                                                                .maxWidth(50.f)
    //                                                                .minWidth(50.f)
    //                                                                .child(bungeegum::make<ColoredBox>()
    //                                                                           .color(0xFF6611FF)
    //                                                                    // .child(bungeegum::make<CustomMultiChildLayout<std::string>>()
    //                                                                    //            .delegate<multiDelegateTest>()
    //                                                                    //            .children({
    //                                                                    //                bungeegum::make<LayoutId<std::string>>()
    //                                                                    //                    .id("1st")
    //                                                                    //                    .child(bungeegum::make<ColoredBox>()
    //                                                                    //                               .color(0xFF8899FF)),
    //                                                                    //                bungeegum::make<LayoutId<std::string>>()
    //                                                                    //                    .id("2nd")
    //                                                                    //                    .child(bungeegum::make<ColoredBox>()
    //                                                                    //                               .color(0xFF8899FF)),
    //                                                                    //                bungeegum::make<LayoutId<std::string>>()
    //                                                                    //                    .id("3rd")
    //                                                                    //                    .child(bungeegum::make_and_get<ColoredBox>()
    //                                                                    //                               .color(0xFF8899FF)),
    //                                                                    //            }))

    //                                                                    )

    //                                                     );
    //                                             }))

    //                               ))

    //     //   )
    // ));

    return 0;
}