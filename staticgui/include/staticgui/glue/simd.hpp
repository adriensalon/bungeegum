//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

#include <array>
#include <initializer_list>
#include <vector>

namespace staticgui {
namespace glue {

    namespace intrinsics {

        template <typename scalar_t, unsigned int count_t>
        void add(const scalar_t* first, const scalar_t* second, scalar_t* result);
    }

    enum struct simd_order {
        row_major,
        column_major
    };

    template <typename scalar_t, unsigned int count_t>
    struct simd_array {
        simd_array();
        simd_array(const std::array<scalar_t, count_t>& array);
        simd_array(const std::vector<scalar_t>& vector);
        simd_array(std::initializer_list<scalar_t> initializer_list);

        scalar_t& at(const unsigned int index);

        const scalar_t& at(const unsigned int index) const;

        scalar_t& operator[](const unsigned int index);

        const scalar_t& operator[](const unsigned int index) const;

        scalar_t* data();

        const scalar_t* data() const;

        scalar_t* operator&();

        const scalar_t* operator&() const;

    private:
        // alignas(128) std::array<scalar_t, count_t> _array;
        alignas(128) scalar_t _array[count_t];
    };

    template <typename scalar_t, unsigned int count_t, simd_order order_t>
    struct simd_ordered_array {
        simd_ordered_array();

        scalar_t& at(const unsigned int index);

    private:
        simd_ordered_array<scalar_t, count_t, order_t> _array;
    };

    using int2 = simd_array<int, 2>;
    using int3 = simd_array<int, 3>;
    using int4 = simd_array<int, 4>;
    using int3x3 = simd_ordered_array<int, 9, simd_order::row_major>;
    using int4x4 = simd_ordered_array<int, 16, simd_order::row_major>;
    using float2 = simd_array<float, 2>;
    using float3 = simd_array<float, 3>;
    using float4 = simd_array<float, 4>;
    using float3x3 = simd_ordered_array<float, 9, simd_order::row_major>;
    using float4x4 = simd_ordered_array<float, 16, simd_order::row_major>;
    using double2 = simd_array<double, 2>;
    using double3 = simd_array<double, 3>;
    using double4 = simd_array<double, 4>;
    using double3x3 = simd_ordered_array<double, 9, simd_order::row_major>;
    using double4x4 = simd_ordered_array<double, 16, simd_order::row_major>;

    template <typename scalar_t, unsigned int count_t>
    simd_array<scalar_t, count_t> operator+(const simd_array<scalar_t, count_t>& first, const simd_array<scalar_t, count_t>& second);

    template <typename scalar_t, unsigned int count_t, simd_order order_t>
    simd_ordered_array<scalar_t, count_t, order_t> operator+(const simd_ordered_array<scalar_t, count_t, order_t>& first, const simd_ordered_array<scalar_t, count_t, order_t>& second);
}
}
