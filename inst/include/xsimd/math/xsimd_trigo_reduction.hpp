/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_TRIGO_REDUCTION_HPP
#define XSIMD_TRIGO_REDUCTION_HPP

#include <array>
#include <limits>

#include "xsimd_horner.hpp"
#include "xsimd_rem_pio2.hpp"
#include "xsimd_rounding.hpp"


namespace xsimd
{

    template <class T, std::size_t N>
    batch<T, N> quadrant(const batch<T, N>& x);

    namespace detail
    {
        template <class B, class T = typename B::value_type>
        struct trigo_evaluation;

        /* origin: boost/simd/arch/common/detail/simd/f_trig_evaluation.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         *
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */
        template <class B>
        struct trigo_evaluation<B, float>
        {
            static inline B cos_eval(const B& z)
            {
                B y = horner<B,
                             0x3d2aaaa5,
                             0xbab60619,
                             0x37ccf5ce>(z);
                return B(1.) + fma(z, B(-0.5), y * z * z);
            }

            static inline B sin_eval(const B& z, const B& x)
            {
                B y = horner<B,
                             0xbe2aaaa2,
                             0x3c08839d,
                             0xb94ca1f9>(z);
                return fma(y * z, x, x);
            }

            static inline B base_tancot_eval(const B& z)
            {
                B zz = z * z;
                B y = horner<B,
                             0x3eaaaa6f,
                             0x3e0896dd,
                             0x3d5ac5c9,
                             0x3cc821b5,
                             0x3b4c779c,
                             0x3c19c53b>(zz);
                return fma(y, zz * z, z);
            }

            template <class BB>
            static inline B tan_eval(const B& z, const BB& test)
            {
                B y = base_tancot_eval(z);
                return select(test, y, -B(1.) / y);
            }

            template <class BB>
            static inline B cot_eval(const B& z, const BB& test)
            {
                B y = base_tancot_eval(z);
                return select(test, B(1.) / y, -y);
            }
        };

        /* origin: boost/simd/arch/common/detail/simd/d_trig_evaluation.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         * 
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */
        template <class B>
        struct trigo_evaluation<B, double>
        {
            static inline B cos_eval(const B& z)
            {
                B y = horner<B,
                             0x3fe0000000000000ll,
                             0xbfa5555555555551ll,
                             0x3f56c16c16c15d47ll,
                             0xbefa01a019ddbcd9ll,
                             0x3e927e4f8e06d9a5ll,
                             0xbe21eea7c1e514d4ll,
                             0x3da8ff831ad9b219ll>(z);
                return B(1.) - y * z;
            }

            static inline B sin_eval(const B& z, const B& x)
            {
                B y = horner<B,
                             0xbfc5555555555548ll,
                             0x3f8111111110f7d0ll,
                             0xbf2a01a019bfdf03ll,
                             0x3ec71de3567d4896ll,
                             0xbe5ae5e5a9291691ll,
                             0x3de5d8fd1fcf0ec1ll>(z);
                return fma(y * z, x, x);
            }

            static inline B base_tancot_eval(const B& z)
            {
                B zz = z * z;
                B num = horner<B,
                               0xc1711fead3299176ll,
                               0x413199eca5fc9dddll,
                               0xc0c992d8d24f3f38ll>(zz);
                B den = horner1<B,
                                0xc189afe03cbe5a31ll,
                                0x4177d98fc2ead8efll,
                                0xc13427bc582abc96ll,
                                0x40cab8a5eeb36572ll>(zz);
                return fma(z, (zz * (num / den)), z);
            }

            template <class BB>
            static inline B tan_eval(const B& z, const BB& test)
            {
                B y = base_tancot_eval(z);
                return select(test, y, -B(1.) / y);
            }

            template <class BB>
            static inline B cot_eval(const B& z, const BB& test)
            {
                B y = base_tancot_eval(z);
                return select(test, B(1.) / y, -y);
            }
        };

        /* origin: boost/simd/arch/common/detail/simd/trig_reduction.hpp */
        /*
         * ====================================================
         * copyright 2016 NumScale SAS
         *
         * Distributed under the Boost Software License, Version 1.0.
         * (See copy at http://boost.org/LICENSE_1_0.txt)
         * ====================================================
         */

        struct trigo_radian_tag
        {
        };
        struct trigo_pi_tag
        {
        };

        template <class B, class Tag = trigo_radian_tag>
        struct trigo_reducer
        {
            static inline B reduce(const B& x, B& xr)
            {
                if (all(x <= pio4<B>()))
                {
                    xr = x;
                    return B(0.);
                }
                else if (all(x <= pio2<B>()))
                {
                    auto test = x > pio4<B>();
                    xr = x - pio2_1<B>();
                    xr -= pio2_2<B>();
                    xr -= pio2_3<B>();
                    xr = select(test, xr, x);
                    return select(test, B(1.), B(0.));
                }
                else if (all(x <= twentypi<B>()))
                {
                    B xi = nearbyint(x * twoopi<B>());
                    xr = fnma(xi, pio2_1<B>(), x);
                    xr -= xi * pio2_2<B>();
                    xr -= xi * pio2_3<B>();
                    return quadrant(xi);
                }
                else if (all(x <= mediumpi<B>()))
                {
                    B fn = nearbyint(x * twoopi<B>());
                    B r = x - fn * pio2_1<B>();
                    B w = fn * pio2_1t<B>();
                    B t = r;
                    w = fn * pio2_2<B>();
                    r = t - w;
                    w = fn * pio2_2t<B>() - ((t - r) - w);
                    t = r;
                    w = fn * pio2_3<B>();
                    r = t - w;
                    w = fn * pio2_3t<B>() - ((t - r) - w);
                    xr = r - w;
                    return quadrant(fn);
                }
                else
                {
                    static constexpr std::size_t size = B::size;
                    using value_type = typename B::value_type;
                    alignas(B) std::array<value_type, size> tmp;
                    alignas(B) std::array<value_type, size> txr;
                    for (std::size_t i = 0; i < size; ++i)
                    {
                        double arg = x[i];
                        if (arg == std::numeric_limits<value_type>::infinity())
                        {
                            tmp[i] = 0.;
                            txr[i] = std::numeric_limits<value_type>::quiet_NaN();
                        }
                        else
                        {
                            double y[2];
                            std::int32_t n = detail::__ieee754_rem_pio2(arg, y);
                            tmp[i] = value_type(n & 3);
                            txr[i] = value_type(y[0]);
                        }
                    }
                    xr.load_aligned(&txr[0]);
                    B res;
                    res.load_aligned(&tmp[0]);
                    return res;
                }
            }
        };

        template <class B>
        struct trigo_reducer<B, trigo_pi_tag>
        {
            static inline B reduce(const B& x, B& xr)
            {
                B xi = nearbyint(x * B(2.));
                B x2 = x - xi * B(0.5);
                xr = x2 * pi<B>();
                return quadrant(xi);
            }
        };

        template <class B, class T = typename B::value_type>
        struct quadrant_impl
        {
            static inline B compute(const B& x)
            {
                return x & B(3);
            }
        };

        template <class B>
        struct quadrant_impl<B, float>
        {
            static inline B compute(const B& x)
            {
                return to_float(quadrant(to_int(x)));
            }
        };

        template <class B>
        struct quadrant_impl<B, double>
        {
            static inline B compute(const B& x)
            {
                B a = x * B(0.25);
                return (a - floor(a)) * B(4.);
            }
        };
    }

    template <class T, std::size_t N>
    inline batch<T, N> quadrant(const batch<T, N>& x)
    {
        return detail::quadrant_impl<batch<T, N>, T>::compute(x);
    }
}

#endif
