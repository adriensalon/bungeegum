//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <gtest/gtest.h>
#include <staticgui/glue/typelist.hpp>

TEST(staticgui_glue, typelist_as)
{
    auto _t1 = staticgui::glue::typelist<int>().as<std::vector>();
    auto _t2 = std::vector<int>();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_wrap)
{
    auto _t1 = staticgui::glue::typelist<int, float>().wrap<std::vector>();
    auto _t2 = staticgui::glue::typelist<std::vector<int>, std::vector<float>>();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_front)
{
    auto _t1 = staticgui::glue::typelist<int, float>().front();
    auto _t2 = int(5);
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_pop_front)
{
    auto _t1 = staticgui::glue::typelist<int>();
    auto _t2 = staticgui::glue::typelist<float, int>().pop_front();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_push_front)
{
    auto _t1 = staticgui::glue::typelist<int, float>();
    auto _t2 = staticgui::glue::typelist<float>().push_front<int>();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_at)
{
    auto _t1 = staticgui::glue::typelist<float, int, std::string>().at<2>();
    auto _t2 = std::string();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_back)
{
    auto _t1 = staticgui::glue::typelist<float, int>().back();
    auto _t2 = int(5);
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

// TEST(staticgui_glue, typelist_pop_back)
// {
//     auto _t1 = staticgui::glue::typelist<int>();
//     auto _t2 = staticgui::glue::typelist<int, float>().pop_back();
//     bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
//     EXPECT_TRUE(_same);
// }

TEST(staticgui_glue, typelist_push_back)
{
    auto _t1 = staticgui::glue::typelist<int, float>();
    auto _t2 = staticgui::glue::typelist<int>().push_back<float>();
    bool _same = std::is_same_v<decltype(_t1), decltype(_t2)>;
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_size)
{
    auto _t1 = staticgui::glue::typelist<int, float>().size();
    bool _same = (_t1 == 2);
    EXPECT_TRUE(_same);
}

TEST(staticgui_glue, typelist_empty)
{
    auto _t1 = staticgui::glue::typelist<int, float>().empty();
    auto _t2 = staticgui::glue::typelist<>().empty();
    bool _same = (_t1 != _t2);
    EXPECT_TRUE(_same);
}
