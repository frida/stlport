/*
 *
 * Copyright (c) 2012
 * Petr Ovtchenkov
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

/* NOTE: include it after <type_traits>
 */

#ifndef _STLP_INTERNAL_HAS_TYPE_H
#define _STLP_INTERNAL_HAS_TYPE_H

_STLP_BEGIN_NAMESPACE

namespace detail {

struct __has_type_selector
{
   // T::result_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::result_type*), declval<true_type>())>::type __test_result_type( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::result_type>(), declval<true_type>())>::type __test_result_type( int );
#endif

    template <class>
    static false_type __test_result_type( ... );

     // T::argument_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::argument_type*), declval<true_type>())>::type __test_argument_type( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::argument_type>(), declval<true_type>())>::type __test_argument_type( int );
#endif

    template <class>
    static false_type __test_argument_type( ... );

     // T::first_argument_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::first_argument_type*), declval<true_type>())>::type __test_first_argument_type( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::first_argument_type>(), declval<true_type>())>::type __test_first_argument_type( int );
#endif

    template <class>
    static false_type __test_first_argument_type( ... );

     // T::second_argument_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::second_argument_type*), declval<true_type>())>::type __test_second_argument_type( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::second_argument_type>(), declval<true_type>())>::type __test_second_argument_type( int );
#endif

    template <class>
    static false_type __test_second_argument_type( ... );

    // T::element_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::element_type*), declval<true_type>())>::type __test( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::element_type>(), declval<true_type>())>::type __test( int );
#endif

    template <class>
    static false_type __test( ... );

    // T::difference_type?
    template <class T>
    static typename T::difference_type __test_d( int );

    template <class>
    static typename _STLP_STD::ptrdiff_t __test_d( ... );

    // T::rebind<U>?
    template <class T, class U>
    static typename remove_reference<decltype( sizeof(typename T::template rebind<U>::type*), declval<true_type>())>::type __test_r( int );

    template <class, class>
    static false_type __test_r( ... );

    // T::allocator_type?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::allocator_type>(), declval<true_type>())>::type __test_a( int );

    template <class>
    static false_type __test_a( ... );

    // T::pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::pointer), declval<true_type>())>::type __test_p( int );
#else
    template <class T>
    static typename remove_reference<decltype(T::pointer(), declval<true_type>())>::type __test_p( int );
#endif

    template <class>
    static false_type __test_p( ... );

    // T::const_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::const_pointer), declval<true_type>())>::type __test_cp( int );
#else
    template <class T>
    static typename remove_reference<decltype(T::const_pointer(), declval<true_type>())>::type __test_cp( int );
#endif

    template <class>
    static false_type __test_cp( ... );

    // T::void_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::void_pointer), declval<true_type>())>::type __test_vp( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::void_pointer>(), declval<true_type>())>::type __test_vp( int );
#endif

    template <class>
    static false_type __test_vp( ... );

    // T::const_void_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::const_void_pointer), declval<true_type>())>::type __test_cvp( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::const_void_pointer>(), declval<true_type>())>::type __test_cvp( int );
#endif

    template <class>
    static false_type __test_cvp( ... );

    // T::difference_type, another?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::difference_type>(), declval<true_type>())>::type __test_dp( int );

    template <class>
    static false_type __test_dp( ... );

    // T::size_type?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::size_type>(), declval<true_type>())>::type __test_sz( int );

    template <class>
    static false_type __test_sz( ... );

    // T::propagate_on_container_copy_assignment?
    template <class T>
    static decltype( declval<typename T::propagate_on_container_copy_assignment>() ) __test_pcca( int );

    template <class>
    static false_type __test_pcca( ... );

    // T::propagate_on_container_move_assignment?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::propagate_on_container_move_assignment>() )>::type __test_pcma( int );

    template <class>
    static false_type __test_pcma( ... );

    // T::propagate_on_container_swap?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::propagate_on_container_swap>() )>::type __test_pcs( int );

    template <class>
    static false_type __test_pcs( ... );

    // T::rebind<U>::other?
    template <class T, class U>
    static typename remove_reference<decltype( declval<typename T::template rebind<U>::other>(), declval<true_type>())>::type __test_ro( int );

    template <class, class>
    static false_type __test_ro( ... );

    // a.allocate( 0, NULL )?
    template <class A>
    static typename remove_reference<decltype( declval<A>().allocate(0,NULL), declval<true_type>())>::type __test_ah( int );

    template <class>
    static false_type __test_ah( ... );

    // a.construct( p, args... )?
    template <class A, class T, class ... Args>
    static typename remove_reference<decltype( declval<A>().construct( declval<T*>(), declval<Args>()...), declval<true_type>())>::type __test_construct( int );

    template <class, class, class ...>
    static false_type __test_construct( ... );

    // a.destroy( p )?
    template <class A, class T>
    static decltype( declval<A>().destroy( declval<T*>() ), declval<true_type>()) __test_destroy( int );

    template <class, class>
    static false_type __test_destroy( ... );

    // a.max_size()?
    template <class A>
    static typename remove_reference<decltype( declval<A>().max_size(), declval<true_type>())>::type __test_max_size( int );

    template <class, class>
    static false_type __test_max_size( ... );

    // a.select_on_container_copy_construction()?
    template <class A>
    static typename remove_reference<decltype( declval<A>().select_on_container_copy_construction(), declval<true_type>())>::type __test_soccc( int );

    template <class, class>
    static false_type __test_soccc( ... );
};

} // namespace detail

template <class T>
inline T* addressof(T& r) noexcept
{ return reinterpret_cast<T*>( &const_cast<char&>( reinterpret_cast<const volatile char&>(r) ) ); }

_STLP_END_NAMESPACE

#endif // _STLP_INTERNAL_HAS_TYPE_H
