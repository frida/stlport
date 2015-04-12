#ifndef __stl_config__qnx_h
#define __stl_config__qnx_h

#define _STLP_PLATFORM "QNX"

#ifndef _STLP_USE_STDIO_IO
#  define _STLP_USE_UNIX_IO
#endif

#define _STLP_USE_DEFAULT_FILE_OFFSET

#if !defined(_STLP_NO_THREADS) && !defined(_REENTRANT)
#  define _REENTRANT
#endif

#if defined(_REENTRANT) && !defined(_PTHREADS)
#  define _PTHREADS
#endif

#if defined(_PTHREADS)
#  define _STLP_THREADS
#  define _STLP_PTHREADS
#endif

#endif /* __stl_config__qnx_h */
