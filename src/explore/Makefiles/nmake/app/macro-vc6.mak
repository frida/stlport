# -*- makefile -*- Time-stamp: <03/09/28 17:34:35 ptr>
# $Id$

!ifndef LDLIBS
LDLIBS =
!endif

LDSEARCH = $(LDSEARCH) /LIBPATH:"$(MSVC_LIB_DIR)"

OPT_DBG = $(OPT_DBG) /MTd
OPT_STLDBG = $(OPT_STLDBG) /MTd
OPT_REL = $(OPT_REL) /MT
OPT_DBG_STATIC = $(OPT_DBG_STATIC) /MTd
OPT_STLDBG_STATIC = $(OPT_STLDBG_STATIC) /MTd
OPT_REL_STATIC = $(OPT_REL_STATIC) /MT

LDFLAGS_REL = $(LDFLAGS_REL) $(LDSEARCH)
LDFLAGS_DBG = $(LDFLAGS_DBG) $(LDSEARCH)
LDFLAGS_STLDBG = $(LDFLAGS_STLDBG) $(LDSEARCH)