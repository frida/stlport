#include "reference_wrapper_test.h"

#include <functional>
#include <type_traits>

using namespace std;

int EXAM_IMPL(ref_wrapper_test::ref)
{
  typedef reference_wrapper<int> rr_type;

  EXAM_CHECK( (is_convertible<rr_type, int&>::value) );
  EXAM_CHECK( (is_same<rr_type::type, int>::value) );

  int i = 1;
  int j = 2;

  rr_type r1 = std::ref(i);

  EXAM_CHECK( r1.get() == 1 );

  r1 = std::ref(j);

  EXAM_CHECK( r1.get() == 2 );

  i = 3;

  EXAM_CHECK( r1.get() == 2 );

  j = 4;

  EXAM_CHECK( r1.get() == 4 );

  r1.get() = 5;

  EXAM_CHECK( j == 5 );

  return EXAM_RESULT;
}

int EXAM_IMPL(ref_wrapper_test::cref)
{
  typedef reference_wrapper<const int> crr_type;

  EXAM_CHECK( (is_convertible<crr_type, const int&>::value) );
  EXAM_CHECK( (is_same<crr_type::type, const int>::value) );

  int i = 1;
  int j = 2;

  crr_type r1 = std::cref(i);

  EXAM_CHECK( r1.get() == 1 );

  r1 = std::cref(j);

  EXAM_CHECK( r1.get() == 2 );

  i = 3;

  EXAM_CHECK( r1.get() == 2 );

  j = 4;

  EXAM_CHECK( r1.get() == 4 );

  return EXAM_RESULT;
}

bool bf1()
{ return true; }

short sf0()
{ return 1; }

short sf1( long )
{ return 0; }

int EXAM_IMPL(ref_wrapper_test::types)
{
  typedef bool (&PF1)();
  typedef bool (F0)();
  typedef short (*PF0)();
  typedef short (*PF2)(long);
  typedef long  (*PF3)(double, const int&);

  struct S
  {
      operator PF2() const
        { return &sf1; }
      double operator()(char, const int&)
        { return 0.0; }
      double operator()(char, int&)
        { return 0.0; }
      void fn(long) const
        { }
      int fi(long)
        { return 1; }
      int fic(long) const
        { return 1; }
      int fiv(long) volatile
        { return 1; }
      int ficv(long) const volatile
        { return 1; }
      char data;
  };

  typedef int (S::*PMF)(long);
  typedef int (S::*PMFc)(long) const;
  typedef int (S::*PMFv)(long) volatile;
  typedef int (S::*PMFcv)(long) const volatile;

  struct D {
      typedef int result_type;
      typedef double argument_type;
      typedef char* first_argument_type;
      typedef short& second_argument_type;
  };

  int i = 1;

  reference_wrapper<int> r( i );

#if 0
  PF1 rf1 = bf1; 
  // reference_wrapper<PF1> rf( bf1 ); // wrong, PF1 is reference
  // reference_wrapper<PF1> rf( rf1 ); // wrong, PF1 is reference
  reference_wrapper<F0> rf00( rf1 ); // ok
  reference_wrapper<F0> rf01( bf1 ); // ok
  // F0 not treated as functor...
#endif
  // EXAM_CHECK((is_same<reference_wrapper<F0>::result_type,bool>::value));

  PF2 rf2 = &sf1;
  // reference_wrapper<PF2> rf3( &sf1 ); // wrong, reference to temporary
  reference_wrapper<PF2> rf4( rf2 ); // ok

  // reference_wrapper<PF1>::result_type q;
  // reference_wrapper<PF0>::result_type q;
  EXAM_CHECK((is_same<reference_wrapper<PF0>::result_type,short>::value));
  EXAM_CHECK((is_same<reference_wrapper<PF2>::argument_type,long>::value));

  EXAM_CHECK((is_same<reference_wrapper<PF3>::first_argument_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<PF3>::second_argument_type,const int&>::value));

  S s;

  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::result_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::first_argument_type,char>::value));
  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::second_argument_type,const int&>::value));

  reference_wrapper<S&(char, const int&)> rs1(S());

  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::result_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::first_argument_type,char>::value));
  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::second_argument_type,const int&>::value));

  reference_wrapper<S(char, const int&)> rs2(S());

  PMF pmf = &S::fi;
  reference_wrapper<PMF> pmfr( pmf );

  EXAM_CHECK((is_same<reference_wrapper<PMF>::result_type,int>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::argument_type,S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::first_argument_type,S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::second_argument_type,long>::value));

  PMFc pmfc = &S::fic;
  EXAM_CHECK((is_same<reference_wrapper<PMFc>::argument_type,const S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMFv>::argument_type,volatile S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMFcv>::argument_type,const volatile S*>::value));

  EXAM_CHECK((is_same<reference_wrapper<D>::result_type,int>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::argument_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::first_argument_type,char*>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::second_argument_type,short&>::value));

  return EXAM_RESULT;
}
