#ifndef CONFIG_H
#define CONFIG_H

#include "Config.hpp"

#define AL_API
#define ALC_API
#define AL_LIBTYPE_STATIC

#if defined(WALO_PLATFORM_WIN32)
#include <mmsystem.h>
#define strncasecmp _stricmp
#define strcasecmp _strcmpi
//#define snprintf _snprintf
//#define timeGetTime GetTickCount
#endif

/* Define to the library version */
#define ALSOFT_VERSION "1.13"

/* Define if we have the ALSA backend */
/* #define HAVE_ALSA */

/* Define if we have the OSS backend */
/* #define HAVE_OSS */

/* Define if we have the Solaris backend */
/* #define HAVE_SOLARIS */

/* Define if we have the SndIO backend */
/*#define HAVE_SNDIO */

/* Define if we have the MMDevApi backend */
/*#define HAVE_MMDEVAPI*/

#if defined(WALO_PLATFORM_WIN32)
/* Define if we have the DSound backend */
#define HAVE_DSOUND
#endif

/* Define if we have the Windows Multimedia backend */
/* #define HAVE_WINMM */

/* Define if we have the PortAudio backend */
/* #define HAVE_PORTAUDIO */

/* Define if we have the PulseAudio backend */
/* #define HAVE_PULSEAUDIO */

#if defined(WALO_PLATFORM_OSX)
#define AL_ALEXT_PROTOTYPES
/* Define if we have the CoreAudio backend */
#define HAVE_COREAUDIO
#endif

#if defined(WALO_PLATFORM_ANDROID)
#define AL_ALEXT_PROTOTYPES
/* Define if we have the OpenSL backend */
#define HAVE_OPENSL
/* Define if we have the OpenSL 1.1 backend */
//#define HAVE_OPENSL_1_1
/* Define if we have the Android backend */
#define HAVE_ANDROID
/* Define if we want to use the low latency Android backend */
#define HAVE_ANDROID_LOW_LATENCY
#endif

/* Define if we have the Wave Writer backend */
/*#define HAVE_WAVE */

/* Define if we have dlfcn.h */
//#define HAVE_DLFCN_H

/* Define if we have the stat function */
#define HAVE_STAT

/* Define if we have the powf function */
#define HAVE_POWF

/* Define if we have the sqrtf function */
#define HAVE_SQRTF

/* Define if we have the cosf function */
#define HAVE_COSF

/* Define if we have the sinf function */
#define HAVE_SINF

/* Define if we have the acosf function */
#define HAVE_ACOSF

/* Define if we have the asinf function */
#define HAVE_ASINF

/* Define if we have the atanf function */
#define HAVE_ATANF

/* Define if we have the atan2f function */
#define HAVE_ATAN2F

/* Define if we have the fabsf function */
#define HAVE_FABSF

/* Define if we have the log10f function */
#define HAVE_LOG10F

/* Define if we have the floorf function */
#define HAVE_FLOORF

/* Define if we have the strtof function */
#define HAVE_STRTOF

/* Define if we have stdint.h */
#define HAVE_STDINT_H

/* Define if we have the __int64 type */
#define HAVE___INT64

/* Define to the size of a long int type */
#define SIZEOF_LONG 4

/* Define to the size of a long long int type */
#define SIZEOF_LONG_LONG 8

#if !defined(WALO_PLATFORM_WIN32)
/* Define if we have GCC's destructor attribute */
#define HAVE_GCC_DESTRUCTOR

/* Define if we have GCC's format attribute */
#define HAVE_GCC_FORMAT
#endif

/* Define if we have pthread_np.h */
//#define HAVE_PTHREAD_NP_H

#if defined(WALO_PLATFORM_ANDROID)

/* Define if we have arm_neon.h */
#define HAVE_ARM_NEON_H

#endif

#if defined(WALO_PLATFORM_WIN32)
/* Define if we have guiddef.h */
#define HAVE_GUIDDEF_H
#endif

/* Define if we have guiddef.h */
//#define HAVE_INITGUID_H

/* Define if we have ieeefp.h */
//#define HAVE_IEEEFP_H

/* Define if we have float.h */
//#define HAVE_FLOAT_H

/* Define if we have fpu_control.h */
//#define HAVE_FPU_CONTROL_H

/* Define if we have fenv.h */
//#define HAVE_FENV_H

/* Define if we have fesetround() */
//#define HAVE_FESETROUND

/* Define if we have _controlfp() */
//#define HAVE__CONTROLFP

/* Define if we have pthread_setschedparam() */
//#define HAVE_PTHREAD_SETSCHEDPARAM

/* Define if we have the restrict keyword */
//#define HAVE_RESTRICT

/* Define if we have the __restrict keyword */
//#define HAVE___RESTRICT

#endif