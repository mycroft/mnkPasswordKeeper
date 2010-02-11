#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#if _LITTLE_ENDIAN || __LITTLE_ENDIAN || __LITTLE_ENDIAN__ || __i386__ || __alpha__ || _M_IX86
/* i386 / Alpha is little endian */
#define L_ENDIAN
#else
/* Sun OS / HPPA are big endian */
#define B_ENDIAN
#endif

#endif
