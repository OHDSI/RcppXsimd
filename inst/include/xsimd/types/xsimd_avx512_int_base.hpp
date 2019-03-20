/***************************************************************************
* Copyright (c) 2016, Wolf Vollprecht, Johan Mabille and Sylvain Corlay    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_AVX_INT512_BASE_HPP
#define XSIMD_AVX_INT512_BASE_HPP

#include "xsimd_base.hpp"
#include "xsimd_utils.hpp"

namespace xsimd
{

#define XSIMD_SPLIT_AVX512(avx_name)                                                                  \
    __m256i avx_name##_low = _mm512_castsi512_si256(avx_name);                                        \
    __m256i avx_name##_high = _mm512_extracti64x4_epi64(avx_name, 1)                                  \

#define XSIMD_RETURN_MERGED_AVX(res_low, res_high)                                                    \
    __m512i result = _mm512_castsi256_si512(res_low);                                                 \
    return _mm512_inserti64x4(result, res_high, 1)                                                    \

#define XSIMD_APPLY_AVX2_FUNCTION(N, func, avx_lhs, avx_rhs)                                          \
    XSIMD_SPLIT_AVX512(avx_lhs);                                                                      \
    XSIMD_SPLIT_AVX512(avx_rhs);                                                                      \
    __m256i res_low = detail::batch_kernel<value_type, N> :: func (avx_lhs##_low, avx_rhs##_low);     \
    __m256i res_high = detail::batch_kernel<value_type, N> :: func (avx_lhs##_high, avx_rhs##_high);  \
    XSIMD_RETURN_MERGED_AVX(res_low, res_high);

    template <class T, std::size_t N>
    class avx512_int_batch : public simd_batch<batch<T, N>>
    {
    public:

        using base_type = simd_batch<batch<T, N>>;

        avx512_int_batch();
        explicit avx512_int_batch(T i);

        template <class... Args, class Enable = detail::is_array_initializer_t<T, N, Args...>>
        avx512_int_batch(Args... exactly_N_scalars);
        explicit avx512_int_batch(const T* src);
        avx512_int_batch(const T* src, aligned_mode);
        avx512_int_batch(const T* src, unaligned_mode);

        avx512_int_batch(const __m512i& rhs);
        avx512_int_batch& operator=(const __m512i& rhs);

        operator __m512i() const;

        batch<T, N>& load_aligned(const T* src);
        batch<T, N>& load_unaligned(const T* src);

        batch<T, N>& load_aligned(const flipped_sign_type_t<T>* src);
        batch<T, N>& load_unaligned(const flipped_sign_type_t<T>* src);

        void store_aligned(T* dst) const;
        void store_unaligned(T* dst) const;

        void store_aligned(flipped_sign_type_t<T>* dst) const;
        void store_unaligned(flipped_sign_type_t<T>* dst) const;

        using base_type::load_aligned;
        using base_type::load_unaligned;
        using base_type::store_aligned;
        using base_type::store_unaligned;

        T operator[](std::size_t index) const;

    protected:

        __m512i m_value;
    };

    /***********************************
     * avx512_int_batch implementation *
     ***********************************/

    namespace avx512_detail
    {
        template<class Tup, std::size_t... Is>
        __m512i revert_args_set_epi8(Tup&& t, detail::index_sequence<Is...>)
        {
            // funny, this instruction is not yet implemented in clang or gcc (will come in future versions)
#if defined(__clang__) || __GNUC__
            return __extension__ (__m512i)(__v64qi)
            {
                std::get<Is>(std::forward<Tup>(t))...
            };
#else
            return _mm512_set_epi8(std::get<Is>(std::forward<Tup>(t))...);
#endif
        }

        template<class Tup, std::size_t... Is>
        __m512i revert_args_set_epi16(Tup&& t, detail::index_sequence<Is...>)
        {
#if defined(__clang__) || __GNUC__
            return __extension__ (__m512i)(__v32hi)
            {
                std::get<Is>(std::forward<Tup>(t))...
            };
#else
            return _mm512_set_epi16(std::get<Is>(std::forward<Tup>(t))...);
#endif
        }

        template <class... Args>
        __m512i int_init(std::integral_constant<std::size_t, 1>, Args... args)
        {
            return revert_args_set_epi8(std::forward_as_tuple(args...), detail::make_index_sequence<sizeof...(Args)>{});
        }

        template <class... Args>
        __m512i int_init(std::integral_constant<std::size_t, 2>, Args... args)
        {
            return revert_args_set_epi16(std::forward_as_tuple(args...), detail::make_index_sequence<sizeof...(Args)>{});
        }

        inline __m512i int_init(std::integral_constant<std::size_t, 4>,
                                int32_t t0, int32_t t1, int32_t t2, int32_t t3,
                                int32_t t4, int32_t t5, int32_t t6, int32_t t7,
                                int32_t t8, int32_t t9, int32_t t10, int32_t t11,
                                int32_t t12, int32_t t13, int32_t t14, int32_t t15)
        {
            // _mm512_setr_epi32 is a macro, preventing parameter pack expansion ...
            return _mm512_setr_epi32(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15);
        }

        inline __m512i int_init(std::integral_constant<std::size_t, 8>,
                                int64_t t0, int64_t t1, int64_t t2, int64_t t3,
                                int64_t t4, int64_t t5, int64_t t6, int64_t t7)
        {
            // _mm512_setr_epi64 is a macro, preventing parameter pack expansion ...
            return _mm512_setr_epi64(t0, t1, t2, t3, t4, t5, t6, t7);
        }

        template <class T>
        inline __m512i int_set(std::integral_constant<std::size_t, 1>, T v)
        {
            return _mm512_set1_epi8(v);
        }

        template <class T>
        inline __m512i int_set(std::integral_constant<std::size_t, 2>, T v)
        {
            return _mm512_set1_epi16(v);
        }

        template <class T>
        inline __m512i int_set(std::integral_constant<std::size_t, 4>, T v)
        {
            return _mm512_set1_epi32(v);
        }

        template <class T>
        inline __m512i int_set(std::integral_constant<std::size_t, 8>, T v)
        {
            return _mm512_set1_epi64(v);
        }
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch()
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch(T i)
        : m_value(avx512_detail::int_set(std::integral_constant<std::size_t, sizeof(T)>{}, i))
    {
    }

    template <class T, std::size_t N>
    template <class... Args, class>
    inline avx512_int_batch<T, N>::avx512_int_batch(Args... args)
        : m_value(avx512_detail::int_init(std::integral_constant<std::size_t, sizeof(T)>{}, args...))
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch(const T* src)
        : m_value(_mm512_loadu_si512((__m512i const*) src))
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch(const T* src, aligned_mode)
        : m_value(_mm512_load_si512((__m512i const*) src))
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch(const T* src, unaligned_mode)
        : m_value(_mm512_loadu_si512((__m512i const*) src))
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::avx512_int_batch(const __m512i& rhs)
        : m_value(rhs)
    {
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>& avx512_int_batch<T, N>::operator=(const __m512i& rhs)
    {
        m_value = rhs;
        return *this;
    }

    template <class T, std::size_t N>
    inline avx512_int_batch<T, N>::operator __m512i() const
    {
        return m_value;
    }

    template <class T, std::size_t N>
    inline batch<T, N>& avx512_int_batch<T, N>::load_aligned(const T* src)
    {
        m_value = _mm512_load_si512((__m512i const*) src);
        return (*this)();
    }

    template <class T, std::size_t N>
    inline batch<T, N>& avx512_int_batch<T, N>::load_unaligned(const T* src)
    {
        m_value = _mm512_loadu_si512((__m512i const*) src);
        return (*this)();
    }

    template <class T, std::size_t N>
    inline batch<T, N>& avx512_int_batch<T, N>::load_aligned(const flipped_sign_type_t<T>* src)
    {
        m_value = _mm512_load_si512((__m512i const*) src);
        return (*this)();
    }

    template <class T, std::size_t N>
    inline batch<T, N>& avx512_int_batch<T, N>::load_unaligned(const flipped_sign_type_t<T>* src)
    {
        m_value = _mm512_loadu_si512((__m512i const*) src);
        return (*this)();
    }

    template <class T, std::size_t N>
    inline void avx512_int_batch<T, N>::store_aligned(T* dst) const
    {
        _mm512_store_si512(dst, m_value);
    }

    template <class T, std::size_t N>
    inline void avx512_int_batch<T, N>::store_unaligned(T* dst) const
    {
        _mm512_storeu_si512(dst, m_value);
    }

    template <class T, std::size_t N>
    inline void avx512_int_batch<T, N>::store_aligned(flipped_sign_type_t<T>* dst) const
    {
        _mm512_store_si512(dst, m_value);
    }

    template <class T, std::size_t N>
    inline void avx512_int_batch<T, N>::store_unaligned(flipped_sign_type_t<T>* dst) const
    {
        _mm512_storeu_si512(dst, m_value);
    }

    template <class T, std::size_t N>
    inline T avx512_int_batch<T, N>::operator[](std::size_t index) const
    {
        alignas(64) T x[N];
        store_aligned(x);
        return x[index & (N - 1)];
    }

    namespace avx512_detail
    {
        template <class F, class T, std::size_t N>
        inline batch<T, N> shift_impl(F&& f, const batch<T, N>& lhs, int32_t rhs)
        {
            alignas(64) T tmp_lhs[N], tmp_res[N];
            lhs.store_aligned(&tmp_lhs[0]);
            unroller<N>([&](std::size_t i) {
                tmp_res[i] = f(tmp_lhs[i], rhs);
            });
            return batch<T, N>(tmp_res, aligned_mode());
        }
    }
}

#endif
