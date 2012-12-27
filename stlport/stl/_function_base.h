/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999
 * Boris Fomitchev
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

#ifndef _STLP_INTERNAL_FUNCTION_BASE_H
#define _STLP_INTERNAL_FUNCTION_BASE_H

// #if defined (_STLP_CLASS_PARTIAL_SPECIALIZATION) && !defined (_STLP_TYPE_TRAITS_H)
// #  include <stl/type_traits.h>
// #endif

#include <type_traits>
#include <utility>

#ifndef _STLP_INTERNAL_HAS_TYPE_H
#  include <stl/_has_type.h>
#endif

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Se, class _Tp>
struct __result_type_aux
{
};

template <class _Tp>
struct __result_type_aux<true_type,_Tp>
{
    typedef typename _Tp::result_type result_type;
};

template <class _Se, class _Tp>
struct __argument_type_aux
{
};

template <class _Tp>
struct __argument_type_aux<true_type,_Tp>
{
    typedef typename _Tp::argument_type argument_type;
};

template <class _Se, class _Tp>
struct __first_argument_type_aux
{
};

template <class _Tp>
struct __first_argument_type_aux<true_type,_Tp>
{
    typedef typename _Tp::first_argument_type first_argument_type;
};

template <class _Se, class _Tp>
struct __second_argument_type_aux
{
};

template <class _Tp>
struct __second_argument_type_aux<true_type,_Tp>
{
    typedef typename _Tp::second_argument_type second_argument_type;
};

template <bool _Se, class Fn, class ...ArgTypes>
struct __functor_aux
{
    typedef decltype(declval<Fn>()(declval<ArgTypes>()...)) result_type;
};

template <class Fn, class ...ArgTypes>
struct __functor_aux<true,Fn,ArgTypes...>
{
    typedef decltype(Fn(declval<ArgTypes>()...)) result_type;
};

template <class _Tp, class ...ArgTypes>
struct __arg1_aux
{
    typedef _Tp argument_type;
};

template <class _Tp1, class _Tp2, class ...ArgTypes>
struct __arg2_aux
{
    typedef _Tp1 argument_type;
    typedef _Tp1 first_argument_type;
    typedef _Tp2 second_argument_type;
};

template <int N, class ...ArgTypes>
struct __arg1 :
    public __arg1_aux<ArgTypes...>
{
};

template <>
struct __arg1<0>
{
};

template <class ...ArgTypes>
struct __arg1<1,ArgTypes...> :
    public __arg1_aux<ArgTypes...>
{
};

template <class ...ArgTypes>
struct __arg1<2,ArgTypes...> :
    public __arg2_aux<ArgTypes...>
{
};

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Tp, class ...Args>
class reference_wrapper :
        public _STLP_PRIV __result_type_aux<decltype(detail::__has_type_selector::__test_result_type<_Tp>(0)),_Tp>,
        public _STLP_PRIV __argument_type_aux<decltype(detail::__has_type_selector::__test_argument_type<_Tp>(0)),_Tp>,
        public _STLP_PRIV __first_argument_type_aux<decltype(detail::__has_type_selector::__test_first_argument_type<_Tp>(0)),_Tp>,
        public _STLP_PRIV __second_argument_type_aux<decltype(detail::__has_type_selector::__test_second_argument_type<_Tp>(0)),_Tp>
{
  public:
    typedef _Tp type;
    // typedef ... result_type;
    // typedef ... argument_type;
    // typedef ... first_argument_type;
    // typedef ... second_argument_type;

    reference_wrapper( _Tp& __x ) noexcept :
        _ref( addressof(__x) )
      { }
    reference_wrapper( _Tp&& ) = delete; // do not bind to temporary objects
    reference_wrapper( const reference_wrapper<_Tp>& __x ) noexcept :
        _ref( __x._ref )
      { }

    reference_wrapper& operator =( const reference_wrapper<_Tp>& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator _Tp&() const noexcept
      { return *_ref; }
    _Tp& get() const noexcept
      { return *_ref; }

    template <class ...ArgTypes>
    typename result_of<_Tp&(ArgTypes&&...)>::type
      operator ()(ArgTypes&&... args) const
      { return (*_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    typename remove_reference<type>::type* _ref;
};

namespace detail {

template <bool, class Fn, class ... ArgTypes> struct __r_member_selector;

template <class R, class T, class A, class ... ArgTypes>
class __r_member_selector<false,R T::*,A,ArgTypes...> // member function
{
  private:
    typedef R T::* _Fn;

    template <class U>
    static U __test( const T& );

    template <class U>
    static decltype(*declval<U>()) __test( ... );

  public:
    typedef decltype( (__test<A>(declval<A>()).*declval<_Fn>())(declval<ArgTypes>()...) ) result_type;

  protected:
    static result_type __invoke( _Fn f, A a, ArgTypes&&... args )
      { return ((a).*f)( _STLP_STD::forward<ArgTypes>(args)... ); }
};

template <class R, class T, class A /* , class ... ArgTypes */>
class __r_member_selector<true,R T::*,A /* ,ArgTypes... */ > // member data
{
  private:
    typedef R T::* _Fn;

    static R&& __test( const T& );

    static const R& __test( const T* );

  public:
    typedef decltype(__test(declval<A>())) result_type;

  protected:
    static result_type __invoke( _Fn f, A a /* , ArgTypes&&... */ )
      { return a.*f; }
};

template <bool, class Fn, class ...ArgTypes>
struct __r_is_function : // member data or member function
        public __r_member_selector<is_member_object_pointer<Fn>::value,Fn,ArgTypes...>
{ };

template <class Fn, class ...ArgTypes>
class __r_is_function<true,Fn,ArgTypes...> // function
{
  public:
    typedef decltype(Fn(declval<ArgTypes>()...)) result_type;

  protected:
    static result_type __invoke( ArgTypes&&... args )
      { return Fn( forward<ArgTypes>(args)... ); }
};

template <bool, class Fn, class ... ArgTypes>
struct __r_is_functor : // function or member data or member function
        public __r_is_function<!is_member_pointer<Fn>::value,Fn,ArgTypes...>
{ };

template <class Fn, class ... ArgTypes>
class __r_is_functor<true,Fn,ArgTypes...> // functor
{
  public:
    typedef decltype( declval<Fn>()(declval<ArgTypes>()...) ) result_type;

  protected:
    static result_type __invoke( Fn& f, ArgTypes&&... args )
      { return f( _STLP_STD::forward<ArgTypes>(args)... ); }
};

} // namespace detail

template <class Fn, class ...ArgTypes>
class reference_wrapper<Fn(ArgTypes...)> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes),ArgTypes...>,
    public detail::__r_is_functor<!is_function<Fn(ArgTypes...)>::value,Fn,ArgTypes...>
{
  private:
    typedef typename detail::__r_is_functor<!is_function<Fn(ArgTypes...)>::value,Fn,ArgTypes...> Base;

  public:
    typedef typename remove_reference<Fn>::type /* Fn */ type;
    // typedef decltype( declval<type>()( declval<ArgTypes>()... ) ) result_type;
    // typedef ... argument_type;
    // typedef ... first_argument_type;
    // typedef ... second_argument_type;

    reference_wrapper( type& __x ) noexcept :
        _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) = delete; // do not bind to temporary objects
    reference_wrapper( const reference_wrapper<Fn,ArgTypes...>& __x ) noexcept :
        _ref( __x._ref )
      { }

    reference_wrapper& operator =( const reference_wrapper<Fn,ArgTypes...>& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    // template <class ...ArgTypes>
    // typename result_of<Fn&(ArgTypes&&...)>::type
    typename Base::result_type operator ()(ArgTypes&&... args) const
      { return Base::__invoke(*_ref, _STLP_STD::forward<ArgTypes>(args)...); }
    // template <class ...Args>
    // typename result_of<Fn&(Args&&...)>::type
    // operator ()( Args&&... args ) const
    //  { } 

//    typename Base::result_type invoke(ArgTypes&&... args) const
//      { return Base::__invoke(*_ref, _STLP_STD::forward<ArgTypes>(args)...); }

  private:
    /* typename remove_reference<type>:: */ type* _ref;
};

template <class R, class ...ArgTypes>
class reference_wrapper<R (*)(ArgTypes...)> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes),ArgTypes...>
{
  public:
    typedef R (*type)(ArgTypes...);
    typedef R result_type;

    reference_wrapper( type& __x ) noexcept :
        _ref( __x )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :
        _ref( __x._ref )
      { }

    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator type&() const noexcept
      { return _ref; }
    type& get() const noexcept
      { return _ref; }

    result_type operator ()(ArgTypes&&... args) const
      { return (*_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    typename remove_reference<type>::type _ref;
};

template <class R, class T0, class ...ArgTypes>
class reference_wrapper<R (T0::*)(ArgTypes...)> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes)+1,T0*,ArgTypes...> 
{
  public:
    typedef R (T0::*type)(ArgTypes...);
    typedef R result_type;

    reference_wrapper( type& __x ) noexcept :
      _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :
      _ref( __x._ref )
      { }
    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    result_type operator ()(T0&& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    type* _ref;
};

template <class R, class T0, class ...ArgTypes>
class reference_wrapper<R (T0::*)(ArgTypes...) const> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes)+1,const T0*,ArgTypes...>
{
  public:
    typedef R (T0::*type)(ArgTypes...) const;
    typedef R result_type;

    reference_wrapper( type& __x ) noexcept :
      _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :
        _ref( __x._ref )
      { }
    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    result_type operator ()(T0&& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    type* _ref;
};

template <class R, class T0, class ...ArgTypes>
class reference_wrapper<R (T0::*)(ArgTypes...) volatile> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes)+1,volatile T0*,ArgTypes...>
{ 
  public:
    typedef R (T0::*type)(ArgTypes...) volatile;
    typedef R result_type;
    
    reference_wrapper( type& __x ) noexcept :
      _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :     
        _ref( __x._ref )
      { }
    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }
    
    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    result_type operator ()(T0&& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    type* _ref;
};

template <class R, class T0, class ...ArgTypes>
class reference_wrapper<R (T0::*)(ArgTypes...) const volatile> :
    public _STLP_PRIV __arg1<sizeof...(ArgTypes)+1,const volatile T0*,ArgTypes...>
{ 
  public:
    typedef R (T0::*type)(ArgTypes...) const volatile;
    typedef R result_type;
    
    reference_wrapper( type& __x ) noexcept :
      _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :     
        _ref( __x._ref )
      { }
    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }
    
    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    result_type operator ()(T0&& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0& a, ArgTypes&&... args) const
    { return (a.**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(const T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }
    result_type operator ()(T0* a, ArgTypes&&... args) const
    { return (a->**_ref)(_STLP_STD::forward<ArgTypes>(args)...); }

  private:
    type* _ref;
};

template <class R, class T0>
class reference_wrapper<R T0::*>
{
  public:
    typedef R T0::*type;

    reference_wrapper( type& __x ) noexcept :
        _ref( addressof(__x) )
      { }
    reference_wrapper( type&& ) noexcept = delete;
    reference_wrapper( const reference_wrapper& __x ) noexcept :
        _ref( __x._ref )
      { }

    reference_wrapper& operator =( const reference_wrapper& __x ) noexcept
      { _ref = __x._ref; return *this; }

    operator type&() const noexcept
      { return *_ref; }
    type& get() const noexcept
      { return *_ref; }

    R operator ()(T0&& a) const
      { return a.**_ref; }
    R operator ()(T0& a) const
      { return a.**_ref; }
    R operator ()(const T0& a) const
      { return a.**_ref; }
    R operator ()(T0* a) const
      { return a->**_ref; }
    R operator ()(const T0* a) const
      { return a->**_ref; }

  private:
    /* typename remove_reference<type>:: */ type* _ref;
};

template <class _Tp>
reference_wrapper<_Tp> ref( _Tp& __x ) noexcept
{ return reference_wrapper<_Tp>( __x ); }

template <class _Tp>
reference_wrapper<_Tp> ref( reference_wrapper<_Tp>& __x ) noexcept
{ return ref( __x.get() ); }

template <class _Tp>
reference_wrapper<const _Tp> cref( const _Tp& __x ) noexcept
{ return reference_wrapper<const _Tp>( __x ); }

template <class _Tp>
reference_wrapper<const _Tp> cref( reference_wrapper<const _Tp>& __x ) noexcept
{ return cref( __x.get() ); }

template <class _Tp>
void ref( const _Tp&& ) = delete;

template <class _Tp>
void cref( const _Tp&& ) = delete;

template <class _Arg, class _Result>
struct unary_function {
  typedef _Arg argument_type;
  typedef _Result result_type;
#if !defined (__BORLANDC__) || (__BORLANDC__ < 0x580)
protected:
  /* This class purpose is to be derived but it is not polymorphic so users should never try
   * to destroy an instance of it directly. The protected non-virtual destructor make this
   * fact obvious at compilation time. */
  ~unary_function() {}
#endif
};

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
  typedef _Arg1 first_argument_type;
  typedef _Arg2 second_argument_type;
  typedef _Result result_type;
#if !defined (__BORLANDC__) || (__BORLANDC__ < 0x580)
protected:
  /* See unary_function comment. */
  ~binary_function() {}
#endif
};

template <class _Tp = void>
struct equal_to /* : public binary_function<_Tp, _Tp, bool> */
{
    bool operator()(const _Tp& __x, const _Tp& __y) const
      { return __x == __y; }

    typedef _Tp first_argument_type;
    typedef _Tp second_argument_type;
    typedef bool result_type;
};

template <class _Tp = void>
struct less /* : public binary_function<_Tp,_Tp,bool> */
{
    bool operator()(const _Tp& __x, const _Tp& __y) const
      { return __x < __y; }

    typedef _Tp first_argument_type;
    typedef _Tp second_argument_type;
    typedef bool result_type;
};

template <>
struct less<void>
{
    template <class T, class U>
    auto operator ()(T&& t, U&& u) const -> decltype(_STLP_STD::forward<T>(t) < _STLP_STD::forward<U>(u))
      { return _STLP_STD::forward<T>(t) < _STLP_STD::forward<U>(u);}
};

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Tp>
less<_Tp> __less(_Tp* ) { return less<_Tp>(); }

template <class _Tp>
equal_to<_Tp> __equal_to(_Tp* ) { return equal_to<_Tp>(); }

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Tp = void>
struct plus /* : public binary_function<_Tp, _Tp, _Tp> */
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
      { return __x + __y; }

    typedef _Tp first_argument_type;
    typedef _Tp second_argument_type;
    typedef _Tp result_type;
};

template <class _Tp = void>
struct minus /* : public binary_function<_Tp, _Tp, _Tp> */
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
      { return __x - __y; }

    typedef _Tp first_argument_type;
    typedef _Tp second_argument_type;
    typedef _Tp result_type;
};

template <>
struct plus<void>
{
    template <class T, class U>
    auto operator ()(T&& t, U&& u) const -> decltype(_STLP_STD::forward<T>(t) + _STLP_STD::forward<U>(u))
      { return _STLP_STD::forward<T>(t) + _STLP_STD::forward<U>(u);}
};

template <>
struct minus<void>
{
    template <class T, class U>
    auto operator ()(T&& t, U&& u) const -> decltype(_STLP_STD::forward<T>(t) - _STLP_STD::forward<U>(u))
      { return _STLP_STD::forward<T>(t) - _STLP_STD::forward<U>(u);}
};

//_STLP_MOVE_TO_PRIV_NAMESPACE
//
//template <class _Tp>
//plus<_Tp> __plus(_Tp* ) { return plus<_Tp>(); }
//
//template <class _Tp>
//minus<_Tp> __minus(_Tp* ) { return minus<_Tp>(); }
//
//_STLP_MOVE_TO_STD_NAMESPACE

template <class _Tp>
struct multiplies /* : public binary_function<_Tp, _Tp, _Tp> */
{
    _Tp operator()(const _Tp& __x, const _Tp& __y) const
      { return __x * __y; }

    typedef _Tp first_argument_type;
    typedef _Tp second_argument_type;
    typedef _Tp result_type;
};

template <>
struct multiplies<void>
{
    template <class T, class U>
    auto operator ()(T&& t, U&& u) const -> decltype(_STLP_STD::forward<T>(t) * _STLP_STD::forward<U>(u))
      { return _STLP_STD::forward<T>(t) * _STLP_STD::forward<U>(u);}
};

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type> {
  const typename _Pair::first_type& operator()(const _Pair& __x) const {
    return __x.first;
  }
};

template <class _Pair>
struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type> {
  const typename _Pair::second_type& operator()(const _Pair& __x) const {
    return __x.second;
  }
};

// project1st and project2nd are extensions: they are not part of the standard
template <class _Arg1, class _Arg2>
struct _Project1st : public binary_function<_Arg1, _Arg2, _Arg1> {
  _Arg1 operator()(const _Arg1& __x, const _Arg2&) const { return __x; }
};

template <class _Arg1, class _Arg2>
struct _Project2nd : public binary_function<_Arg1, _Arg2, _Arg2> {
  _Arg2 operator()(const _Arg1&, const _Arg2& __y) const { return __y; }
};

#if defined (_STLP_MULTI_CONST_TEMPLATE_ARG_BUG)
// fbp : sort of select1st just for maps
template <class _Pair, class _Whatever>
// JDJ (CW Pro1 doesn't like const when first_type is also const)
struct __Select1st_hint : public unary_function<_Pair, _Whatever> {
    const _Whatever& operator () (const _Pair& __x) const { return __x.first; }
};
#  define  _STLP_SELECT1ST(__x,__y) _STLP_PRIV __Select1st_hint< __x, __y >
#else
#  define  _STLP_SELECT1ST(__x, __y) _STLP_PRIV _Select1st< __x >
#endif

template <class _Tp>
struct _Identity : public unary_function<_Tp,_Tp> {
  const _Tp& operator()(const _Tp& __x) const { return __x; }
};

template <class _Result, class _Argument>
struct _Constant_unary_fun {
  typedef _Argument argument_type;
  typedef  _Result  result_type;
  result_type _M_val;

  _Constant_unary_fun(const result_type& __v) : _M_val(__v) {}
  const result_type& operator()(const _Argument&) const { return _M_val; }
};

template <class _Result, class _Arg1, class _Arg2>
struct _Constant_binary_fun {
  typedef  _Arg1   first_argument_type;
  typedef  _Arg2   second_argument_type;
  typedef  _Result result_type;
  _Result _M_val;

  _Constant_binary_fun(const _Result& __v) : _M_val(__v) {}
  const result_type& operator()(const _Arg1&, const _Arg2&) const {
    return _M_val;
  }
};

// identity_element (not part of the C++ standard).
template <class _Tp> inline _Tp __identity_element(plus<_Tp>) {  return _Tp(0); }
template <class _Tp> inline _Tp __identity_element(multiplies<_Tp>) { return _Tp(1); }

_STLP_MOVE_TO_STD_NAMESPACE

_STLP_END_NAMESPACE

#endif /* _STLP_INTERNAL_FUNCTION_BASE_H */

// Local Variables:
// mode:C++
// End:
