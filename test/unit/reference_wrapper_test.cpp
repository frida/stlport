#include "reference_wrapper_test.h"

#include <functional>
#include <type_traits>

using namespace std;

namespace ref_wrapper_ns {

bool bf1()
{ return true; }

short sf0()
{ return 1; }

short sf1( long )
{ return 0; }

short sf2( long )
{ return 1; }

typedef bool (&PF1)();
typedef bool (F0)();
typedef short (*PF0)();
typedef short (*PF2)(long);
typedef long  (*PF3)(double, const int&);

struct S
{
    operator PF2() const
      { return &sf1; }
    double operator()(char c, const int& i)
      { data = c; data2 = static_cast<const long&>(i); return 0.0; }
//    double fd(char, const int&)
//      { return 0.0; }
    double operator()(char c, int& i )
      { data = c; i = static_cast<int>(data2); return 2.0; }
    void fn(long) const
      { }
    int fi(long)
      { return 1; }
    int fi2(long)
      { return 5; }
    int fi3( long v )
      { data2 = v; return 0; }
    int fic(long) const
      { return 2; }
    int fiv(long) volatile
      { return 3; }
    int ficv(long) const volatile
      { return 4; }
    char data;
    long data2;
};

typedef int (S::*PMF)(long);
typedef int (S::*PMFc)(long) const;
typedef int (S::*PMFv)(long) volatile;
typedef int (S::*PMFcv)(long) const volatile;
typedef char S::*PMD;

typedef double (S::*dPMF)(char, const int&);

S func(char, const int&)
{ return S(); }

typedef S (*GPF)(char, const int&);

struct D {
    typedef int result_type;
    typedef double argument_type;
    typedef char* first_argument_type;
    typedef short& second_argument_type;
};

} // ref_wrapper_ns

int EXAM_IMPL(ref_wrapper_test::ctors)
{
  using namespace ref_wrapper_ns;

  int i = 1;

  reference_wrapper<int> r( i );

#if 0
  PF1 rf1 = bf1;
  // reference_wrapper<PF1> rf( bf1 ); // wrong, PF1 is reference
  // reference_wrapper<PF1> rf( rf1 ); // wrong, PF1 is reference
  reference_wrapper<F0> rf00( rf1 ); // ok
  reference_wrapper<F0> rf01( bf1 ); // ok
#endif

  PF2 rf2 = &sf1;
  // reference_wrapper<PF2> rf3( &sf1 ); // wrong, reference to temporary
  reference_wrapper<PF2> rf4( rf2 ); // ok
  reference_wrapper<S&(char, const int&)> rs1(S());
  reference_wrapper<S(char, const int&)> rs2(S());

  PMF pmf = &S::fi;

  reference_wrapper<PMF> pmfr( pmf );
  PMFc pmfc = &S::fic;

  PMD pmd = &S::data;
  reference_wrapper<PMD> pmdr( pmd );

  return EXAM_RESULT;
}

int EXAM_IMPL(ref_wrapper_test::ref)
{
  using namespace ref_wrapper_ns;

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

  // reference_wrapper<F0> rf0 = std::ref( bf1 );
  PF1 rf = bf1;

  // reference_wrapper<F0> rf0 = std::ref( rf );

  PF2 f2 = &sf1;
  PF2 f3 = &sf2;

  reference_wrapper<PF2> rf2 = std::ref( f2 );
  reference_wrapper<PF2> rf3 = std::ref( f3 );

  EXAM_CHECK( rf2(0L) == 0 );
  EXAM_CHECK( rf3(0L) == 1 );

  rf2 = rf3;

  EXAM_CHECK( rf2(0L) == 1 );

  S s;

  PMD pmd = &S::data;
  reference_wrapper<PMD> rd1 = std::ref( pmd );

  s.data = 'y';
  s.*rd1.get() = 'n';

  EXAM_CHECK( s.data == 'n' );

  return EXAM_RESULT;
}

int EXAM_IMPL(ref_wrapper_test::cref)
{
  using namespace ref_wrapper_ns;

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

  // reference_wrapper<F0> rf0 = std::cref( bf1 );

  return EXAM_RESULT;
}

int EXAM_IMPL(ref_wrapper_test::types)
{
  using namespace ref_wrapper_ns;

  EXAM_CHECK((is_same<reference_wrapper<F0>::result_type,bool>::value));

  EXAM_CHECK((is_same<reference_wrapper<PF0>::result_type,short>::value));
  EXAM_CHECK((is_same<reference_wrapper<PF2>::argument_type,long>::value));

  EXAM_CHECK((is_same<reference_wrapper<PF3>::first_argument_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<PF3>::second_argument_type,const int&>::value));

  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::result_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::first_argument_type,char>::value));
  EXAM_CHECK((is_same<reference_wrapper<S&(char, const int&)>::second_argument_type,const int&>::value));

  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::result_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::first_argument_type,char>::value));
  EXAM_CHECK((is_same<reference_wrapper<S(char, const int&)>::second_argument_type,const int&>::value));

  EXAM_CHECK((is_same<reference_wrapper<PMF>::result_type,int>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::argument_type,S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::first_argument_type,S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMF>::second_argument_type,long>::value));

  EXAM_CHECK((is_same<reference_wrapper<PMFc>::argument_type,const S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMFv>::argument_type,volatile S*>::value));
  EXAM_CHECK((is_same<reference_wrapper<PMFcv>::argument_type,const volatile S*>::value));

  EXAM_CHECK((is_same<reference_wrapper<D>::result_type,int>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::argument_type,double>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::first_argument_type,char*>::value));
  EXAM_CHECK((is_same<reference_wrapper<D>::second_argument_type,short&>::value));

  return EXAM_RESULT;
}

int EXAM_IMPL(ref_wrapper_test::invoke)
{
  using namespace ref_wrapper_ns;

  PF2 f2 = &sf1;
  PF2 f3 = &sf2;

  reference_wrapper<PF2> rf2 = std::ref( f2 );
  reference_wrapper<PF2> rf3 = std::ref( f3 );

  EXAM_CHECK( rf2(0L) == 0 );
  EXAM_CHECK( rf3.operator ()(0L) == 1 );

  S s;

  PMF pmf = &S::fi;

  reference_wrapper<PMF> pmfr( pmf );

  EXAM_CHECK( pmfr(s,2L) == 1 );

  pmfr.get() = &S::fi2;

  EXAM_CHECK( pmfr(s,2L) == 5 );

  pmfr.get() = &S::fi3;

  EXAM_CHECK( pmfr(s,7L) == 0 );
  EXAM_CHECK( s.data2 == 7L );

  EXAM_CHECK( pmfr(&s,8L) == 0 );
  EXAM_CHECK( s.data2 == 8L );

  pmfr.get() = &S::fi;
  EXAM_CHECK( pmfr(S(),0L) == 1 );

  PMFc pmfc = &S::fic;

  reference_wrapper<PMFc> pmfrc( pmfc );

  EXAM_CHECK( pmfrc(s,2L) == 2 );

  PMFv pmfv = &S::fiv;

  reference_wrapper<PMFv> pmfrv( pmfv );

  EXAM_CHECK( pmfrv(s,2L) == 3 );

  PMFcv pmfcv = &S::ficv;

  reference_wrapper<PMFcv> pmfrcv( pmfcv );

  EXAM_CHECK( pmfrcv(s,2L) == 4 );

  // S& sr = s;
  // reference_wrapper<S&(char, const int&)> srr0( s(char, const int& ) );

  // What I can do with it (just below)?
  reference_wrapper<S(char, const int&)> srr( S(char, const int& ) );
 
  //GPF gpf = &func;
  //reference_wrapper<S(char, const int&)> srr0 = std::ref( gpf );

  //srr; // .get() = &S::operator ()(char,const int&); //  .invoke('y',0);
  // int k;
  // EXAM_CHECK( (s.*srr('y',k) == 0.0) );
  // (*srr)(&func); // <- compile, but not link
  // srr(&func); // <- compile, but not link
  // dPMF xx( srr );
  // srr(&func)('y',k);
  // srr('y',k);

  reference_wrapper<S> sr(s);

  EXAM_CHECK( sr('y',10) == 0.0 );
  EXAM_CHECK( s.data == 'y' );
  EXAM_CHECK( s.data2 == 10L );
  EXAM_CHECK( sr('y',10) == 0.0 );

  int z = 4;

  EXAM_CHECK( sr('n',z) == 2.0 );
  EXAM_CHECK( s.data == 'n' );
  EXAM_CHECK( z == 10 );

  PMD pmd = &S::data;
  reference_wrapper<PMD> rd1 = std::ref( pmd );

  s.data = 'y';
 
  EXAM_CHECK( rd1(s) == 'y' );

  // rd1(s) = 'n'; // error: rd1(s) is rvalue; use s.*rd1.get() = 'n';

  return EXAM_RESULT;
}
