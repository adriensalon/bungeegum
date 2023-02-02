//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#include <staticgui/glue/simd.hpp>
#include <staticgui/glue/toolchain.hpp>

#if TOOLCHAIN_PLATFORM_EMSCRIPTEN
#define SIMD_INTRINSICS_WEBASM 1
#define SIMD_INTRINSICS_NEON 0
#define SIMD_INTRINSICS_INTEL 0
#include <wasm_simd128.h>
using impl_int4 = v128_t;
using impl_float4 = v128_t;
using impl_double2 = v128_t;
#elif TOOLCHAIN_CPU_ARM && defined(NEON)
#define SIMD_INTRINSICS_WEBASM 0
#define SIMD_INTRINSICS_NEON 1
#define SIMD_INTRINSICS_INTEL 0
#elif TOOLCHAIN_CPU_X86
#define SIMD_INTRINSICS_WEBASM 0
#define SIMD_INTRINSICS_NEON 0
#define SIMD_INTRINSICS_INTEL 1
#include <vectorclass.h>
using impl_int4 = Vec4i;
using impl_float4 = Vec4f;
using impl_double2 = Vec2d;
using impl_int8 = Vec8i;
using impl_float8 = Vec8f;
using impl_double4 = Vec4d;
using impl_int16 = Vec16i;
using impl_float16 = Vec16f;
using impl_double8 = Vec8d;
#else // fallback
using impl_int4 = int;
using impl_float4 = int;
using impl_double2 = int;
#endif

namespace staticgui {
namespace glue {

    namespace detail {
        constexpr bool is_simd_intrinsics_webasm = SIMD_INTRINSICS_WEBASM;
        constexpr bool is_simd_intrinsics_neon = SIMD_INTRINSICS_NEON;
        constexpr bool is_simd_intrinsics_intel = SIMD_INTRINSICS_INTEL;

        // load

        // store
    }

    namespace intrinsics {

        template <>
        void add<int, 2>(const int* first, const int* second, int* result)
        {
            if constexpr (detail::is_simd_intrinsics_webasm) {

            } else if constexpr (detail::is_simd_intrinsics_neon) {

            } else if constexpr (detail::is_simd_intrinsics_intel) {

            } else // element wise
            {
            }
        }

        template <>
        void add<float, 2>(const float* first, const float* second, float* result)
        {
            // assert corrupted stack
            // dev assert sizeof(first)/sizeof(float) == 4
            // dev assert sizeof(second)/sizeof(float) == 4
            // dev assert sizeof(result)/sizeof(float) == 4
            impl_float4 _first, _second, _result;
#if SIMD_INTRINSICS_WEBASM
            _first = wasm_v128_load(first);
            _second = wasm_v128_load(second);
            _result = wasm_f32x4_add(_first, _second);
            wasm_v128_store(result, _result);
#elif SIMD_INTRINSICS_NEON
            // todo
#elif SIMD_INTRINSICS_INTEL
            _first.load(first);
            _second.load(second);
            _result = _first + _second;
            _result.store(result);
#else
            // todo
#endif
        }
    }

}
}