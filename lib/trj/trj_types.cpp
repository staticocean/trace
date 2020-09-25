
#include "trj_types.h"

//------------------------------------------------------------------------------

inline ufp32_t   ufp32_float32 (float32_t float32_data) { return (ufp32_t) { .data = (uint32_t) (1E+06 * (float64_t) float32_data) }; }
inline ufp32_t   ufp32_float64 (float64_t float64_data) { return (ufp32_t) { .data = (uint32_t) (1E+06 * (float64_t) float64_data) }; }
inline ufp64_t   ufp64_float32 (float32_t float32_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float32_data) }; }
inline ufp64_t   ufp64_float64 (float64_t float64_data) { return (ufp64_t) { .data = (uint64_t) (1E+12 * (float64_t) float64_data) }; }

inline fp32_t    fp32_float32  (float32_t float32_data) { return (fp32_t ) { .data = (int32_t ) (1E+06 * (float64_t) float32_data) }; }
inline fp32_t    fp32_float64  (float64_t float64_data) { return (fp32_t ) { .data = (int32_t ) (1E+06 * (float64_t) float64_data) }; }
inline fp64_t    fp64_float32  (float32_t float32_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float32_data) }; }
inline fp64_t    fp64_float64  (float64_t float64_data) { return (fp64_t ) { .data = (int64_t ) (1E+12 * (float64_t) float64_data) }; }

inline float32_t float32_ufp32 (ufp32_t ufp32_data) { return (float32_t) (1E-06 * (float64_t) ufp32_data.data); }
inline float64_t float64_ufp32 (ufp32_t ufp32_data) { return (float64_t) (1E-06 * (float64_t) ufp32_data.data); }
inline float32_t float32_ufp64 (ufp64_t ufp64_data) { return (float32_t) (1E-12 * (float64_t) ufp64_data.data); }
inline float64_t float64_ufp64 (ufp64_t ufp64_data) { return (float64_t) (1E-12 * (float64_t) ufp64_data.data); }

inline float32_t float32_fp32  (fp32_t  fp32_data ) { return (float32_t) (1E-06 * (float64_t) fp32_data.data ); }
inline float64_t float64_fp32  (fp32_t  fp32_data ) { return (float64_t) (1E-06 * (float64_t) fp32_data.data ); }
inline float32_t float32_fp64  (fp64_t  fp64_data ) { return (float32_t) (1E-12 * (float64_t) fp64_data.data ); }
inline float64_t float64_fp64  (fp64_t  fp64_data ) { return (float64_t) (1E-12 * (float64_t) fp64_data.data ); }

//------------------------------------------------------------------------------

#define UnsignedToFloat(u)         (((long double) ((long) (u - 2147483647L - 1))) + 2147483648.0)

//------------------------------------------------------------------------------















