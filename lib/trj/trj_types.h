
#ifndef __TYPES_H__
#define __TYPES_H__

//------------------------------------------------------------------------------

#ifndef __TRJ_ENV__
#include <inttypes.h>
#endif

//------------------------------------------------------------------------------

#ifdef __TRJ_ENV__
typedef signed char 			int8_t;
typedef unsigned char 			uint8_t;
typedef signed int 				int32_t;
typedef unsigned int  			uint32_t;
#endif

typedef double	 	float64_t;
typedef float      	float32_t;

typedef struct ufp32 { uint32_t data; } ufp32_t;
typedef struct fp32  { int32_t  data; } fp32_t ;

#ifndef __TRJ_ENV__
typedef struct ufp64 { uint64_t data; } ufp64_t;
typedef struct fp64  { int64_t  data; } fp64_t ;
#endif

//------------------------------------------------------------------------------

inline fp32_t    fp32_float32  (float32_t float32_data) { return (fp32_t ) { .data = (int32_t ) (1E+06 * (float64_t) float32_data) }; }
inline fp32_t    fp32_float64  (float64_t float64_data) { return (fp32_t ) { .data = (int32_t ) (1E+06 * (float64_t) float64_data) }; }
inline ufp32_t   ufp32_float32 (float32_t float32_data) { return (ufp32_t) { .data = (uint32_t) (1E+06 * (float64_t) float32_data) }; }
inline ufp32_t   ufp32_float64 (float64_t float64_data) { return (ufp32_t) { .data = (uint32_t) (1E+06 * (float64_t) float64_data) }; }

inline float32_t float32_fp32  (fp32_t  fp32_data ) { return (float32_t) (1E-06 * (float64_t) fp32_data.data ); }
inline float64_t float64_fp32  (fp32_t  fp32_data ) { return (float64_t) (1E-06 * (float64_t) fp32_data.data ); }
inline float32_t float32_ufp32 (ufp32_t ufp32_data) { return (float32_t) (1E-06 * (float64_t) ufp32_data.data); }
inline float64_t float64_ufp32 (ufp32_t ufp32_data) { return (float64_t) (1E-06 * (float64_t) ufp32_data.data); }

#ifndef __TRJ_ENV__
inline fp64_t    fp64_float32  (float32_t float32_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float32_data) }; }
inline fp64_t    fp64_float64  (float64_t float64_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float64_data) }; }
inline ufp64_t   ufp64_float32 (float32_t float32_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float32_data) }; }
inline ufp64_t   ufp64_float64 (float64_t float64_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float64_data) }; }

inline float32_t float32_fp64  (fp64_t  fp64_data ) { return (float32_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float64_t float64_fp64  (fp64_t  fp64_data ) { return (float64_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float32_t float32_ufp64 (ufp64_t ufp64_data) { return (float32_t) (1E-12 * (float64_t) ufp64_data.data); }
inline float64_t float64_ufp64 (ufp64_t ufp64_data) { return (float64_t) (1E-12 * (float64_t) ufp64_data.data); }
#endif

//------------------------------------------------------------------------------

#endif /* __TYPES_H__ */


