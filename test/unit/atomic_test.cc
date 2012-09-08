// -*- C++ -*- Time-stamp: <2012-05-28 15:35:09 ptr>

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

#include <atomic>
#include "atomic_test.h"

int EXAM_IMPL(atomic_test::atomic_int)
{
  // int val = -1;

  std::atomic<int> ai;

  ai.store( -1 );

  EXAM_CHECK( ai.value == -1 );
  
  return EXAM_RESULT;
}
