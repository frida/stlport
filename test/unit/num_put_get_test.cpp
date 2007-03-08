#include <limits>

#if !defined (STLPORT) || !defined (_STLP_USE_NO_IOSTREAMS)
#  include <iomanip>
#  include <string>
#  include <sstream>
#  include <cstdio>
#include <iostream>

#  include "complete_digits.h"
#  include "cppunit/cppunit_proxy.h"

#  if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#  endif

//
// TestCase class
//
class NumPutGetTest : public CPPUNIT_NS::TestCase
{
  CPPUNIT_TEST_SUITE(NumPutGetTest);
  CPPUNIT_TEST(num_put_float);
  CPPUNIT_TEST(num_put_integer);
  CPPUNIT_TEST(num_get_float);
  CPPUNIT_TEST(num_get_integer);
  CPPUNIT_TEST(inhex);
  CPPUNIT_TEST(pointer);
  CPPUNIT_TEST(fix_float_long);
  CPPUNIT_TEST_SUITE_END();

private:
  void num_put_float();
  void num_put_integer();
  void num_get_float();
  void num_get_integer();
  void inhex();
  void pointer();
  void fix_float_long();

  static bool check_float(float val, float ref)
  {
    float epsilon = numeric_limits<float>::epsilon();
    return val <= ref + epsilon && val >= ref - epsilon;
  }

  static string reset_stream(ostringstream &ostr)
  {
    string tmp = ostr.str();
    ostr.str("");
    return tmp;
  }

#if !defined (STLPORT) || !defined (_STLP_NO_MEMBER_TEMPLATES)
  template <class F>
  void check_get_float( F v )
  {
    F in_val_d = v;
    {
      stringstream str;

      str << "1E+" << numeric_limits<F>::max_exponent10;

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      CPPUNIT_ASSERT( in_val_d != numeric_limits<F>::infinity() );
    }
    {
      stringstream str;

      str << "-1E+" << numeric_limits<F>::max_exponent10;

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      CPPUNIT_ASSERT( in_val_d != -numeric_limits<F>::infinity() ); // may be problems with portability
      // CPPUNIT_ASSERT( in_val_d != numeric_limits<F>::quiet_NaN() );
    }
    {
      stringstream str;

      str << "1E" << numeric_limits<F>::min_exponent10;

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      CPPUNIT_ASSERT( in_val_d != F(0.0) );
    }
    {
      stringstream str;

      str << "1E+" << (numeric_limits<F>::max_exponent10 + 1);

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      CPPUNIT_ASSERT( in_val_d == numeric_limits<F>::infinity() );
    }
    {
      stringstream str;

      str << "-1E+" << (numeric_limits<F>::max_exponent10 + 1);

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      CPPUNIT_ASSERT( in_val_d == -numeric_limits<F>::infinity() ); // may be problems with portability
    }
    {
      stringstream str;

      str << "1E" << (numeric_limits<F>::min_exponent10 - 1);

      str >> in_val_d;
      CPPUNIT_ASSERT(!str.fail());
      CPPUNIT_ASSERT(str.eof());
      // The next is due to float, that calculated as double and converted to float
      // on last stage:
      CPPUNIT_ASSERT( in_val_d >= F(0.0) && in_val_d <= numeric_limits<F>::min() ); 
    }
  }
#else
#  define __check_get_float( F ) \
  void check_get_float( F v ) \
  { \
    F in_val_d = v; \
    { \
      stringstream str; \

      str << "1E+" << numeric_limits<F>::max_exponent10; \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d != numeric_limits<F>::infinity() ); \
    } \
    { \
      stringstream str; \
 \
      str << "-1E+" << numeric_limits<F>::max_exponent10; \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d != -numeric_limits<F>::infinity() ); \
    } \
    { \
      stringstream str; \
 \
      str << "1E" << numeric_limits<F>::min_exponent10; \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d != F(0.0) ); \
    } \
    { \
      stringstream str; \
 \
      str << "1E+" << (numeric_limits<F>::max_exponent10 + 1); \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d == numeric_limits<F>::infinity() ); \
    } \
    { \
      stringstream str; \
 \
      str << "-1E+" << (numeric_limits<F>::max_exponent10 + 1); \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d == -numeric_limits<F>::infinity() ); \
    } \
    { \
      stringstream str; \
 \
      str << "1E" << (numeric_limits<F>::min_exponent10 - 1); \
 \
      str >> in_val_d; \
      CPPUNIT_ASSERT(!str.fail()); \
      CPPUNIT_ASSERT(str.eof()); \
      CPPUNIT_ASSERT( in_val_d >= F(0.0) && in_val_d <= numeric_limits<F>::min() ); \
    } \
  }

  __check_get_float( float )
  __check_get_float( double )
#  if !defined (STLPORT) || !defined (_STLP_NO_LONG_DOUBLE)
  __check_get_float( long double )
#  endif
#  undef __check_get_float
#endif // _STLP_NO_MEMBER_TEMPLATES
};

CPPUNIT_TEST_SUITE_REGISTRATION(NumPutGetTest);

#if defined (_MSC_VER)
#  pragma warning (disable : 4056)
#  pragma warning (disable : 4756)
#endif

//
// tests implementation
//
void NumPutGetTest::num_put_float()
{
  {
    ostringstream ostr;
    string output;

    ostr.precision(6);
    float test_val = (float)1.23457e+17;

    ostr << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    string digits = "17";
    complete_digits(digits);
    CPPUNIT_ASSERT(output == string("1.23457e+") + digits );

    ostr << fixed << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output.size() == 25);
    CPPUNIT_ASSERT(output.substr(0, 5) == "12345");
    CPPUNIT_ASSERT(output.substr(18) == ".000000");

    ostr << showpos << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output.size() == 26);
    CPPUNIT_ASSERT(output.substr(0, 6) == "+12345");
    CPPUNIT_ASSERT(output.substr(19) == ".000000");

    ostr << setprecision(100) << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output.size() == 120);
    CPPUNIT_ASSERT(output.substr(0, 6) == "+12345");
    CPPUNIT_ASSERT(output.substr(19) == ".0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000" );

    ostr << noshowpos << scientific;
    CPPUNIT_ASSERT(ostr.good());

    test_val = 0.12345678f;
    ostr << setprecision(8) << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    digits = "1";
    complete_digits(digits);
    CPPUNIT_ASSERT(output == string("1.2345678e-") + digits );

    ostr << setw(30) << fixed << setfill('0') << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output == "000000000000000000000.12345678");

    ostr << setw(30) << showpos << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output == "0000000000000000000+0.12345678");

    ostr << setw(30) << left << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output == "+0.123456780000000000000000000");

    ostr << setw(30) << internal << test_val;
    CPPUNIT_ASSERT(ostr.good());
    output = reset_stream(ostr);
    CPPUNIT_ASSERT(output == "+00000000000000000000.12345678");
  }

  {
    ostringstream str;

    str.setf(ios::fixed, ios::floatfield);
    str << 1.0e+5;
    // cerr << str.str() << endl;
    CPPUNIT_CHECK( str.str() == "100000.000000" );

    reset_stream(str);
    str.precision(0);
    str << 1.0e+5;
    CPPUNIT_CHECK( str.str() == "100000" );

    reset_stream(str);
    str.precision(4);
    str << 1.0e+5;
    CPPUNIT_CHECK( str.str() == "100000.0000" );

    reset_stream(str);
    str.precision(0);
    // str << 1.0e+83;
    // CPPUNIT_CHECK( str.str() == "100000000000000000000000000000000000000000000000000000000000000000000000000000000000" );
    reset_stream(str);
    str.precision(0);
    // str << 1.0e+22; // 1.0e+23 critical for fcvt_r on Linux
    // CPPUNIT_CHECK( str.str() == "10000000000000000000000" );

    // cerr.setf(ios::fixed, ios::floatfield);
    // cerr << DBL_MAX << endl;
    // cerr << 1.0e+37 << endl;
  }
}

#define CHECK_COMPLETE(type, val, base, showbase, showpos, casing, width, adjust, expected) \
{ \
  type tmp = val; \
  ostringstream ostr; \
  ostr << base << showbase << showpos << casing << setw(width) << adjust << tmp; \
  CPPUNIT_CHECK( ostr.str() == expected ); \
}

#define CHECK(type, val, base, expected) \
  CHECK_COMPLETE(type, val, base, noshowbase, noshowpos, nouppercase, 0, right, expected)

void NumPutGetTest::num_put_integer()
{
  //octal outputs
  {
    CHECK(short, 0, oct, "0")
    CHECK(short, 1, oct, "1")
    CHECK(short, 12345, oct, "30071")
    if (sizeof(short) == 2) {
      CHECK(short, -1, oct, "177777")
      CHECK(short, -12345, oct, "147707")
    }

    CHECK(unsigned short, 0, oct, "0")
    CHECK(unsigned short, 12345, oct, "30071")

    CHECK(int, 0, oct, "0")
    CHECK(int, 12345678, oct, "57060516")
    if (sizeof(int) == 4) {
      CHECK(int, -1, oct, "37777777777")
      CHECK(int, -12345678, oct, "37720717262")
    }

    CHECK(unsigned int, 0, oct, "0")
    CHECK(unsigned int, 12345678, oct, "57060516")

    CHECK(long, 0, oct, "0")
    CHECK(long, 12345678, oct, "57060516")
    if (sizeof(long) == 4) {
      CHECK(long, -1, oct, "37777777777")
      CHECK(long, -12345678, oct, "37720717262")
    }

    CHECK(unsigned long, 0, oct, "0")
    CHECK(unsigned long, 12345678, oct, "57060516")

#if defined (STLPORT) && defined (_STLP_LONG_LONG)
    CHECK(_STLP_LONG_LONG, 0, oct, "0")
    CHECK(_STLP_LONG_LONG, 12345678, oct, "57060516")
    if (sizeof(_STLP_LONG_LONG) == 8) {
      CHECK(_STLP_LONG_LONG, -1, oct, "1777777777777777777777")
      CHECK(_STLP_LONG_LONG, -12345678, oct, "1777777777777720717262")
    }

    CHECK(unsigned _STLP_LONG_LONG, 0, oct, "0")
    CHECK(unsigned _STLP_LONG_LONG, 12345678, oct, "57060516")
#endif

    //Even with showbase, 0 value gives "0" (see printf documentation)
    CHECK_COMPLETE(short, 0, oct, showbase, noshowpos, nouppercase, 0, right, "0")
    CHECK_COMPLETE(short, 0, oct, showbase, showpos, nouppercase, 6, right, "     0")

    CHECK_COMPLETE(short, 1, oct, showbase, noshowpos, nouppercase, 6, right, "    01")
    CHECK_COMPLETE(short, 1, oct, showbase, noshowpos, nouppercase, 6, left, "01    ")
    CHECK_COMPLETE(short, 1, oct, showbase, showpos, nouppercase, 6, internal, "    01")
  }

  //decimal outputs
  {
    CHECK(short, 0, dec, "0")
    CHECK(short, -1, dec, "-1")
    CHECK(short, 12345, dec, "12345")
    CHECK(short, -12345, dec, "-12345")

    CHECK(unsigned short, 0, dec, "0")
    CHECK(unsigned short, 12345, dec, "12345")

    CHECK(int, 0, dec, "0")
    CHECK(int, -1, dec, "-1")
    CHECK(int, 12345678, dec, "12345678")
    CHECK(int, -12345678, dec, "-12345678")

    CHECK(unsigned int, 0, dec, "0")
    CHECK(unsigned int, 12345678, dec, "12345678")

    CHECK(long, 0, dec, "0")
    CHECK(long, -1, dec, "-1")
    CHECK(long, 12345678, dec, "12345678")
    CHECK(long, -12345678, dec, "-12345678")

    CHECK(unsigned long, 0, dec, "0")
    CHECK(unsigned long, 12345678, dec, "12345678")
#if defined (STLPORT) && defined (_STLP_LONG_LONG)
    CHECK(_STLP_LONG_LONG, 0, dec, "0")
    CHECK(_STLP_LONG_LONG, -1, dec, "-1")
    CHECK(_STLP_LONG_LONG, 12345678, dec, "12345678")
    CHECK(_STLP_LONG_LONG, -12345678, dec, "-12345678")

    CHECK(unsigned _STLP_LONG_LONG, 0, dec, "0")
    CHECK(unsigned _STLP_LONG_LONG, 12345678, dec, "12345678")
#endif

    CHECK_COMPLETE(short, 0, dec, showbase, showpos, nouppercase, 0, right, "+0")
    CHECK_COMPLETE(short, 0, dec, showbase, showpos, nouppercase, 6, right, "    +0")
    CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, right, "    +1")
    CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, left, "+1    ")
    CHECK_COMPLETE(short, 1, dec, showbase, showpos, nouppercase, 6, internal, "+    1")
  }

  //hexadecimal outputs
  {
    CHECK(short, 0, hex, "0")
    CHECK(short, 12345, hex, "3039")
    if (sizeof(short) == 2) {
      CHECK(short, -1, hex, "ffff")
      CHECK(short, -12345, hex, "cfc7")
    }

    CHECK(unsigned short, 0, hex, "0")
    CHECK(unsigned short, 12345, hex, "3039")

    CHECK(int, 0, hex, "0")
    CHECK(int, 12345678, hex, "bc614e")
    if (sizeof(int) == 4) {
      CHECK(int, -1, hex, "ffffffff")
      CHECK(int, -12345678, hex, "ff439eb2")
    }

    CHECK(unsigned int, 0, hex, "0")
    CHECK(unsigned int, 12345678, hex, "bc614e")

    CHECK(long, 0, hex, "0")
    CHECK(long, 12345678, hex, "bc614e")
    if (sizeof(long) == 4) {
      CHECK(long, -1, hex, "ffffffff")
      CHECK(long, -12345678, hex, "ff439eb2")
    }

    CHECK(unsigned long, 0, hex, "0")
    CHECK(unsigned long, 12345678, hex, "bc614e")
#if defined (STLPORT) && defined (_STLP_LONG_LONG)
    CHECK(_STLP_LONG_LONG, 0, hex, "0")
    CHECK(_STLP_LONG_LONG, 12345678, hex, "bc614e")
    if (sizeof(_STLP_LONG_LONG) == 8) {
      CHECK(_STLP_LONG_LONG, -1, hex, "ffffffffffffffff")
      CHECK(_STLP_LONG_LONG, -12345678, hex, "ffffffffff439eb2")
    }

    CHECK(unsigned _STLP_LONG_LONG, 0, hex, "0")
    CHECK(unsigned _STLP_LONG_LONG, 12345678, hex, "bc614e")
#endif

    //Even with showbase, 0 value gives "0" output (see printf documentation)
    CHECK_COMPLETE(short, 0, hex, showbase, showpos, nouppercase, 0, right, "0")
    CHECK_COMPLETE(short, 0, hex, showbase, noshowpos, nouppercase, 6, right, "     0")
    CHECK_COMPLETE(short, 0, hex, showbase, noshowpos, nouppercase, 6, internal, "     0")

    CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, right, "   0x1")
    CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, left, "0x1   ")
    CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, nouppercase, 6, internal, "0x   1")
    CHECK_COMPLETE(short, 1, hex, showbase, noshowpos, uppercase, 6, left, "0X1   ")
    CHECK_COMPLETE(short, 1, hex, showbase, showpos, uppercase, 6, internal, "0X   1")
  }
}

void NumPutGetTest::num_get_float()
{
  float in_val;

  istringstream istr;

  istr.str("1.2345");
  istr >> in_val;
  CPPUNIT_ASSERT(!istr.fail());
  CPPUNIT_ASSERT(istr.eof());
  CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
  istr.clear();

  istr.str("-1.2345");
  istr >> in_val;
  CPPUNIT_ASSERT(!istr.fail());
  CPPUNIT_ASSERT(istr.eof());
  CPPUNIT_ASSERT(check_float(in_val, -1.2345f));
  istr.clear();

  istr.str("+1.2345");
  istr >> in_val;
  CPPUNIT_ASSERT(!istr.fail());
  CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
  istr.clear();

  istr.str("000000000000001.234500000000");
  istr >> in_val;
  CPPUNIT_ASSERT(!istr.fail());
  CPPUNIT_ASSERT(istr.eof());
  CPPUNIT_ASSERT(check_float(in_val, 1.2345f));
  istr.clear();

  istr.str("1.2345e+04");
  istr >> in_val;
  CPPUNIT_ASSERT(!istr.fail());
  CPPUNIT_ASSERT(istr.eof());
  CPPUNIT_ASSERT(check_float(in_val, 12345.0f));
  istr.clear();

  check_get_float( 0.0F );
  check_get_float( 0.0 );
#if !defined (STLPORT) || !defined (_STLP_NO_LONG_DOUBLE)
  check_get_float( 0.0L );
#endif
}

void NumPutGetTest::num_get_integer()
{
  //octal input
  {
    istringstream istr;
    istr.str("30071");
    short val;
    istr >> oct >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 12345 );
    istr.clear();

    if (sizeof(short) == 2) {
      istr.str("177777");
      istr >> oct >> val;
      CPPUNIT_ASSERT( !istr.fail() );
      CPPUNIT_ASSERT( istr.eof() );
      CPPUNIT_ASSERT( val == -1 );
      istr.clear();
    }
  }

  //decimal input
  {
    istringstream istr;
    istr.str("10000");
    short val = -1;
    istr >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 10000 );
    istr.clear();

    istr.str("+10000");
    val = -1;
    istr >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 10000 );
    istr.clear();

    if (sizeof(short) == 2) {
      val = -1;
      istr.str("10000000");
      istr >> val;
      CPPUNIT_ASSERT( istr.fail() );
      CPPUNIT_ASSERT( istr.eof() );
      CPPUNIT_ASSERT( val == -1 );
      istr.clear();
    }

    val = -1;
    istr.str("0x0");
    istr >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( !istr.eof() );
    CPPUNIT_ASSERT( val == 0 );
    istr.clear();

    val = -1;
    istr.str("000001");
    istr >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 1 );
    istr.clear();
  }

  //hexadecimal input
  {
    istringstream istr;
    istr.str("3039");
    short val = -1;
    istr >> hex >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 12345 );
    istr.clear();

    istr.str("x3039");
    val = -1;
    istr >> hex >> val;
    CPPUNIT_ASSERT( istr.fail() );
    CPPUNIT_ASSERT( !istr.eof() );
    CPPUNIT_ASSERT( val == -1 );
    istr.clear();

    istr.str("03039");
    val = -1;
    istr >> hex >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 12345 );
    istr.clear();

    istr.str("0x3039");
    istr >> hex >> val;
    CPPUNIT_ASSERT( !istr.fail() );
    CPPUNIT_ASSERT( istr.eof() );
    CPPUNIT_ASSERT( val == 12345 );
    istr.clear();

    if (sizeof(short) == 2) {
      val = -1;
      istr.str("cfc7");
      istr >> hex >> val;
      CPPUNIT_ASSERT( !istr.fail() );
      CPPUNIT_ASSERT( istr.eof() );
      CPPUNIT_ASSERT( val == -12345 );
      istr.clear();
    }
  }
}

void NumPutGetTest::inhex()
{
  {
    ostringstream s;
    s << hex << 0;
    CPPUNIT_CHECK( s.str() == "0" );
  }
  {
    ostringstream s;
    s << hex << 0xff;
    CPPUNIT_CHECK( s.str() == "ff" );
  }
  {
    ostringstream s;
    s << hex << setw( 4 ) << 0xff;
    CPPUNIT_CHECK( s.str() == "  ff" );
  }
  {
    ostringstream s;
    s << hex << setw( 4 ) << 0;
    CPPUNIT_CHECK( s.str() == "   0" );
  }
  {
    ostringstream s;
    s << hex << showbase << 0;
    CPPUNIT_CHECK( s.str() == "0" );
  }
  {
    ostringstream s;
    s << hex << showbase << 0xff;
    CPPUNIT_CHECK( s.str() == "0xff" );
  }
  {
    ostringstream s;
    s << hex << showbase << setw( 4 ) << 0xff;
    CPPUNIT_CHECK( s.str() == "0xff" );
  }
  { // special case for regression (partially duplicate CHECK_COMPLETE above):
    ostringstream s;
    s.setf( ios_base::internal, ios_base::adjustfield );
    s << hex << showbase << setw(8+2) << 0;
    CPPUNIT_CHECK( s.str() == "         0" );
  }
}

void NumPutGetTest::pointer()
{
  // Problem with printing pointer to null

  /*
   * Really C's formatting not help here, due to:
   *
   * p  The argument shall be a pointer to void. The value of
   *    the pointer is converted to a sequence of printable characters,
   *    in an implementation-defined manner.
   */
  {
    /*
    char buf[128];
    void *p = (void *)0xff00;
    sprintf( buf, "%p", p );
    // cerr << buf << endl;
    // Hmmm, I see 0xff00 on box with 32-bits address; pointer like 'unsigned hex'? 
    if ( sizeof( p ) == 2 ) {
      CPPUNIT_ASSERT( strcmp( buf, "0xff00" ) == 0 );
    } else if ( sizeof( p ) == 4 ) {
      CPPUNIT_ASSERT( strcmp( buf, "0x0000ff00" ) == 0 );
    } else if ( sizeof( p ) == 8 ) {
      CPPUNIT_ASSERT( strcmp( buf, "0x000000000000ff00" ) == 0 );
    } else {
      CPPUNIT_CHECK( sizeof( p ) == 2 || sizeof( p ) == 4 || sizeof( p ) == 8 );
    }
    */
  }
  {
    /*
    char buf[128];
    void *p = 0;
    */
    // sprintf( buf, "%p", p );
    /* Cool. "%p" print '(nil)'; "%#x" print '0' */
    // sprintf( buf, "%#x", (unsigned)p );
    // cerr << buf << endl;
  }
  {
    ostringstream s;
    void *p = (void *)0xff00;
    s << p;
    CPPUNIT_ASSERT( s.good() );
    if ( sizeof( p ) == 2 ) {
      CPPUNIT_ASSERT( s.str() == "0xff00" );
    } else if ( sizeof( p ) == 4 ) {
      CPPUNIT_ASSERT( s.str() == "0x0000ff00" ); // this pass
    } else if ( sizeof( p ) == 8 ) {
      CPPUNIT_ASSERT( s.str() == "0x000000000000ff00" );
    } else {
      CPPUNIT_CHECK( sizeof( p ) == 2 || sizeof( p ) == 4 || sizeof( p ) == 8 );
    }
  }
  {
    ostringstream s;
    void *p = 0;
    s << p;
    CPPUNIT_ASSERT( s.good() );
    if ( sizeof( p ) == 2 ) {
      CPPUNIT_ASSERT( s.str() == "0x0000" );
    } else if ( sizeof( p ) == 4 ) {
      CPPUNIT_ASSERT( s.str() == "0x00000000" ); // but this will fail, if follow %p
    } else if ( sizeof( p ) == 8 ) {
      CPPUNIT_ASSERT( s.str() == "0x0000000000000000" );
    } else {
      CPPUNIT_CHECK( sizeof( p ) == 2 || sizeof( p ) == 4 || sizeof( p ) == 8 );
    }
  }
}

void NumPutGetTest::fix_float_long()
{
  ostringstream str;

  str.setf(ios::fixed, ios::floatfield);
  str << 1.0e+5;
  CPPUNIT_CHECK( str.str() == "100000.000000" );

  reset_stream(str);
  str.precision(0);
  str << 1.0e+5;
  CPPUNIT_CHECK( str.str() == "100000" );

  reset_stream(str);
  str.precision(4);
  str << 1.0e+5;
  CPPUNIT_CHECK( str.str() == "100000.0000" );

  reset_stream(str);
  str.precision(0);
  str << 1.0e+83;
  {
    istringstream istr( str.str() );
    double f;
    istr >> f;
    CPPUNIT_CHECK( !istr.fail() );
    if ( int(numeric_limits<double>::digits10) < 83 ) {
      double delta = 1.0;
      for ( int ee = 83 - int(numeric_limits<double>::digits10); ee > 0; --ee ) {
        delta *= 10.0;
      }
      // we may loss some digits here, but not more than mantissa:
      CPPUNIT_CHECK( (f > (1.0e+83 - delta)) && (f < (1.0e+83 + delta)) );
    } else {
      CPPUNIT_CHECK( check_float(f, 1.0e+83) );
    }
  }

#if 0 // #ifndef _STLP_NO_LONG_DOUBLE
  reset_stream(str);
  str.precision(0);
  str << 1.0e+83l;
  {
    istringstream istr( str.str() );
    long double f;
    istr >> f;
    CPPUNIT_CHECK( !istr.fail() );
    if ( int(numeric_limits<long double>::digits10) < 83 ) {
      long double delta = 1.0l;
      for ( int ee = 83 - int(numeric_limits<long double>::digits10); ee > 0; --ee ) {
        delta *= 10.0l;
      }
      // we may loss some digits here, but not more than mantissa:
      cerr << "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" << endl;
      cerr << str.str() << endl;
      cerr << delta << endl;
      cerr << f << endl;
      CPPUNIT_CHECK( (f > (1.0e+83l - delta)) && (f < (1.0e+83l + delta)) );
    } else {
      CPPUNIT_CHECK( check_float(f, 1.0e+83l) );
    }
  }
#endif

  reset_stream(str);
  str.precision(0);
  str << numeric_limits<double>::max();
  {
    istringstream istr( str.str() );
    double f;
    istr >> f;
    CPPUNIT_CHECK( !istr.fail() );
    if ( int(numeric_limits<double>::digits10) < int(numeric_limits<double>::max_exponent10) ) {
      double delta = 9.0;
      for ( int ee = int(numeric_limits<double>::max_exponent10) - int(numeric_limits<double>::digits10); ee > 0; --ee ) {
        delta *= 10.0;
      }
      // we may loss some digits here, but not more than mantissa:
      CPPUNIT_CHECK( (f > (numeric_limits<double>::max() - delta)) );
    }
  }

#if 0 // #ifndef _STLP_NO_LONG_DOUBLE
  reset_stream(str);
  str.precision(0);
  str << numeric_limits<long double>::max();
  {
    istringstream istr( str.str() );
    long double f;
    istr >> f;
    CPPUNIT_CHECK( !istr.fail() );
    if ( int(numeric_limits<long double>::digits10) < int(numeric_limits<long double>::max_exponent10) ) {
      long double delta = 1.0l;
      for ( int ee = int(numeric_limits<long double>::max_exponent10) - int(numeric_limits<long double>::digits10); ee > 0; --ee ) {
        delta *= 10.0l;
      }
      // we may loss some digits here, but not more than mantissa:
      CPPUNIT_CHECK( (f > (numeric_limits<long double>::max() - delta)) );
    }
  }
#endif
}

#endif

