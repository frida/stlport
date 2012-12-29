#ifndef _STLP_PTRS_SPECIALIZE_H
#define _STLP_PTRS_SPECIALIZE_H

#  define _STLP_POINTERS_SPECIALIZE( _TpP )
#  define _STLP_DEFINE_ARROW_OPERATOR  pointer operator->() const { return &(operator*()); }

#endif
