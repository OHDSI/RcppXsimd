/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_FP_SIGN_HPP
#define XSIMD_FP_SIGN_HPP

#include <type_traits>

#include "xsimd_numerical_constant.hpp"

namespace xsimd
{

    template <class T, std::size_t N>
    batch<T, N> bitofsign(const batch<T, N>& x);

    template <class T, std::size_t N>
    batch<T, N> copysign(const batch<T, N>& x1, const batch<T, N>& x2);

    template <class T, std::size_t N>
    batch<T, N> sign(const batch<T, N>& x);

    template <class T, std::size_t N>
    batch<T, N> signnz(const batch<T, N>& x);

    /**************************
     * fp_sign implementation *
     **************************/

    template <class T, std::size_t N>
    inline batch<T, N> bitofsign(const batch<T, N>& x)
    {
        using btype = batch<T, N>;
        return x & minuszero<btype>();
    }

    template <class T, std::size_t N>
    inline batch<T, N> copysign(const batch<T, N>& x1, const batch<T, N>& x2)
    {
        return abs(x1) | bitofsign(x2);
    }

    /***********************
     * sign implementation *
     ***********************/

    namespace detail
    {
        /* origin: boost/simd/arch/common/simd/function/sign.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         *
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */

        template <class B, bool cond = std::is_integral<typename B::value_type>::value>
        struct sign_impl
        {
            static inline B compute(const B& a)
            {
                return select(a > zero<B>(), B(1), zero<B>()) - select(a < zero<B>(), B(1), zero<B>());
            }
        };

        template <class B>
        struct sign_impl<B, false>
        {
            static inline B compute(const B& a)
            {
                B r = select(a > B(0.), B(1.), B(0.)) - select(a < B(0.), B(1.), B(0.));
#ifdef XSIMD_NO_NANS
                return r;
#else
                return select(isnan(a), nan<B>(), r);
#endif
            }
        };
    }

    template <class T, std::size_t N>
    inline batch<T, N> sign(const batch<T, N>& x)
    {
        return detail::sign_impl<batch<T, N>>::compute(x);
    }

    /*************************
     * signnz implementation *
     *************************/

    namespace detail
    {
        /* origin: boost/simd/arch/common/simd/function/signnz.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         *
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */
        template <class B, bool cond = std::is_floating_point<typename B::value_type>::value>
        struct signnz_impl
        {
            static inline B compute(const B& x)
            {
                using value_type = typename B::value_type;
                return (x >> (sizeof(value_type) * 8 - 1)) | B(1.);
            }
        };

        template <class B>
        struct signnz_impl<B, true>
        {
            static inline B compute(const B& x)
            {
#ifndef XSIMD_NO_NANS
                return select(isnan(x), nan<B>(), B(1.) | (signmask<B>() & x));
#else
                return B(1.) | (signmask<B>() & x);
#endif
            }
        };
    }

    template <class T, std::size_t N>
    inline batch<T, N> signnz(const batch<T, N>& x)
    {
        return detail::signnz_impl<batch<T, N>>::compute(x);
    }
}

#endif
