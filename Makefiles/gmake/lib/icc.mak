# -*- makefile -*- Time-stamp: <08/06/12 15:00:07 ptr>
#
# Copyright (c) 1997-1999, 2002, 2003, 2005-2008
# Petr Ovtchenkov
#
# This material is provided "as is", with absolutely no warranty expressed
# or implied. Any use is at your own risk.
#
# Permission to use or copy this software for any purpose is hereby granted
# without fee, provided the above notices are retained on all copies.
# Permission to modify the code and to distribute modified code is granted,
# provided the above notices are retained, and a notice that the code was
# modified is included with the above copyright notice.
#

OPT += -KPIC

ifeq ($(OSNAME),linux)
dbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_DBGxx)
stldbg-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAME_STLDBGxx)
release-shared:	LDFLAGS += -shared -Wl,-h$(SO_NAMExx)
endif

