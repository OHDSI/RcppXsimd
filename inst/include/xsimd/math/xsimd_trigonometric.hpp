/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_TRIGONOMETRIC_HPP
#define XSIMD_TRIGONOMETRIC_HPP

#include "xsimd_fp_sign.hpp"
#include "xsimd_invtrigo.hpp"
#include "xsimd_trigo_reduction.hpp"

namespace xsimd
{

    /**
     * Computes the sine of the batch \c x.
     * @param x batch of floating point values.
     * @return the sine of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> sin(const batch<T, N>& x);

    /**
     * Computes the cosine of the batch \c x.
     * @param x batch of floating point values.
     * @return the cosine of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> cos(const batch<T, N>& x);

    /**
     * Computes the sine and the cosine of the batch \c x. This method is faster
     * than calling sine and cosine independently.
     * @param x batch of floating point values.
     * @param si the sine of x.
     * @param co the cosine of x.
     */
    template <class T, std::size_t N>
    void sincos(const batch<T, N>& x, batch<T, N>& si, batch<T, N>& co);

    /**
     * Computes the tangent of the batch \c x.
     * @param x batch of floating point values.
     * @return the tangent of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> tan(const batch<T, N>& x);

    /**
     * Computes the arc sine of the batch \c x.
     * @param x batch of floating point values.
     * @return the arc sine of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> asin(const batch<T, N>& x);

    /**
     * Computes the arc cosine of the batch \c x.
     * @param x batch of floating point values.
     * @return the arc cosine of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> acos(const batch<T, N>& x);

    /**
     * Computes the arc tangent of the batch \c x.
     * @param x batch of floating point values.
     * @return the arc tangent of \c x.
     */
    template <class T, std::size_t N>
    batch<T, N> atan(const batch<T, N>& x);

    /**
     * Computes the arc tangent of the batch \c x/y, using the signs of the
     * arguments to determine the correct quadrant.
     * @param x batch of floating point values.
     * @param y batch of floating point values.
     * @return the arc tangent of \c x/y.
     */
    template <class T, std::size_t N>
    batch<T, N> atan2(const batch<T, N>& y, const batch<T, N>& x);

    namespace detail
    {
        /* origin: boost/simd/arch/common/detail/simd/trig_base.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         *
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */

        template <class B>
        struct trigo_kernel
        {
            template <class Tag = trigo_radian_tag>
            static inline B sin(const B& a, Tag = Tag())
            {
                const B x = abs(a);
                B xr = nan<B>();
                const B n = trigo_reducer<B, Tag>::reduce(x, xr);
                auto tmp = select(n >= B(2.), B(1.), B(0.));
                auto swap_bit = fma(B(-2.), tmp, n);
                auto sign_bit = bitofsign(a) ^ select(tmp != B(0.), signmask<B>(), B(0.));
                const B z = xr * xr;
                const B se = trigo_evaluation<B>::sin_eval(z, xr);
                const B ce = trigo_evaluation<B>::cos_eval(z);
                const B z1 = select(swap_bit == B(0.), se, ce);
                return z1 ^ sign_bit;
            }

            static inline B cos(const B& a)
            {
                const B x = abs(a);
                B xr = nan<B>();
                const B n = trigo_reducer<B>::reduce(x, xr);
                auto tmp = select(n >= B(2.), B(1.), B(0.));
                auto swap_bit = fma(B(-2.), tmp, n);
                auto sign_bit = select((swap_bit ^ tmp) != B(0.), signmask<B>(), B(0.));
                const B z = xr * xr;
                const B se = trigo_evaluation<B>::sin_eval(z, xr);
                const B ce = trigo_evaluation<B>::cos_eval(z);
                const B z1 = select(swap_bit != B(0.), se, ce);
                return z1 ^ sign_bit;
            }

            static inline B tan(const B& a)
            {
                const B x = abs(a);
                B xr = nan<B>();
                const B n = trigo_reducer<B>::reduce(x, xr);
                auto tmp = select(n >= B(2.), B(1.), B(0.));
                auto swap_bit = fma(B(-2.), tmp, n);
                auto test = (swap_bit == B(0.));
                const B y = trigo_evaluation<B>::tan_eval(xr, test);
                return y ^ bitofsign(a);
            }

            static inline void sincos(const B& a, B& si, B& co)
            {
                const B x = abs(a);
                B xr = nan<B>();
                const B n = trigo_reducer<B>::reduce(x, xr);
                auto tmp = select(n >= B(2.), B(1.), B(0.));
                auto swap_bit = fma(B(-2.), tmp, n);
                const B z = xr * xr;
                const B se = trigo_evaluation<B>::sin_eval(z, xr);
                const B ce = trigo_evaluation<B>::cos_eval(z);
                auto sin_sign_bit = bitofsign(a) ^ select(tmp != B(0.), signmask<B>(), B(0.));
                const B sin_z1 = select(swap_bit == B(0.), se, ce);
                si = sin_z1 ^ sin_sign_bit;
                auto cos_sign_bit = select((swap_bit ^ tmp) != B(0.), signmask<B>(), B(0.));
                const B cos_z1 = select(swap_bit != B(0.), se, ce);
                co = cos_z1 ^ cos_sign_bit;
            }
        };
    }

    template <class T, std::size_t N>
    inline batch<T, N> sin(const batch<T, N>& x)
    {
        return detail::trigo_kernel<batch<T, N>>::sin(x);
    }

    template <class T, std::size_t N>
    inline batch<T, N> cos(const batch<T, N>& x)
    {
        return detail::trigo_kernel<batch<T, N>>::cos(x);
    }

    template <class T, std::size_t N>
    inline void sincos(const batch<T, N>& x, batch<T, N>& si, batch<T, N>& co)
    {
        detail::trigo_kernel<batch<T, N>>::sincos(x, si, co);
    }

    template <class T, std::size_t N>
    inline batch<T, N> tan(const batch<T, N>& x)
    {
        return detail::trigo_kernel<batch<T, N>>::tan(x);
    }

    template <class T, std::size_t N>
    inline batch<T, N> asin(const batch<T, N>& x)
    {
        return detail::invtrigo_kernel<batch<T, N>>::asin(x);
    }

    template <class T, std::size_t N>
    inline batch<T, N> acos(const batch<T, N>& x)
    {
        return detail::invtrigo_kernel<batch<T, N>>::acos(x);
    }

    template <class T, std::size_t N>
    inline batch<T, N> atan(const batch<T, N>& x)
    {
        return detail::invtrigo_kernel<batch<T, N>>::atan(x);
    }

    template <class T, std::size_t N>
    inline batch<T, N> atan2(const batch<T, N>& y, const batch<T, N>& x)
    {
        return detail::invtrigo_kernel<batch<T, N>>::atan2(y, x);
    }
}

#endif
