// -*- C++ -*- Time-stamp: <2011-09-23 16:17:27 ptr>

/*
 * Copyright (c) 2011
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

#include "shared_ptr_test.h"

#include <memory>
// #include <typeinfo>
#include <stl/_shared_ptr.h>

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif

#if !defined(_STLP_NO_EXTENSIONS) && defined(_STLP_USE_BOOST_SUPPORT)
struct X;

struct X :
    public std::tr1::enable_shared_from_this<X>
{
};
#endif /* !_STLP_NO_EXTENSIONS && _STLP_USE_BOOST_SUPPORT */

int EXAM_IMPL(shared_ptr_test::shared_from_this)
{
#if !defined(_STLP_NO_EXTENSIONS) && defined(_STLP_USE_BOOST_SUPPORT)
  std::tr1::shared_ptr<X> p( new X );
  std::tr1::shared_ptr<X> q = p->shared_from_this();

  EXAM_CHECK( p == q );
  EXAM_CHECK( !(p < q) && !(q < p) ); // p and q share ownership
#else
  throw exam::skip_exception();
#endif
  return EXAM_RESULT;
}

struct Test
{
    Test()
      {
        ++cnt;
      }

    ~Test()
      {
        --cnt;
      }

    static int cnt;
};

int Test::cnt = 0;

struct TestNext :
    public Test
{
    TestNext()
      {
        ++cnt_next;
      }

    ~TestNext()
      {
        --cnt_next;
      }

    static int cnt_next;
};

int TestNext::cnt_next = 0;

struct my_destroyer
{
    void operator ()( Test* p )
      {
        Test::cnt = 0;
        TestNext::cnt_next = 0;
        delete p;
      }
};

int EXAM_IMPL(shared_ptr_test::shared_ptr_base)
{
  shared_ptr<int> p;
  EXAM_CHECK( p.use_count() == 0 );
  
  EXAM_CHECK( Test::cnt == 0 );

  {
    shared_ptr<Test> pi( new Test() );

    EXAM_CHECK( Test::cnt == 1 ); // Test ctor called
    EXAM_CHECK( pi.use_count() == 1 );
  }

  EXAM_CHECK( Test::cnt == 0 ); // Test dtor called

  EXAM_CHECK( TestNext::cnt_next == 0 );
  {
    shared_ptr<Test> pi( new TestNext() ); // type convertion

    EXAM_CHECK( Test::cnt == 1 ); // Test ctor
    EXAM_CHECK( TestNext::cnt_next == 1 ); // TestNext ctor
    EXAM_CHECK( pi.use_count() == 1 );
  }

  EXAM_CHECK( Test::cnt == 0 ); // Test dtor called
  EXAM_CHECK( TestNext::cnt_next == 1 ); // TestNext dtor shouldn't be called

  {
    shared_ptr<Test> pi( new Test(), my_destroyer() ); // custom deleter

    EXAM_CHECK( Test::cnt == 1 ); // Test ctor called
    EXAM_CHECK( pi.use_count() == 1 );
  }

  EXAM_CHECK( Test::cnt == -1 ); // custom deleter clear Test::cnt, call Test dtor ...
  EXAM_CHECK( TestNext::cnt_next == 0 ); // ... and clear TestNext::cnt_next too

  {
    shared_ptr<int> pi1( new int(0) );
    EXAM_CHECK( pi1.use_count() == 1 );

    {
      shared_ptr<int> pi2( pi1 );

      EXAM_CHECK( pi1.use_count() == 2 );
      EXAM_CHECK( pi2.use_count() == 2 );
    }

    EXAM_CHECK( pi1.use_count() == 1 );
  }

  return EXAM_RESULT;
}

int EXAM_IMPL(shared_ptr_test::alias)
{
  struct A
  {
      A() :
          b( new int(5) )
        { }

      ~A()
        {
          delete b;
        }

      int* b;
  };

  shared_ptr<A> p( new A() );
  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( *p->b == 5 );

  {
    // simple alias

    shared_ptr<int> internal( p, p->b );

    EXAM_CHECK( internal.get() == p->b );
    EXAM_CHECK( internal.use_count() == p.use_count() );

    EXAM_CHECK( p.use_count() == 2 );
    EXAM_CHECK( internal.use_count() == 2 );

    EXAM_CHECK( *internal == 5 );
    *internal = 6;
  }

  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( *p->b == 6 );

  {
    // simple alias
    shared_ptr<int> internal_a( p, p->b );

    EXAM_CHECK( *internal_a == 6 );
    {
      // shared alias
      shared_ptr<int> internal_b( internal_a );

      EXAM_CHECK( p.use_count() == 3 );
      EXAM_CHECK( *internal_b == 6 );

      *internal_b = 7;

      EXAM_CHECK( *internal_a == 7 );
      EXAM_CHECK( *internal_b == 7 );
    }
    EXAM_CHECK( p.use_count() == 2 );
  }

  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( *p->b == 7 );

  *p->b = 4;

  {
    // simple alias
    shared_ptr<int> internal_a( p, p->b );

    EXAM_CHECK( *internal_a == 4 );
    {
      // yet another alias
      shared_ptr<int> internal_b( p, p->b );

      EXAM_CHECK( p.use_count() == 3 );
      EXAM_CHECK( *internal_b == 4 );

      *internal_b = 7;

      EXAM_CHECK( *internal_a == 7 );
      EXAM_CHECK( *internal_b == 7 );
    }
    EXAM_CHECK( p.use_count() == 2 );
  }

  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( *p->b == 7 );

  *p->b = 3;

  {
    // simple alias
    shared_ptr<int> internal_a( p, p->b );

    EXAM_CHECK( *internal_a == 3 );
    {
      // alias of alias
      shared_ptr<int> internal_b( internal_a, p->b );

      EXAM_CHECK( p.use_count() == 3 );
      EXAM_CHECK( *internal_b == 3 );

      *internal_b = 7;

      EXAM_CHECK( *internal_a == 7 );
      EXAM_CHECK( *internal_b == 7 );
    }
    EXAM_CHECK( p.use_count() == 2 );
  }

  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( *p->b == 7 );

  return EXAM_RESULT;
}

int EXAM_IMPL(shared_ptr_test::convert)
{
  struct A
  {
      A() :
          b(3)
        { }

      int b;
  };

  struct B :
    public A
  {
      B() :
          c(4)
        { }

      int c;
  };

  shared_ptr<B> p( new B() );

  EXAM_CHECK( p.use_count() == 1 );
  EXAM_CHECK( p->b == 3 );
  EXAM_CHECK( p->c == 4 );

  shared_ptr<A> pa( p );

  EXAM_CHECK( pa->b == 3 );

  p->b = 2;

  EXAM_CHECK( pa->b == 2 );

  // shared_ptr<A> pa2( new A() );
  // shared_ptr<B> pb2( pa2 ); // <<-- compilation fail (should be fail)

  return EXAM_RESULT;
}
