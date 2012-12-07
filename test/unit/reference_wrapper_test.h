#ifndef __TEST_REFERENCE_WRAPPER_TEST_H
#define __TEST_REFERENCE_WRAPPER_TEST_H

#define __FIT_EXAM

#include <exam/suite.h>

class ref_wrapper_test
{
  public:
    int EXAM_DECL(ctors); 
    int EXAM_DECL(ref);
    int EXAM_DECL(cref);
    int EXAM_DECL(types);
    int EXAM_DECL(invoke);
};

#endif // __TEST_REFERENCE_WRAPPER_H
