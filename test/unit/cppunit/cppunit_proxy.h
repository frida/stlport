/*
 * Copyright (c) 2003, 2004
 * Zdenek Nemec
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

/* $Id$ */

#ifndef _CPPUNITPROXYINTERFACE_H_
# define _CPPUNITPROXYINTERFACE_H_

#if defined(CPPUNIT_MINI) || defined(CPPUNIT_DUMMY)
#   include "cppunit_mini.h"
#else
#   include <cppunit/TestCase.h>
#   include <cppunit/extensions/HelperMacros.h>
#endif

#endif
