//       _        _   _                 _
//      | |      | | (_)               (_)
//   ___| |_ __ _| |_ _  ___ __ _ _   _ _
//  / __| __/ _` | __| |/ __/ _` | | | | |
//  \__ \ || (_| | |_| | (_| (_| | |_| | |
//  |___/\__\__,_|\__|_|\___\__, |\__,_|_|
//                           __/ |
//                          |___/     v0.0

#pragma once

namespace staticgui {
namespace glue {

    // array

    template <typename scalar_t, unsigned int count_t>
    simd_array<scalar_t, count_t>::simd_array()
    {
        static_assert((count_t == 2) || (count_t == 3) || (count_t == 4), "Invalid scalar_t value. Possible values are 2, 3, 4. Any other value defeats the purpose of SIMD cpu instructions.");
    }

    template <typename scalar_t, unsigned int count_t>
    simd_array<scalar_t, count_t>::simd_array(const std::array<scalar_t, count_t>& array)
        : _array(array)
    {
        static_assert((count_t == 2) || (count_t == 3) || (count_t == 4), "Invalid scalar_t value. Possible values are 2, 3, 4. Any other value defeats the purpose of SIMD cpu instructions.");
    }

    template <typename scalar_t, unsigned int count_t>
    simd_array<scalar_t, count_t>::simd_array(const std::vector<scalar_t>& vector) // : _array(vector)
    {
        static_assert((count_t == 2) || (count_t == 3) || (count_t == 4), "Invalid scalar_t value. Possible values are 2, 3, 4. Any other value defeats the purpose of SIMD cpu instructions.");
    }

    // template <typename scalar_t, unsigned int count_t>
    // simd_array<scalar_t, count_t>::simd_array(std::initializer_list<scalar_t> initializer_list)
    // // : _array { initializer_list }
    // {
    //     static_assert((count_t == 2) || (count_t == 3) || (count_t == 4), "Invalid scalar_t value. Possible values are 2, 3, 4. Any other value defeats the purpose of SIMD cpu instructions.");
    //     _array = std::array<scalar_t, count_t>({ { initializer_list } });
    // }

    template <typename scalar_t, unsigned int count_t>
    scalar_t& simd_array<scalar_t, count_t>::at(const unsigned int index);

    template <typename scalar_t, unsigned int count_t>
    const scalar_t& simd_array<scalar_t, count_t>::at(const unsigned int index) const;

    template <typename scalar_t, unsigned int count_t>
    scalar_t& simd_array<scalar_t, count_t>::operator[](const unsigned int index);

    template <typename scalar_t, unsigned int count_t>
    const scalar_t& simd_array<scalar_t, count_t>::operator[](const unsigned int index) const;

    template <typename scalar_t, unsigned int count_t>
    scalar_t* simd_array<scalar_t, count_t>::data()
    {
        // return this->_array.data();
        return static_cast<scalar_t*>(this->_array.data());
    }

    template <typename scalar_t, unsigned int count_t>
    const scalar_t* simd_array<scalar_t, count_t>::data() const
    {
        // return this->_array.data();
        return static_cast<const scalar_t*>(this->_array.data());
    }

    template <typename scalar_t, unsigned int count_t>
    scalar_t* simd_array<scalar_t, count_t>::operator&()
    {
        return this->data();
    }

    template <typename scalar_t, unsigned int count_t>
    const scalar_t* simd_array<scalar_t, count_t>::operator&() const
    {
        return this->data();
    }

    // ordered_array

    template <typename scalar_t, unsigned int count_t, simd_order order_t>
    simd_ordered_array<scalar_t, count_t, order_t>::simd_ordered_array()
    {
        static_assert((count_t == 9) || (count_t == 16), "Invalid scalar_t value. Possible values are 9, 16. Any other value defeats the purpose of SIMD cpu instructions.");
    }

    // operators

    template <typename scalar_t, unsigned int count_t>
    simd_array<scalar_t, count_t> operator+(const simd_array<scalar_t, count_t>& first, const simd_array<scalar_t, count_t>& second)
    {
        simd_array<scalar_t, count_t> _retval;
        intrinsics::add<scalar_t, count_t>(&first, &second, &_retval);
        return _retval;
    }

}
}
