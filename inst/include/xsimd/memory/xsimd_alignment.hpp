/***************************************************************************
* Copyright (c) 2016, Johan Mabille and Sylvain Corlay                     *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XSIMD_ALIGNMENT_HPP
#define XSIMD_ALIGNMENT_HPP

#include "../config/xsimd_align.hpp"
#include "xsimd_aligned_allocator.hpp"

namespace xsimd
{
    /**
    * @struct aligned_mode
    * @brief tag for load and store of aligned memory.
    */
    struct aligned_mode
    {
    };

    /**
    * @struct unaligned_mode
    * @brief tag for load and store of unaligned memory.
    */
    struct unaligned_mode
    {
    };

    /***********************
     * Allocator alignment *
     ***********************/

    template <class A>
    struct allocator_alignment
    {
        using type = unaligned_mode;
    };

#if defined(XSIMD_X86_INSTR_SET_AVAILABLE) || defined(XSIMD_ARM_INSTR_SET_AVAILABLE)
    template <class T>
    struct allocator_alignment<aligned_allocator<T, XSIMD_DEFAULT_ALIGNMENT>>
    {
        using type = aligned_mode;
    };
#endif

    template <class A>
    using allocator_alignment_t = typename allocator_alignment<A>::type;

    /***********************
     * container alignment *
     ***********************/

    namespace detail
    {
        template <class T>
        struct void_t
        {
            using type = void;
        };
    }

    template <class C, class = void>
    struct container_alignment
    {
        using type = unaligned_mode;
    };

    template <class C>
    struct container_alignment<C, typename detail::void_t<typename C::allocator_type>::type>
    {
        using type = allocator_alignment_t<typename C::allocator_type>;
    };

    template <class C>
    using container_alignment_t = typename container_alignment<C>::type;

}

#endif
