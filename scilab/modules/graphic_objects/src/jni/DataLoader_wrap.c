/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.2.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


#define SWIG_VERSION 0x040201
#define SWIGJAVA

/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)
#  define SWIGTEMPLATEDISAMBIGUATOR template
# elif defined(__HP_aCC)
/* Needed even with `aCC -AA' when `aCC -V' reports HP ANSI C++ B3910B A.03.55 */
/* If we find a maximum version that requires this, the test would be __HP_aCC <= 35500 for A.03.55 */
#  define SWIGTEMPLATEDISAMBIGUATOR template
# else
#  define SWIGTEMPLATEDISAMBIGUATOR
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__))
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__))
# else
#   define SWIGUNUSED
# endif
#endif

#ifndef SWIG_MSC_UNSUPPRESS_4505
# if defined(_MSC_VER)
#   pragma warning(disable : 4505) /* unreferenced local function has been removed */
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* exporting methods */
#if defined(__GNUC__)
#  if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#    ifndef GCC_HASCLASSVISIBILITY
#      define GCC_HASCLASSVISIBILITY
#    endif
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif

/* Deal with Microsoft's attempt at deprecating methods in the standard C++ library */
#if !defined(SWIG_NO_SCL_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_SCL_SECURE_NO_DEPRECATE)
# define _SCL_SECURE_NO_DEPRECATE
#endif

/* Deal with Apple's deprecated 'AssertMacros.h' from Carbon-framework */
#if defined(__APPLE__) && !defined(__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES)
# define __ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES 0
#endif

/* Intel's compiler complains if a variable which was never initialised is
 * cast to void, which is a common idiom which we use to indicate that we
 * are aware a variable isn't used.  So we just silence that warning.
 * See: https://github.com/swig/swig/issues/192 for more discussion.
 */
#ifdef __INTEL_COMPILER
# pragma warning disable 592
#endif

#if defined(__cplusplus) && __cplusplus >=201103L
# define SWIG_NULLPTR nullptr
#else
# define SWIG_NULLPTR NULL
#endif 

/* -----------------------------------------------------------------------------
 * swigcompat.swg
 *
 * Macros to provide support compatibility with older C and C++ standards.
 * ----------------------------------------------------------------------------- */

/* C99 and C++11 should provide snprintf, but define SWIG_NO_SNPRINTF
 * if you're missing it.
 */
#if ((defined __STDC_VERSION__ && __STDC_VERSION__ >= 199901L) || \
     (defined __cplusplus && __cplusplus >= 201103L) || \
     defined SWIG_HAVE_SNPRINTF) && \
    !defined SWIG_NO_SNPRINTF
# define SWIG_snprintf(O,S,F,A) snprintf(O,S,F,A)
# define SWIG_snprintf2(O,S,F,A,B) snprintf(O,S,F,A,B)
#else
/* Fallback versions ignore the buffer size, but most of our uses either have a
 * fixed maximum possible size or dynamically allocate a buffer that's large
 * enough.
 */
# define SWIG_snprintf(O,S,F,A) sprintf(O,F,A)
# define SWIG_snprintf2(O,S,F,A,B) sprintf(O,F,A,B)
#endif


#include <jni.h>
#include <stdlib.h>
#include <string.h>


/* Support for throwing Java exceptions */
typedef enum {
  SWIG_JavaOutOfMemoryError = 1,
  SWIG_JavaIOException,
  SWIG_JavaRuntimeException,
  SWIG_JavaIndexOutOfBoundsException,
  SWIG_JavaArithmeticException,
  SWIG_JavaIllegalArgumentException,
  SWIG_JavaNullPointerException,
  SWIG_JavaDirectorPureVirtual,
  SWIG_JavaUnknownError,
  SWIG_JavaIllegalStateException,
} SWIG_JavaExceptionCodes;

typedef struct {
  SWIG_JavaExceptionCodes code;
  const char *java_exception;
} SWIG_JavaExceptions_t;


static void SWIGUNUSED SWIG_JavaThrowException(JNIEnv *jenv, SWIG_JavaExceptionCodes code, const char *msg) {
  jclass excep;
  static const SWIG_JavaExceptions_t java_exceptions[] = {
    { SWIG_JavaOutOfMemoryError, "java/lang/OutOfMemoryError" },
    { SWIG_JavaIOException, "java/io/IOException" },
    { SWIG_JavaRuntimeException, "java/lang/RuntimeException" },
    { SWIG_JavaIndexOutOfBoundsException, "java/lang/IndexOutOfBoundsException" },
    { SWIG_JavaArithmeticException, "java/lang/ArithmeticException" },
    { SWIG_JavaIllegalArgumentException, "java/lang/IllegalArgumentException" },
    { SWIG_JavaNullPointerException, "java/lang/NullPointerException" },
    { SWIG_JavaDirectorPureVirtual, "java/lang/RuntimeException" },
    { SWIG_JavaUnknownError,  "java/lang/UnknownError" },
    { SWIG_JavaIllegalStateException, "java/lang/IllegalStateException" },
    { (SWIG_JavaExceptionCodes)0,  "java/lang/UnknownError" }
  };
  const SWIG_JavaExceptions_t *except_ptr = java_exceptions;

  while (except_ptr->code != code && except_ptr->code)
    except_ptr++;

  (*jenv)->ExceptionClear(jenv);
  excep = (*jenv)->FindClass(jenv, except_ptr->java_exception);
  if (excep)
    (*jenv)->ThrowNew(jenv, excep, msg);
}


/* Contract support */

#define SWIG_contract_assert(nullreturn, expr, msg) do { if (!(expr)) {SWIG_JavaThrowException(jenv, SWIG_JavaIllegalArgumentException, msg); return nullreturn; } } while (0)


#if defined(SWIG_NOINCLUDE) || defined(SWIG_NOARRAYS)


static int SWIG_JavaArrayInSchar (JNIEnv *jenv, jbyte **jarr, signed char **carr, jbyteArray input);
static void SWIG_JavaArrayArgoutSchar (JNIEnv *jenv, jbyte *jarr, signed char *carr, jbyteArray input);
static jbyteArray SWIG_JavaArrayOutSchar (JNIEnv *jenv, signed char *result, jsize sz);


static int SWIG_JavaArrayInUchar (JNIEnv *jenv, jshort **jarr, unsigned char **carr, jshortArray input);
static void SWIG_JavaArrayArgoutUchar (JNIEnv *jenv, jshort *jarr, unsigned char *carr, jshortArray input);
static jshortArray SWIG_JavaArrayOutUchar (JNIEnv *jenv, unsigned char *result, jsize sz);


static int SWIG_JavaArrayInShort (JNIEnv *jenv, jshort **jarr, short **carr, jshortArray input);
static void SWIG_JavaArrayArgoutShort (JNIEnv *jenv, jshort *jarr, short *carr, jshortArray input);
static jshortArray SWIG_JavaArrayOutShort (JNIEnv *jenv, short *result, jsize sz);


static int SWIG_JavaArrayInUshort (JNIEnv *jenv, jint **jarr, unsigned short **carr, jintArray input);
static void SWIG_JavaArrayArgoutUshort (JNIEnv *jenv, jint *jarr, unsigned short *carr, jintArray input);
static jintArray SWIG_JavaArrayOutUshort (JNIEnv *jenv, unsigned short *result, jsize sz);


static int SWIG_JavaArrayInInt (JNIEnv *jenv, jint **jarr, int **carr, jintArray input);
static void SWIG_JavaArrayArgoutInt (JNIEnv *jenv, jint *jarr, int *carr, jintArray input);
static jintArray SWIG_JavaArrayOutInt (JNIEnv *jenv, int *result, jsize sz);


static int SWIG_JavaArrayInUint (JNIEnv *jenv, jlong **jarr, unsigned int **carr, jlongArray input);
static void SWIG_JavaArrayArgoutUint (JNIEnv *jenv, jlong *jarr, unsigned int *carr, jlongArray input);
static jlongArray SWIG_JavaArrayOutUint (JNIEnv *jenv, unsigned int *result, jsize sz);


static int SWIG_JavaArrayInLong (JNIEnv *jenv, jint **jarr, long **carr, jintArray input);
static void SWIG_JavaArrayArgoutLong (JNIEnv *jenv, jint *jarr, long *carr, jintArray input);
static jintArray SWIG_JavaArrayOutLong (JNIEnv *jenv, long *result, jsize sz);


static int SWIG_JavaArrayInUlong (JNIEnv *jenv, jlong **jarr, unsigned long **carr, jlongArray input);
static void SWIG_JavaArrayArgoutUlong (JNIEnv *jenv, jlong *jarr, unsigned long *carr, jlongArray input);
static jlongArray SWIG_JavaArrayOutUlong (JNIEnv *jenv, unsigned long *result, jsize sz);


static int SWIG_JavaArrayInLonglong (JNIEnv *jenv, jlong **jarr, long long **carr, jlongArray input);
static void SWIG_JavaArrayArgoutLonglong (JNIEnv *jenv, jlong *jarr, long long *carr, jlongArray input);
static jlongArray SWIG_JavaArrayOutLonglong (JNIEnv *jenv, long long *result, jsize sz);


static int SWIG_JavaArrayInFloat (JNIEnv *jenv, jfloat **jarr, float **carr, jfloatArray input);
static void SWIG_JavaArrayArgoutFloat (JNIEnv *jenv, jfloat *jarr, float *carr, jfloatArray input);
static jfloatArray SWIG_JavaArrayOutFloat (JNIEnv *jenv, float *result, jsize sz);


static int SWIG_JavaArrayInDouble (JNIEnv *jenv, jdouble **jarr, double **carr, jdoubleArray input);
static void SWIG_JavaArrayArgoutDouble (JNIEnv *jenv, jdouble *jarr, double *carr, jdoubleArray input);
static jdoubleArray SWIG_JavaArrayOutDouble (JNIEnv *jenv, double *result, jsize sz);


#else


/* signed char[] support */
static int SWIG_JavaArrayInSchar (JNIEnv *jenv, jbyte **jarr, signed char **carr, jbyteArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetByteArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (signed char*) malloc(sz * sizeof(signed char)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (signed char)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutSchar (JNIEnv *jenv, jbyte *jarr, signed char *carr, jbyteArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jbyte)carr[i];
  (*jenv)->ReleaseByteArrayElements(jenv, input, jarr, 0);
}

static jbyteArray SWIG_JavaArrayOutSchar (JNIEnv *jenv, signed char *result, jsize sz) {
  jbyte *arr;
  int i;
  jbyteArray jresult = (*jenv)->NewByteArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetByteArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jbyte)result[i];
  (*jenv)->ReleaseByteArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* unsigned char[] support */
static int SWIG_JavaArrayInUchar (JNIEnv *jenv, jshort **jarr, unsigned char **carr, jshortArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetShortArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (unsigned char*) malloc(sz * sizeof(unsigned char)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (unsigned char)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutUchar (JNIEnv *jenv, jshort *jarr, unsigned char *carr, jshortArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jshort)carr[i];
  (*jenv)->ReleaseShortArrayElements(jenv, input, jarr, 0);
}

static jshortArray SWIG_JavaArrayOutUchar (JNIEnv *jenv, unsigned char *result, jsize sz) {
  jshort *arr;
  int i;
  jshortArray jresult = (*jenv)->NewShortArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetShortArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jshort)result[i];
  (*jenv)->ReleaseShortArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* short[] support */
static int SWIG_JavaArrayInShort (JNIEnv *jenv, jshort **jarr, short **carr, jshortArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetShortArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (short*) malloc(sz * sizeof(short)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (short)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutShort (JNIEnv *jenv, jshort *jarr, short *carr, jshortArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jshort)carr[i];
  (*jenv)->ReleaseShortArrayElements(jenv, input, jarr, 0);
}

static jshortArray SWIG_JavaArrayOutShort (JNIEnv *jenv, short *result, jsize sz) {
  jshort *arr;
  int i;
  jshortArray jresult = (*jenv)->NewShortArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetShortArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jshort)result[i];
  (*jenv)->ReleaseShortArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* unsigned short[] support */
static int SWIG_JavaArrayInUshort (JNIEnv *jenv, jint **jarr, unsigned short **carr, jintArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetIntArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (unsigned short*) malloc(sz * sizeof(unsigned short)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (unsigned short)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutUshort (JNIEnv *jenv, jint *jarr, unsigned short *carr, jintArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jint)carr[i];
  (*jenv)->ReleaseIntArrayElements(jenv, input, jarr, 0);
}

static jintArray SWIG_JavaArrayOutUshort (JNIEnv *jenv, unsigned short *result, jsize sz) {
  jint *arr;
  int i;
  jintArray jresult = (*jenv)->NewIntArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetIntArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jint)result[i];
  (*jenv)->ReleaseIntArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* int[] support */
static int SWIG_JavaArrayInInt (JNIEnv *jenv, jint **jarr, int **carr, jintArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetIntArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (int*) malloc(sz * sizeof(int)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (int)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutInt (JNIEnv *jenv, jint *jarr, int *carr, jintArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jint)carr[i];
  (*jenv)->ReleaseIntArrayElements(jenv, input, jarr, 0);
}

static jintArray SWIG_JavaArrayOutInt (JNIEnv *jenv, int *result, jsize sz) {
  jint *arr;
  int i;
  jintArray jresult = (*jenv)->NewIntArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetIntArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jint)result[i];
  (*jenv)->ReleaseIntArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* unsigned int[] support */
static int SWIG_JavaArrayInUint (JNIEnv *jenv, jlong **jarr, unsigned int **carr, jlongArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetLongArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (unsigned int*) malloc(sz * sizeof(unsigned int)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (unsigned int)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutUint (JNIEnv *jenv, jlong *jarr, unsigned int *carr, jlongArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jlong)carr[i];
  (*jenv)->ReleaseLongArrayElements(jenv, input, jarr, 0);
}

static jlongArray SWIG_JavaArrayOutUint (JNIEnv *jenv, unsigned int *result, jsize sz) {
  jlong *arr;
  int i;
  jlongArray jresult = (*jenv)->NewLongArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetLongArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jlong)result[i];
  (*jenv)->ReleaseLongArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* long[] support */
static int SWIG_JavaArrayInLong (JNIEnv *jenv, jint **jarr, long **carr, jintArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetIntArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (long*) malloc(sz * sizeof(long)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (long)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutLong (JNIEnv *jenv, jint *jarr, long *carr, jintArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jint)carr[i];
  (*jenv)->ReleaseIntArrayElements(jenv, input, jarr, 0);
}

static jintArray SWIG_JavaArrayOutLong (JNIEnv *jenv, long *result, jsize sz) {
  jint *arr;
  int i;
  jintArray jresult = (*jenv)->NewIntArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetIntArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jint)result[i];
  (*jenv)->ReleaseIntArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* unsigned long[] support */
static int SWIG_JavaArrayInUlong (JNIEnv *jenv, jlong **jarr, unsigned long **carr, jlongArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetLongArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (unsigned long*) malloc(sz * sizeof(unsigned long)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (unsigned long)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutUlong (JNIEnv *jenv, jlong *jarr, unsigned long *carr, jlongArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jlong)carr[i];
  (*jenv)->ReleaseLongArrayElements(jenv, input, jarr, 0);
}

static jlongArray SWIG_JavaArrayOutUlong (JNIEnv *jenv, unsigned long *result, jsize sz) {
  jlong *arr;
  int i;
  jlongArray jresult = (*jenv)->NewLongArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetLongArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jlong)result[i];
  (*jenv)->ReleaseLongArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* long long[] support */
static int SWIG_JavaArrayInLonglong (JNIEnv *jenv, jlong **jarr, long long **carr, jlongArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetLongArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (long long*) malloc(sz * sizeof(long long)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (long long)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutLonglong (JNIEnv *jenv, jlong *jarr, long long *carr, jlongArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jlong)carr[i];
  (*jenv)->ReleaseLongArrayElements(jenv, input, jarr, 0);
}

static jlongArray SWIG_JavaArrayOutLonglong (JNIEnv *jenv, long long *result, jsize sz) {
  jlong *arr;
  int i;
  jlongArray jresult = (*jenv)->NewLongArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetLongArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jlong)result[i];
  (*jenv)->ReleaseLongArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* float[] support */
static int SWIG_JavaArrayInFloat (JNIEnv *jenv, jfloat **jarr, float **carr, jfloatArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetFloatArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (float*) malloc(sz * sizeof(float)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (float)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutFloat (JNIEnv *jenv, jfloat *jarr, float *carr, jfloatArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jfloat)carr[i];
  (*jenv)->ReleaseFloatArrayElements(jenv, input, jarr, 0);
}

static jfloatArray SWIG_JavaArrayOutFloat (JNIEnv *jenv, float *result, jsize sz) {
  jfloat *arr;
  int i;
  jfloatArray jresult = (*jenv)->NewFloatArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetFloatArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jfloat)result[i];
  (*jenv)->ReleaseFloatArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


/* double[] support */
static int SWIG_JavaArrayInDouble (JNIEnv *jenv, jdouble **jarr, double **carr, jdoubleArray input) {
  int i;
  jsize sz;
  if (!input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "null array");
    return 0;
  }
  sz = (*jenv)->GetArrayLength(jenv, input);
  *jarr = (*jenv)->GetDoubleArrayElements(jenv, input, 0);
  if (!*jarr)
    return 0; 
  *carr = (double*) malloc(sz * sizeof(double)); 
  if (!*carr) {
    SWIG_JavaThrowException(jenv, SWIG_JavaOutOfMemoryError, "array memory allocation failed");
    return 0;
  }
  for (i=0; i<sz; i++)
    (*carr)[i] = (double)(*jarr)[i];
  return 1;
}

static void SWIG_JavaArrayArgoutDouble (JNIEnv *jenv, jdouble *jarr, double *carr, jdoubleArray input) {
  int i;
  jsize sz = (*jenv)->GetArrayLength(jenv, input);
  for (i=0; i<sz; i++)
    jarr[i] = (jdouble)carr[i];
  (*jenv)->ReleaseDoubleArrayElements(jenv, input, jarr, 0);
}

static jdoubleArray SWIG_JavaArrayOutDouble (JNIEnv *jenv, double *result, jsize sz) {
  jdouble *arr;
  int i;
  jdoubleArray jresult = (*jenv)->NewDoubleArray(jenv, sz);
  if (!jresult)
    return NULL;
  arr = (*jenv)->GetDoubleArrayElements(jenv, jresult, 0);
  if (!arr)
    return NULL;
  for (i=0; i<sz; i++)
    arr[i] = (jdouble)result[i];
  (*jenv)->ReleaseDoubleArrayElements(jenv, jresult, arr, 0);
  return jresult;
}


#endif


  #include "DataLoader.hxx"


#ifdef __cplusplus
extern "C" {
#endif

SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureWidth(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getTextureWidth(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureHeight(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getTextureHeight(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillTextureData(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3) {
  jint jresult = 0 ;
  int arg1 ;
  unsigned char *arg2 = (unsigned char *) 0 ;
  int arg3 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  result = (int)fillTextureData(arg1,arg2,arg3);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillSubTextureData(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3, jint jarg4, jint jarg5, jint jarg6, jint jarg7) {
  jint jresult = 0 ;
  int arg1 ;
  unsigned char *arg2 = (unsigned char *) 0 ;
  int arg3 ;
  int arg4 ;
  int arg5 ;
  int arg6 ;
  int arg7 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  arg4 = (int)jarg4; 
  arg5 = (int)jarg5; 
  arg6 = (int)jarg6; 
  arg7 = (int)jarg7; 
  result = (int)fillSubTextureData(arg1,arg2,arg3,arg4,arg5,arg6,arg7);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getDataSize(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getDataSize(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillVertices(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3, jint jarg4, jint jarg5, jdoubleArray jarg6, jdoubleArray jarg7, jint jarg8) {
  int arg1 ;
  float *arg2 = (float *) 0 ;
  int arg3 ;
  int arg4 ;
  int arg5 ;
  double *arg6 = (double *) (double *)0 ;
  double *arg7 = (double *) (double *)0 ;
  int arg8 ;
  jdouble *jarr6 ;
  jdouble *jarr7 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  arg4 = (int)jarg4; 
  arg5 = (int)jarg5; 
  if (!SWIG_JavaArrayInDouble(jenv, &jarr6, (double **)&arg6, jarg6)) return ; 
  if (!SWIG_JavaArrayInDouble(jenv, &jarr7, (double **)&arg7, jarg7)) return ; 
  arg8 = (int)jarg8; 
  fillVertices(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);
  SWIG_JavaArrayArgoutDouble(jenv, jarr6, (double *)arg6, jarg6); 
  SWIG_JavaArrayArgoutDouble(jenv, jarr7, (double *)arg7, jarg7); 
  free(arg6); 
  free(arg7); 
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillNormals(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jobject jarg3, jint jarg4, jint jarg5) {
  int arg1 ;
  float *arg2 = (float *) 0 ;
  float *arg3 = (float *) 0 ;
  int arg4 ;
  int arg5 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  {
    arg3 = (*jenv)->GetDirectBufferAddress(jenv, jarg3);
    if (arg3 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg4 = (int)jarg4; 
  arg5 = (int)jarg5; 
  fillNormals(arg1,arg2,arg3,arg4,arg5);
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillColors(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3, jint jarg4) {
  int arg1 ;
  float *arg2 = (float *) 0 ;
  int arg3 ;
  int arg4 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  arg4 = (int)jarg4; 
  fillColors(arg1,arg2,arg3,arg4);
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillTextureCoordinates(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3) {
  int arg1 ;
  float *arg2 = (float *) 0 ;
  int arg3 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  fillTextureCoordinates(arg1,arg2,arg3);
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getIndicesSize(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getIndicesSize(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillIndices(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3, jint jarg4) {
  jint jresult = 0 ;
  int arg1 ;
  int *arg2 = (int *) 0 ;
  int arg3 ;
  int arg4 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  arg4 = (int)jarg4; 
  result = (int)fillIndices(arg1,arg2,arg3,arg4);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getWireIndicesSize(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getWireIndicesSize(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillWireIndices(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3, jint jarg4) {
  jint jresult = 0 ;
  int arg1 ;
  int *arg2 = (int *) 0 ;
  int arg3 ;
  int arg4 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  arg4 = (int)jarg4; 
  result = (int)fillWireIndices(arg1,arg2,arg3,arg4);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getMarkIndicesSize(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getMarkIndicesSize(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_fillMarkIndices(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2, jint jarg3) {
  jint jresult = 0 ;
  int arg1 ;
  int *arg2 = (int *) 0 ;
  int arg3 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  arg3 = (int)jarg3; 
  result = (int)fillMarkIndices(arg1,arg2,arg3);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jobject JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureData(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jobject jresult = 0 ;
  int arg1 ;
  JavaDirectBuffer result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = getTextureData(arg1);
  {
    if ((&result)->address)
    {
      jresult = (*jenv)->NewDirectByteBuffer(jenv, (&result)->address, (&result)->size);
    }
  }
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureImageType(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getTextureImageType(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureDataType(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getTextureDataType(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_getTextureGLType(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)getTextureGLType(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_disposeTextureData(JNIEnv *jenv, jclass jcls, jint jarg1, jobject jarg2) {
  int arg1 ;
  unsigned char *arg2 = (unsigned char *) 0 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  {
    arg2 = (*jenv)->GetDirectBufferAddress(jenv, jarg2);
    if (arg2 == NULL) {
      SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of direct buffer. Buffer must be allocated direct.");
    }
  }
  disposeTextureData(arg1,arg2);
}


SWIGEXPORT jint JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_isTextureRowOrder(JNIEnv *jenv, jclass jcls, jint jarg1) {
  jint jresult = 0 ;
  int arg1 ;
  int result;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  result = (int)isTextureRowOrder(arg1);
  jresult = (jint)result; 
  return jresult;
}


SWIGEXPORT void JNICALL Java_org_scilab_modules_graphic_1objects_DataLoaderJNI_setABGRExt(JNIEnv *jenv, jclass jcls, jint jarg1) {
  int arg1 ;
  
  (void)jenv;
  (void)jcls;
  arg1 = (int)jarg1; 
  setABGRExt(arg1);
}


#ifdef __cplusplus
}
#endif

