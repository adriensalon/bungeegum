//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <gtest/gtest.h>
#include <staticgui/glue/simd.hpp>

TEST(staticgui_glue, simd_intrinsics_add_float2)
{
    const std::array<float, 4> _v1({ 44, 10, 0, 0 });
    const std::array<float, 4> _v2({ 10, -5, 0, 0 });
    std::array<float, 4> _vr({ 0, 0, 0, 0 });
    staticgui::detail::intrinsics::add<float, 2>(_v1.data(), _v2.data(), _vr.data());
    bool _valid = (_vr[0] == 54 && _vr[1] == 5);
    EXPECT_TRUE(_valid);
}