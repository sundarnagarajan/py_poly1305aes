
#include <Python.h>
#include <stddef.h>

/* this block of #ifs should be kept exactly identical between
   c/_cffi_backend.c, cffi/vengine_cpy.py, cffi/vengine_gen.py */
#if defined(_MSC_VER)
# include <malloc.h>   /* for alloca() */
# if _MSC_VER < 1600   /* MSVC < 2010 */
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
   typedef __int8 int_least8_t;
   typedef __int16 int_least16_t;
   typedef __int32 int_least32_t;
   typedef __int64 int_least64_t;
   typedef unsigned __int8 uint_least8_t;
   typedef unsigned __int16 uint_least16_t;
   typedef unsigned __int32 uint_least32_t;
   typedef unsigned __int64 uint_least64_t;
   typedef __int8 int_fast8_t;
   typedef __int16 int_fast16_t;
   typedef __int32 int_fast32_t;
   typedef __int64 int_fast64_t;
   typedef unsigned __int8 uint_fast8_t;
   typedef unsigned __int16 uint_fast16_t;
   typedef unsigned __int32 uint_fast32_t;
   typedef unsigned __int64 uint_fast64_t;
   typedef __int64 intmax_t;
   typedef unsigned __int64 uintmax_t;
# else
#  include <stdint.h>
# endif
# if _MSC_VER < 1800   /* MSVC < 2013 */
   typedef unsigned char _Bool;
# endif
#else
# include <stdint.h>
# if (defined (__SVR4) && defined (__sun)) || defined(_AIX)
#  include <alloca.h>
# endif
#endif

#if PY_MAJOR_VERSION < 3
# undef PyCapsule_CheckExact
# undef PyCapsule_GetPointer
# define PyCapsule_CheckExact(capsule) (PyCObject_Check(capsule))
# define PyCapsule_GetPointer(capsule, name) \
    (PyCObject_AsVoidPtr(capsule))
#endif

#if PY_MAJOR_VERSION >= 3
# define PyInt_FromLong PyLong_FromLong
#endif

#define _cffi_from_c_double PyFloat_FromDouble
#define _cffi_from_c_float PyFloat_FromDouble
#define _cffi_from_c_long PyInt_FromLong
#define _cffi_from_c_ulong PyLong_FromUnsignedLong
#define _cffi_from_c_longlong PyLong_FromLongLong
#define _cffi_from_c_ulonglong PyLong_FromUnsignedLongLong

#define _cffi_to_c_double PyFloat_AsDouble
#define _cffi_to_c_float PyFloat_AsDouble

#define _cffi_from_c_int_const(x)                                        \
    (((x) > 0) ?                                                         \
        ((unsigned long long)(x) <= (unsigned long long)LONG_MAX) ?      \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromUnsignedLongLong((unsigned long long)(x)) :       \
        ((long long)(x) >= (long long)LONG_MIN) ?                        \
            PyInt_FromLong((long)(x)) :                                  \
            PyLong_FromLongLong((long long)(x)))

#define _cffi_from_c_int(x, type)                                        \
    (((type)-1) > 0 ? /* unsigned */                                     \
        (sizeof(type) < sizeof(long) ?                                   \
            PyInt_FromLong((long)x) :                                    \
         sizeof(type) == sizeof(long) ?                                  \
            PyLong_FromUnsignedLong((unsigned long)x) :                  \
            PyLong_FromUnsignedLongLong((unsigned long long)x)) :        \
        (sizeof(type) <= sizeof(long) ?                                  \
            PyInt_FromLong((long)x) :                                    \
            PyLong_FromLongLong((long long)x)))

#define _cffi_to_c_int(o, type)                                          \
    ((type)(                                                             \
     sizeof(type) == 1 ? (((type)-1) > 0 ? (type)_cffi_to_c_u8(o)        \
                                         : (type)_cffi_to_c_i8(o)) :     \
     sizeof(type) == 2 ? (((type)-1) > 0 ? (type)_cffi_to_c_u16(o)       \
                                         : (type)_cffi_to_c_i16(o)) :    \
     sizeof(type) == 4 ? (((type)-1) > 0 ? (type)_cffi_to_c_u32(o)       \
                                         : (type)_cffi_to_c_i32(o)) :    \
     sizeof(type) == 8 ? (((type)-1) > 0 ? (type)_cffi_to_c_u64(o)       \
                                         : (type)_cffi_to_c_i64(o)) :    \
     (Py_FatalError("unsupported size for type " #type), (type)0)))

#define _cffi_to_c_i8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[1])
#define _cffi_to_c_u8                                                    \
                 ((int(*)(PyObject *))_cffi_exports[2])
#define _cffi_to_c_i16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[3])
#define _cffi_to_c_u16                                                   \
                 ((int(*)(PyObject *))_cffi_exports[4])
#define _cffi_to_c_i32                                                   \
                 ((int(*)(PyObject *))_cffi_exports[5])
#define _cffi_to_c_u32                                                   \
                 ((unsigned int(*)(PyObject *))_cffi_exports[6])
#define _cffi_to_c_i64                                                   \
                 ((long long(*)(PyObject *))_cffi_exports[7])
#define _cffi_to_c_u64                                                   \
                 ((unsigned long long(*)(PyObject *))_cffi_exports[8])
#define _cffi_to_c_char                                                  \
                 ((int(*)(PyObject *))_cffi_exports[9])
#define _cffi_from_c_pointer                                             \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[10])
#define _cffi_to_c_pointer                                               \
    ((char *(*)(PyObject *, CTypeDescrObject *))_cffi_exports[11])
#define _cffi_get_struct_layout                                          \
    ((PyObject *(*)(Py_ssize_t[]))_cffi_exports[12])
#define _cffi_restore_errno                                              \
    ((void(*)(void))_cffi_exports[13])
#define _cffi_save_errno                                                 \
    ((void(*)(void))_cffi_exports[14])
#define _cffi_from_c_char                                                \
    ((PyObject *(*)(char))_cffi_exports[15])
#define _cffi_from_c_deref                                               \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[16])
#define _cffi_to_c                                                       \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[17])
#define _cffi_from_c_struct                                              \
    ((PyObject *(*)(char *, CTypeDescrObject *))_cffi_exports[18])
#define _cffi_to_c_wchar_t                                               \
    ((wchar_t(*)(PyObject *))_cffi_exports[19])
#define _cffi_from_c_wchar_t                                             \
    ((PyObject *(*)(wchar_t))_cffi_exports[20])
#define _cffi_to_c_long_double                                           \
    ((long double(*)(PyObject *))_cffi_exports[21])
#define _cffi_to_c__Bool                                                 \
    ((_Bool(*)(PyObject *))_cffi_exports[22])
#define _cffi_prepare_pointer_call_argument                              \
    ((Py_ssize_t(*)(CTypeDescrObject *, PyObject *, char **))_cffi_exports[23])
#define _cffi_convert_array_from_object                                  \
    ((int(*)(char *, CTypeDescrObject *, PyObject *))_cffi_exports[24])
#define _CFFI_NUM_EXPORTS 25

typedef struct _ctypedescr CTypeDescrObject;

static void *_cffi_exports[_CFFI_NUM_EXPORTS];
static PyObject *_cffi_types, *_cffi_VerificationError;

static int _cffi_setup_custom(PyObject *lib);   /* forward */

static PyObject *_cffi_setup(PyObject *self, PyObject *args)
{
    PyObject *library;
    int was_alive = (_cffi_types != NULL);
    (void)self; /* unused */
    if (!PyArg_ParseTuple(args, "OOO", &_cffi_types, &_cffi_VerificationError,
                                       &library))
        return NULL;
    Py_INCREF(_cffi_types);
    Py_INCREF(_cffi_VerificationError);
    if (_cffi_setup_custom(library) < 0)
        return NULL;
    return PyBool_FromLong(was_alive);
}

static int _cffi_init(void)
{
    PyObject *module, *c_api_object = NULL;

    module = PyImport_ImportModule("_cffi_backend");
    if (module == NULL)
        goto failure;

    c_api_object = PyObject_GetAttrString(module, "_C_API");
    if (c_api_object == NULL)
        goto failure;
    if (!PyCapsule_CheckExact(c_api_object)) {
        PyErr_SetNone(PyExc_ImportError);
        goto failure;
    }
    memcpy(_cffi_exports, PyCapsule_GetPointer(c_api_object, "cffi"),
           _CFFI_NUM_EXPORTS * sizeof(void *));

    Py_DECREF(module);
    Py_DECREF(c_api_object);
    return 0;

  failure:
    Py_XDECREF(module);
    Py_XDECREF(c_api_object);
    return -1;
}

#define _cffi_type(num) ((CTypeDescrObject *)PyList_GET_ITEM(_cffi_types, num))

/**********/



extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_isequal(const unsigned char x[16],
  const unsigned char y[16]);
void poly1305aes_53_clamp(unsigned char kr[32])
{
  unsigned int r3;
  unsigned int r7;
  unsigned int r11;
  unsigned int r15;
  unsigned int r4;
  unsigned int r8;
  unsigned int r12;
  r3 = (kr + 16)[3];
  r7 = (kr + 16)[7];
  r11 = (kr + 16)[11];
  r15 = (kr + 16)[15];
  r4 = (kr + 16)[4];
  r8 = (kr + 16)[8];
  r12 = (kr + 16)[12];
  r3 &= 15;
  r7 &= 15;
  r11 &= 15;
  r15 &= 15;
  r4 &= 252;
  r8 &= 252;
  r12 &= 252;
  (kr + 16)[3] = r3;
  (kr + 16)[7] = r7;
  (kr + 16)[11] = r11;
  (kr + 16)[15] = r15;
  (kr + 16)[4] = r4;
  (kr + 16)[8] = r8;
  (kr + 16)[12] = r12;
}
extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_isequal(const unsigned char x[16],
  const unsigned char y[16]);
extern void poly1305_53(unsigned char out[16],
  const unsigned char r[16],
  const unsigned char s[16],
  const unsigned char m[],unsigned int l);
extern const double poly1305_53_constants[];
extern void aes_big(unsigned char out[16],
  const unsigned char k[16],
  const unsigned char n[16]);
extern const unsigned int aes_big_constants[1034];
void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l)
{
  unsigned char aeskn[16];
  aes_big(aeskn,(kr + 0),n);
  poly1305_53(out,(kr + 16),aeskn,m,l);
}
extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_isequal(const unsigned char x[16],
  const unsigned char y[16]);
extern void poly1305_53(unsigned char out[16],
  const unsigned char r[16],
  const unsigned char s[16],
  const unsigned char m[],unsigned int l);
extern const double poly1305_53_constants[];
extern void aes_big(unsigned char out[16],
  const unsigned char k[16],
  const unsigned char n[16]);
extern const unsigned int aes_big_constants[1034];
int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l)
{
  unsigned char aeskn[16];
  unsigned char valid[16];
  aes_big(aeskn,(kr + 0),n);
  poly1305_53(valid,(kr + 16),aeskn,m,l);
  return poly1305aes_53_isequal(a,valid);
}
extern void poly1305_53(unsigned char out[16],
  const unsigned char r[16],
  const unsigned char s[16],
  const unsigned char m[],unsigned int l);
extern const double poly1305_53_constants[];
void poly1305_53(
  unsigned char out[16],
  const unsigned char r[16],
  const unsigned char s[16],
  const unsigned char m[],
  unsigned int l
)
{
  double r0high_stack;
  double r1high_stack;
  double r1low_stack;
  double sr1high_stack;
  double r2low_stack;
  double sr2high_stack;
  double r0low_stack;
  double sr1low_stack;
  double r2high_stack;
  double sr2low_stack;
  double r3high_stack;
  double sr3high_stack;
  double r3low_stack;
  double sr3low_stack;
  long long d0;
  long long d1;
  long long d2;
  long long d3;
  register double scale;
  register double alpha0;
  register double alpha32;
  register double alpha64;
  register double alpha96;
  register double alpha130;
  register double h0;
  register double h1;
  register double h2;
  register double h3;
  register double h4;
  register double h5;
  register double h6;
  register double h7;
  register double y7;
  register double y6;
  register double y1;
  register double y0;
  register double y5;
  register double y4;
  register double x7;
  register double x6;
  register double x1;
  register double x0;
  register double y3;
  register double y2;
  register double r3low;
  register double r0low;
  register double r3high;
  register double r0high;
  register double sr1low;
  register double x5;
  register double r3lowx0;
  register double sr1high;
  register double x4;
  register double r0lowx6;
  register double r1low;
  register double x3;
  register double r3highx0;
  register double r1high;
  register double x2;
  register double r0highx6;
  register double sr2low;
  register double r0lowx0;
  register double sr2high;
  register double sr1lowx6;
  register double r2low;
  register double r0highx0;
  register double r2high;
  register double sr1highx6;
  register double sr3low;
  register double r1lowx0;
  register double sr3high;
  register double sr2lowx6;
  register double r1highx0;
  register double sr2highx6;
  register double r2lowx0;
  register double sr3lowx6;
  register double r2highx0;
  register double sr3highx6;
  register double r1highx4;
  register double r1lowx4;
  register double r0highx4;
  register double r0lowx4;
  register double sr3highx4;
  register double sr3lowx4;
  register double sr2highx4;
  register double sr2lowx4;
  register double r0lowx2;
  register double r0highx2;
  register double r1lowx2;
  register double r1highx2;
  register double r2lowx2;
  register double r2highx2;
  register double sr3lowx2;
  register double sr3highx2;
  register double z0;
  register double z1;
  register double z2;
  register double z3;
  register long long r0;
  register long long r1;
  register long long r2;
  register long long r3;
  register unsigned int r00;
  register unsigned int r01;
  register unsigned int r02;
  register unsigned int r03;
  register unsigned int r10;
  register unsigned int r11;
  register unsigned int r12;
  register unsigned int r13;
  register unsigned int r20;
  register unsigned int r21;
  register unsigned int r22;
  register unsigned int r23;
  register unsigned int r30;
  register unsigned int r31;
  register unsigned int r32;
  register unsigned int r33;
  register long long m0;
  register long long m1;
  register long long m2;
  register long long m3;
  register unsigned int m00;
  register unsigned int m01;
  register unsigned int m02;
  register unsigned int m03;
  register unsigned int m10;
  register unsigned int m11;
  register unsigned int m12;
  register unsigned int m13;
  register unsigned int m20;
  register unsigned int m21;
  register unsigned int m22;
  register unsigned int m23;
  register unsigned int m30;
  register unsigned int m31;
  register unsigned int m32;
  register unsigned long long m33;
  register char *constants;
  register int lbelow2;
  register int lbelow3;
  register int lbelow4;
  register int lbelow5;
  register int lbelow6;
  register int lbelow7;
  register int lbelow8;
  register int lbelow9;
  register int lbelow10;
  register int lbelow11;
  register int lbelow12;
  register int lbelow13;
  register int lbelow14;
  register int lbelow15;
  register double alpham80;
  register double alpham48;
  register double alpham16;
  register double alpha18;
  register double alpha50;
  register double alpha82;
  register double alpha112;
  register double offset0;
  register double offset1;
  register double offset2;
  register double offset3;
  register unsigned int s00;
  register unsigned int s01;
  register unsigned int s02;
  register unsigned int s03;
  register unsigned int s10;
  register unsigned int s11;
  register unsigned int s12;
  register unsigned int s13;
  register unsigned int s20;
  register unsigned int s21;
  register unsigned int s22;
  register unsigned int s23;
  register unsigned int s30;
  register unsigned int s31;
  register unsigned int s32;
  register unsigned int s33;
  register unsigned long long bits32;
  register unsigned long long f;
  register unsigned long long f0;
  register unsigned long long f1;
  register unsigned long long f2;
  register unsigned long long f3;
  register unsigned long long f4;
  register unsigned long long g;
  register unsigned long long g0;
  register unsigned long long g1;
  register unsigned long long g2;
  register unsigned long long g3;
  register unsigned long long g4;
  r00 = *(unsigned char *) (r + 0);
  constants = (char *) &poly1305_53_constants;
  r01 = *(unsigned char *) (r + 1);
  r02 = *(unsigned char *) (r + 2);
  r0 = 2151;
  r03 = *(unsigned char *) (r + 3);
  r0 <<= 51;
  r10 = *(unsigned char *) (r + 4);
  r01 <<= 8;
  r0 += r00;
  r11 = *(unsigned char *) (r + 5);
  r02 <<= 16;
  r0 += r01;
  r12 = *(unsigned char *) (r + 6);
  r03 <<= 24;
  r0 += r02;
  r13 = *(unsigned char *) (r + 7);
  r1 = 2215;
  r0 += r03;
  d0 = r0;
  r1 <<= 51;
  r2 = 2279;
  r20 = *(unsigned char *) (r + 8);
  r11 <<= 8;
  r1 += r10;
  r21 = *(unsigned char *) (r + 9);
  r12 <<= 16;
  r1 += r11;
  r22 = *(unsigned char *) (r + 10);
  r13 <<= 24;
  r1 += r12;
  r23 = *(unsigned char *) (r + 11);
  r2 <<= 51;
  r1 += r13;
  d1 = r1;
  r21 <<= 8;
  r2 += r20;
  r30 = *(unsigned char *) (r + 12);
  r22 <<= 16;
  r2 += r21;
  r31 = *(unsigned char *) (r + 13);
  r23 <<= 24;
  r2 += r22;
  r32 = *(unsigned char *) (r + 14);
  r2 += r23;
  r3 = 2343;
  d2 = r2;
  r3 <<= 51;
  alpha32 = *(double *) (constants + 40);
  r33 = *(unsigned char *) (r + 15);
  r31 <<= 8;
  r3 += r30;
  r32 <<= 16;
  r3 += r31;
  r33 <<= 24;
  r3 += r32;
  r3 += r33;
  h0 = alpha32 - alpha32;
  d3 = r3;
  h1 = alpha32 - alpha32;
  alpha0 = *(double *) (constants + 24);
  h2 = alpha32 - alpha32;
  alpha64 = *(double *) (constants + 56);
  h3 = alpha32 - alpha32;
  alpha18 = *(double *) (constants + 32);
  h4 = alpha32 - alpha32;
  r0low = *(double *) &d0;
  h5 = alpha32 - alpha32;
  r1low = *(double *) &d1;
  h6 = alpha32 - alpha32;
  r2low = *(double *) &d2;
  h7 = alpha32 - alpha32;
  alpha50 = *(double *) (constants + 48);
  r0low -= alpha0;
  alpha82 = *(double *) (constants + 64);
  r1low -= alpha32;
  scale = *(double *) (constants + 96);
  r2low -= alpha64;
  alpha96 = *(double *) (constants + 72);
  r0high = r0low + alpha18;
  r3low = *(double *) &d3;
  alpham80 = *(double *) (constants + 0);
  r1high = r1low + alpha50;
  sr1low = scale * r1low;
  alpham48 = *(double *) (constants + 8);
  r2high = r2low + alpha82;
  sr2low = scale * r2low;
  r0high -= alpha18;
  r0high_stack = r0high;
  r3low -= alpha96;
  r1high -= alpha50;
  r1high_stack = r1high;
  sr1high = sr1low + alpham80;
  alpha112 = *(double *) (constants + 80);
  r0low -= r0high;
  alpham16 = *(double *) (constants + 16);
  r2high -= alpha82;
  sr3low = scale * r3low;
  alpha130 = *(double *) (constants + 88);
  sr2high = sr2low + alpham48;
  r1low -= r1high;
  r1low_stack = r1low;
  sr1high -= alpham80;
  sr1high_stack = sr1high;
  r2low -= r2high;
  r2low_stack = r2low;
  sr2high -= alpham48;
  sr2high_stack = sr2high;
  r3high = r3low + alpha112;
  r0low_stack = r0low;
  sr1low -= sr1high;
  sr1low_stack = sr1low;
  sr3high = sr3low + alpham16;
  r2high_stack = r2high;
  sr2low -= sr2high;
  sr2low_stack = sr2low;
  r3high -= alpha112;
  r3high_stack = r3high;
  sr3high -= alpham16;
  sr3high_stack = sr3high;
  r3low -= r3high;
  r3low_stack = r3low;
  sr3low -= sr3high;
  sr3low_stack = sr3low;
if (l < 16) goto addatmost15bytes;
  m00 = *(unsigned char *) (m + 0);
  m0 = 2151;
  m0 <<= 51;
  m1 = 2215;
  m01 = *(unsigned char *) (m + 1);
  m1 <<= 51;
  m2 = 2279;
  m02 = *(unsigned char *) (m + 2);
  m2 <<= 51;
  m3 = 2343;
  m03 = *(unsigned char *) (m + 3);
  m10 = *(unsigned char *) (m + 4);
  m01 <<= 8;
  m0 += m00;
  m11 = *(unsigned char *) (m + 5);
  m02 <<= 16;
  m0 += m01;
  m12 = *(unsigned char *) (m + 6);
  m03 <<= 24;
  m0 += m02;
  m13 = *(unsigned char *) (m + 7);
  m3 <<= 51;
  m0 += m03;
  m20 = *(unsigned char *) (m + 8);
  m11 <<= 8;
  m1 += m10;
  m21 = *(unsigned char *) (m + 9);
  m12 <<= 16;
  m1 += m11;
  m22 = *(unsigned char *) (m + 10);
  m13 <<= 24;
  m1 += m12;
  m23 = *(unsigned char *) (m + 11);
  m1 += m13;
  m30 = *(unsigned char *) (m + 12);
  m21 <<= 8;
  m2 += m20;
  m31 = *(unsigned char *) (m + 13);
  m22 <<= 16;
  m2 += m21;
  m32 = *(unsigned char *) (m + 14);
  m23 <<= 24;
  m2 += m22;
  m33 = *(unsigned char *) (m + 15);
  m2 += m23;
  d0 = m0;
  m31 <<= 8;
  m3 += m30;
  d1 = m1;
  m32 <<= 16;
  m3 += m31;
  d2 = m2;
  m33 += 256;
  m33 <<= 24;
  m3 += m32;
  m3 += m33;
  d3 = m3;
  m += 16;
  l -= 16;
  z0 = *(double *) &d0;
  z1 = *(double *) &d1;
  z2 = *(double *) &d2;
  z3 = *(double *) &d3;
  z0 -= alpha0;
  z1 -= alpha32;
  z2 -= alpha64;
  z3 -= alpha96;
  h0 += z0;
  h1 += z1;
  h3 += z2;
  h5 += z3;
if (l < 16) goto multiplyaddatmost15bytes;
multiplyaddatleast16bytes:;
  m2 = 2279;
  m20 = *(unsigned char *) (m + 8);
  y7 = h7 + alpha130;
  m2 <<= 51;
  m3 = 2343;
  m21 = *(unsigned char *) (m + 9);
  y6 = h6 + alpha130;
  m3 <<= 51;
  m0 = 2151;
  m22 = *(unsigned char *) (m + 10);
  y1 = h1 + alpha32;
  m0 <<= 51;
  m1 = 2215;
  m23 = *(unsigned char *) (m + 11);
  y0 = h0 + alpha32;
  m1 <<= 51;
  m30 = *(unsigned char *) (m + 12);
  y7 -= alpha130;
  m21 <<= 8;
  m2 += m20;
  m31 = *(unsigned char *) (m + 13);
  y6 -= alpha130;
  m22 <<= 16;
  m2 += m21;
  m32 = *(unsigned char *) (m + 14);
  y1 -= alpha32;
  m23 <<= 24;
  m2 += m22;
  m33 = *(unsigned char *) (m + 15);
  y0 -= alpha32;
  m2 += m23;
  m00 = *(unsigned char *) (m + 0);
  y5 = h5 + alpha96;
  m31 <<= 8;
  m3 += m30;
  m01 = *(unsigned char *) (m + 1);
  y4 = h4 + alpha96;
  m32 <<= 16;
  m02 = *(unsigned char *) (m + 2);
  x7 = h7 - y7;
  y7 *= scale;
  m33 += 256;
  m03 = *(unsigned char *) (m + 3);
  x6 = h6 - y6;
  y6 *= scale;
  m33 <<= 24;
  m3 += m31;
  m10 = *(unsigned char *) (m + 4);
  x1 = h1 - y1;
  m01 <<= 8;
  m3 += m32;
  m11 = *(unsigned char *) (m + 5);
  x0 = h0 - y0;
  m3 += m33;
  m0 += m00;
  m12 = *(unsigned char *) (m + 6);
  y5 -= alpha96;
  m02 <<= 16;
  m0 += m01;
  m13 = *(unsigned char *) (m + 7);
  y4 -= alpha96;
  m03 <<= 24;
  m0 += m02;
  d2 = m2;
  x1 += y7;
  m0 += m03;
  d3 = m3;
  x0 += y6;
  m11 <<= 8;
  m1 += m10;
  d0 = m0;
  x7 += y5;
  m12 <<= 16;
  m1 += m11;
  x6 += y4;
  m13 <<= 24;
  m1 += m12;
  y3 = h3 + alpha64;
  m1 += m13;
  d1 = m1;
  y2 = h2 + alpha64;
  x0 += x1;
  x6 += x7;
  y3 -= alpha64;
  r3low = r3low_stack;
  y2 -= alpha64;
  r0low = r0low_stack;
  x5 = h5 - y5;
  r3lowx0 = r3low * x0;
  r3high = r3high_stack;
  x4 = h4 - y4;
  r0lowx6 = r0low * x6;
  r0high = r0high_stack;
  x3 = h3 - y3;
  r3highx0 = r3high * x0;
  sr1low = sr1low_stack;
  x2 = h2 - y2;
  r0highx6 = r0high * x6;
  sr1high = sr1high_stack;
  x5 += y3;
  r0lowx0 = r0low * x0;
  r1low = r1low_stack;
  h6 = r3lowx0 + r0lowx6;
  sr1lowx6 = sr1low * x6;
  r1high = r1high_stack;
  x4 += y2;
  r0highx0 = r0high * x0;
  sr2low = sr2low_stack;
  h7 = r3highx0 + r0highx6;
  sr1highx6 = sr1high * x6;
  sr2high = sr2high_stack;
  x3 += y1;
  r1lowx0 = r1low * x0;
  r2low = r2low_stack;
  h0 = r0lowx0 + sr1lowx6;
  sr2lowx6 = sr2low * x6;
  r2high = r2high_stack;
  x2 += y0;
  r1highx0 = r1high * x0;
  sr3low = sr3low_stack;
  h1 = r0highx0 + sr1highx6;
  sr2highx6 = sr2high * x6;
  sr3high = sr3high_stack;
  x4 += x5;
  r2lowx0 = r2low * x0;
  z2 = *(double *) &d2;
  h2 = r1lowx0 + sr2lowx6;
  sr3lowx6 = sr3low * x6;
  x2 += x3;
  r2highx0 = r2high * x0;
  z3 = *(double *) &d3;
  h3 = r1highx0 + sr2highx6;
  sr3highx6 = sr3high * x6;
  r1highx4 = r1high * x4;
  z2 -= alpha64;
  h4 = r2lowx0 + sr3lowx6;
  r1lowx4 = r1low * x4;
  r0highx4 = r0high * x4;
  z3 -= alpha96;
  h5 = r2highx0 + sr3highx6;
  r0lowx4 = r0low * x4;
  h7 += r1highx4;
  sr3highx4 = sr3high * x4;
  h6 += r1lowx4;
  sr3lowx4 = sr3low * x4;
  h5 += r0highx4;
  sr2highx4 = sr2high * x4;
  h4 += r0lowx4;
  sr2lowx4 = sr2low * x4;
  h3 += sr3highx4;
  r0lowx2 = r0low * x2;
  h2 += sr3lowx4;
  r0highx2 = r0high * x2;
  h1 += sr2highx4;
  r1lowx2 = r1low * x2;
  h0 += sr2lowx4;
  r1highx2 = r1high * x2;
  h2 += r0lowx2;
  r2lowx2 = r2low * x2;
  h3 += r0highx2;
  r2highx2 = r2high * x2;
  h4 += r1lowx2;
  sr3lowx2 = sr3low * x2;
  h5 += r1highx2;
  sr3highx2 = sr3high * x2;
  alpha0 = *(double *) (constants + 24);
  m += 16;
  h6 += r2lowx2;
  l -= 16;
  h7 += r2highx2;
  z1 = *(double *) &d1;
  h0 += sr3lowx2;
  z0 = *(double *) &d0;
  h1 += sr3highx2;
  z1 -= alpha32;
  z0 -= alpha0;
  h5 += z3;
  h3 += z2;
  h1 += z1;
  h0 += z0;
if (l >= 16) goto multiplyaddatleast16bytes;
multiplyaddatmost15bytes:;
  y7 = h7 + alpha130;
  y6 = h6 + alpha130;
  y1 = h1 + alpha32;
  y0 = h0 + alpha32;
  y7 -= alpha130;
  y6 -= alpha130;
  y1 -= alpha32;
  y0 -= alpha32;
  y5 = h5 + alpha96;
  y4 = h4 + alpha96;
  x7 = h7 - y7;
  y7 *= scale;
  x6 = h6 - y6;
  y6 *= scale;
  x1 = h1 - y1;
  x0 = h0 - y0;
  y5 -= alpha96;
  y4 -= alpha96;
  x1 += y7;
  x0 += y6;
  x7 += y5;
  x6 += y4;
  y3 = h3 + alpha64;
  y2 = h2 + alpha64;
  x0 += x1;
  x6 += x7;
  y3 -= alpha64;
  r3low = r3low_stack;
  y2 -= alpha64;
  r0low = r0low_stack;
  x5 = h5 - y5;
  r3lowx0 = r3low * x0;
  r3high = r3high_stack;
  x4 = h4 - y4;
  r0lowx6 = r0low * x6;
  r0high = r0high_stack;
  x3 = h3 - y3;
  r3highx0 = r3high * x0;
  sr1low = sr1low_stack;
  x2 = h2 - y2;
  r0highx6 = r0high * x6;
  sr1high = sr1high_stack;
  x5 += y3;
  r0lowx0 = r0low * x0;
  r1low = r1low_stack;
  h6 = r3lowx0 + r0lowx6;
  sr1lowx6 = sr1low * x6;
  r1high = r1high_stack;
  x4 += y2;
  r0highx0 = r0high * x0;
  sr2low = sr2low_stack;
  h7 = r3highx0 + r0highx6;
  sr1highx6 = sr1high * x6;
  sr2high = sr2high_stack;
  x3 += y1;
  r1lowx0 = r1low * x0;
  r2low = r2low_stack;
  h0 = r0lowx0 + sr1lowx6;
  sr2lowx6 = sr2low * x6;
  r2high = r2high_stack;
  x2 += y0;
  r1highx0 = r1high * x0;
  sr3low = sr3low_stack;
  h1 = r0highx0 + sr1highx6;
  sr2highx6 = sr2high * x6;
  sr3high = sr3high_stack;
  x4 += x5;
  r2lowx0 = r2low * x0;
  h2 = r1lowx0 + sr2lowx6;
  sr3lowx6 = sr3low * x6;
  x2 += x3;
  r2highx0 = r2high * x0;
  h3 = r1highx0 + sr2highx6;
  sr3highx6 = sr3high * x6;
  r1highx4 = r1high * x4;
  h4 = r2lowx0 + sr3lowx6;
  r1lowx4 = r1low * x4;
  r0highx4 = r0high * x4;
  h5 = r2highx0 + sr3highx6;
  r0lowx4 = r0low * x4;
  h7 += r1highx4;
  sr3highx4 = sr3high * x4;
  h6 += r1lowx4;
  sr3lowx4 = sr3low * x4;
  h5 += r0highx4;
  sr2highx4 = sr2high * x4;
  h4 += r0lowx4;
  sr2lowx4 = sr2low * x4;
  h3 += sr3highx4;
  r0lowx2 = r0low * x2;
  h2 += sr3lowx4;
  r0highx2 = r0high * x2;
  h1 += sr2highx4;
  r1lowx2 = r1low * x2;
  h0 += sr2lowx4;
  r1highx2 = r1high * x2;
  h2 += r0lowx2;
  r2lowx2 = r2low * x2;
  h3 += r0highx2;
  r2highx2 = r2high * x2;
  h4 += r1lowx2;
  sr3lowx2 = sr3low * x2;
  h5 += r1highx2;
  sr3highx2 = sr3high * x2;
  h6 += r2lowx2;
  h7 += r2highx2;
  h0 += sr3lowx2;
  h1 += sr3highx2;
addatmost15bytes:;
if (l == 0) goto nomorebytes;
  lbelow2 = l - 2;
  lbelow3 = l - 3;
  lbelow2 >>= 31;
  lbelow4 = l - 4;
  m00 = *(unsigned char *) (m + 0);
  lbelow3 >>= 31;
  m += lbelow2;
  m01 = *(unsigned char *) (m + 1);
  lbelow4 >>= 31;
  m += lbelow3;
  m02 = *(unsigned char *) (m + 2);
  m += lbelow4;
  m0 = 2151;
  m03 = *(unsigned char *) (m + 3);
  m0 <<= 51;
  m1 = 2215;
  m0 += m00;
  m01 &= ~lbelow2;
  m02 &= ~lbelow3;
  m01 -= lbelow2;
  m01 <<= 8;
  m03 &= ~lbelow4;
  m0 += m01;
  lbelow2 -= lbelow3;
  m02 += lbelow2;
  lbelow3 -= lbelow4;
  m02 <<= 16;
  m03 += lbelow3;
  m03 <<= 24;
  m0 += m02;
  m0 += m03;
  lbelow5 = l - 5;
  lbelow6 = l - 6;
  lbelow7 = l - 7;
  lbelow5 >>= 31;
  lbelow8 = l - 8;
  lbelow6 >>= 31;
  m += lbelow5;
  m10 = *(unsigned char *) (m + 4);
  lbelow7 >>= 31;
  m += lbelow6;
  m11 = *(unsigned char *) (m + 5);
  lbelow8 >>= 31;
  m += lbelow7;
  m12 = *(unsigned char *) (m + 6);
  m1 <<= 51;
  m += lbelow8;
  m13 = *(unsigned char *) (m + 7);
  m10 &= ~lbelow5;
  lbelow4 -= lbelow5;
  m10 += lbelow4;
  lbelow5 -= lbelow6;
  m11 &= ~lbelow6;
  m11 += lbelow5;
  m11 <<= 8;
  m1 += m10;
  m1 += m11;
  m12 &= ~lbelow7;
  lbelow6 -= lbelow7;
  m13 &= ~lbelow8;
  m12 += lbelow6;
  lbelow7 -= lbelow8;
  m12 <<= 16;
  m13 += lbelow7;
  m13 <<= 24;
  m1 += m12;
  m1 += m13;
  m2 = 2279;
  lbelow9 = l - 9;
  m3 = 2343;
  lbelow10 = l - 10;
  lbelow11 = l - 11;
  lbelow9 >>= 31;
  lbelow12 = l - 12;
  lbelow10 >>= 31;
  m += lbelow9;
  m20 = *(unsigned char *) (m + 8);
  lbelow11 >>= 31;
  m += lbelow10;
  m21 = *(unsigned char *) (m + 9);
  lbelow12 >>= 31;
  m += lbelow11;
  m22 = *(unsigned char *) (m + 10);
  m2 <<= 51;
  m += lbelow12;
  m23 = *(unsigned char *) (m + 11);
  m20 &= ~lbelow9;
  lbelow8 -= lbelow9;
  m20 += lbelow8;
  lbelow9 -= lbelow10;
  m21 &= ~lbelow10;
  m21 += lbelow9;
  m21 <<= 8;
  m2 += m20;
  m2 += m21;
  m22 &= ~lbelow11;
  lbelow10 -= lbelow11;
  m23 &= ~lbelow12;
  m22 += lbelow10;
  lbelow11 -= lbelow12;
  m22 <<= 16;
  m23 += lbelow11;
  m23 <<= 24;
  m2 += m22;
  m3 <<= 51;
  lbelow13 = l - 13;
  lbelow13 >>= 31;
  lbelow14 = l - 14;
  lbelow14 >>= 31;
  m += lbelow13;
  lbelow15 = l - 15;
  m30 = *(unsigned char *) (m + 12);
  lbelow15 >>= 31;
  m += lbelow14;
  m31 = *(unsigned char *) (m + 13);
  m += lbelow15;
  m2 += m23;
  m32 = *(unsigned char *) (m + 14);
  m30 &= ~lbelow13;
  lbelow12 -= lbelow13;
  m30 += lbelow12;
  lbelow13 -= lbelow14;
  m3 += m30;
  m31 &= ~lbelow14;
  m31 += lbelow13;
  m32 &= ~lbelow15;
  m31 <<= 8;
  lbelow14 -= lbelow15;
  m3 += m31;
  m32 += lbelow14;
  d0 = m0;
  m32 <<= 16;
  m33 = lbelow15 + 1;
  d1 = m1;
  m33 <<= 24;
  m3 += m32;
  d2 = m2;
  m3 += m33;
  d3 = m3;
  alpha0 = *(double *) (constants + 24);
  z3 = *(double *) &d3;
  z2 = *(double *) &d2;
  z1 = *(double *) &d1;
  z0 = *(double *) &d0;
  z3 -= alpha96;
  z2 -= alpha64;
  z1 -= alpha32;
  z0 -= alpha0;
  h5 += z3;
  h3 += z2;
  h1 += z1;
  h0 += z0;
  y7 = h7 + alpha130;
  y6 = h6 + alpha130;
  y1 = h1 + alpha32;
  y0 = h0 + alpha32;
  y7 -= alpha130;
  y6 -= alpha130;
  y1 -= alpha32;
  y0 -= alpha32;
  y5 = h5 + alpha96;
  y4 = h4 + alpha96;
  x7 = h7 - y7;
  y7 *= scale;
  x6 = h6 - y6;
  y6 *= scale;
  x1 = h1 - y1;
  x0 = h0 - y0;
  y5 -= alpha96;
  y4 -= alpha96;
  x1 += y7;
  x0 += y6;
  x7 += y5;
  x6 += y4;
  y3 = h3 + alpha64;
  y2 = h2 + alpha64;
  x0 += x1;
  x6 += x7;
  y3 -= alpha64;
  r3low = r3low_stack;
  y2 -= alpha64;
  r0low = r0low_stack;
  x5 = h5 - y5;
  r3lowx0 = r3low * x0;
  r3high = r3high_stack;
  x4 = h4 - y4;
  r0lowx6 = r0low * x6;
  r0high = r0high_stack;
  x3 = h3 - y3;
  r3highx0 = r3high * x0;
  sr1low = sr1low_stack;
  x2 = h2 - y2;
  r0highx6 = r0high * x6;
  sr1high = sr1high_stack;
  x5 += y3;
  r0lowx0 = r0low * x0;
  r1low = r1low_stack;
  h6 = r3lowx0 + r0lowx6;
  sr1lowx6 = sr1low * x6;
  r1high = r1high_stack;
  x4 += y2;
  r0highx0 = r0high * x0;
  sr2low = sr2low_stack;
  h7 = r3highx0 + r0highx6;
  sr1highx6 = sr1high * x6;
  sr2high = sr2high_stack;
  x3 += y1;
  r1lowx0 = r1low * x0;
  r2low = r2low_stack;
  h0 = r0lowx0 + sr1lowx6;
  sr2lowx6 = sr2low * x6;
  r2high = r2high_stack;
  x2 += y0;
  r1highx0 = r1high * x0;
  sr3low = sr3low_stack;
  h1 = r0highx0 + sr1highx6;
  sr2highx6 = sr2high * x6;
  sr3high = sr3high_stack;
  x4 += x5;
  r2lowx0 = r2low * x0;
  h2 = r1lowx0 + sr2lowx6;
  sr3lowx6 = sr3low * x6;
  x2 += x3;
  r2highx0 = r2high * x0;
  h3 = r1highx0 + sr2highx6;
  sr3highx6 = sr3high * x6;
  r1highx4 = r1high * x4;
  h4 = r2lowx0 + sr3lowx6;
  r1lowx4 = r1low * x4;
  r0highx4 = r0high * x4;
  h5 = r2highx0 + sr3highx6;
  r0lowx4 = r0low * x4;
  h7 += r1highx4;
  sr3highx4 = sr3high * x4;
  h6 += r1lowx4;
  sr3lowx4 = sr3low * x4;
  h5 += r0highx4;
  sr2highx4 = sr2high * x4;
  h4 += r0lowx4;
  sr2lowx4 = sr2low * x4;
  h3 += sr3highx4;
  r0lowx2 = r0low * x2;
  h2 += sr3lowx4;
  r0highx2 = r0high * x2;
  h1 += sr2highx4;
  r1lowx2 = r1low * x2;
  h0 += sr2lowx4;
  r1highx2 = r1high * x2;
  h2 += r0lowx2;
  r2lowx2 = r2low * x2;
  h3 += r0highx2;
  r2highx2 = r2high * x2;
  h4 += r1lowx2;
  sr3lowx2 = sr3low * x2;
  h5 += r1highx2;
  sr3highx2 = sr3high * x2;
  h6 += r2lowx2;
  h7 += r2highx2;
  h0 += sr3lowx2;
  h1 += sr3highx2;
nomorebytes:;
  offset0 = *(double *) (constants + 104);
  y7 = h7 + alpha130;
  offset1 = *(double *) (constants + 112);
  y0 = h0 + alpha32;
  offset2 = *(double *) (constants + 120);
  y1 = h1 + alpha32;
  offset3 = *(double *) (constants + 128);
  y2 = h2 + alpha64;
  y7 -= alpha130;
  y3 = h3 + alpha64;
  y4 = h4 + alpha96;
  y5 = h5 + alpha96;
  x7 = h7 - y7;
  y7 *= scale;
  y0 -= alpha32;
  y1 -= alpha32;
  y2 -= alpha64;
  h6 += x7;
  y3 -= alpha64;
  y4 -= alpha96;
  y5 -= alpha96;
  y6 = h6 + alpha130;
  x0 = h0 - y0;
  x1 = h1 - y1;
  x2 = h2 - y2;
  y6 -= alpha130;
  x0 += y7;
  x3 = h3 - y3;
  x4 = h4 - y4;
  x5 = h5 - y5;
  x6 = h6 - y6;
  y6 *= scale;
  x2 += y0;
  x3 += y1;
  x4 += y2;
  x0 += y6;
  x5 += y3;
  x6 += y4;
  x2 += x3;
  x0 += x1;
  x4 += x5;
  x6 += y5;
  x2 += offset1;
  *(double *) &d1 = x2;
  x0 += offset0;
  *(double *) &d0 = x0;
  x4 += offset2;
  *(double *) &d2 = x4;
  x6 += offset3;
  *(double *) &d3 = x6;
  f0 = d0;
  f1 = d1;
  bits32 = -1;
  f2 = d2;
  bits32 >>= 32;
  f3 = d3;
  f = f0 >> 32;
  f0 &= bits32;
  f &= 255;
  f1 += f;
  g0 = f0 + 5;
  g = g0 >> 32;
  g0 &= bits32;
  f = f1 >> 32;
  f1 &= bits32;
  f &= 255;
  g1 = f1 + g;
  g = g1 >> 32;
  f2 += f;
  f = f2 >> 32;
  g1 &= bits32;
  f2 &= bits32;
  f &= 255;
  f3 += f;
  g2 = f2 + g;
  g = g2 >> 32;
  g2 &= bits32;
  f4 = f3 >> 32;
  f3 &= bits32;
  f4 &= 255;
  g3 = f3 + g;
  g = g3 >> 32;
  g3 &= bits32;
  g4 = f4 + g;
  g4 = g4 - 4;
  s00 = *(unsigned char *) (s + 0);
  f = (long long) g4 >> 63;
  s01 = *(unsigned char *) (s + 1);
  f0 &= f;
  g0 &= ~f;
  s02 = *(unsigned char *) (s + 2);
  f1 &= f;
  f0 |= g0;
  s03 = *(unsigned char *) (s + 3);
  g1 &= ~f;
  f2 &= f;
  s10 = *(unsigned char *) (s + 4);
  f3 &= f;
  g2 &= ~f;
  s11 = *(unsigned char *) (s + 5);
  g3 &= ~f;
  f1 |= g1;
  s12 = *(unsigned char *) (s + 6);
  f2 |= g2;
  f3 |= g3;
  s13 = *(unsigned char *) (s + 7);
  s01 <<= 8;
  f0 += s00;
  s20 = *(unsigned char *) (s + 8);
  s02 <<= 16;
  f0 += s01;
  s21 = *(unsigned char *) (s + 9);
  s03 <<= 24;
  f0 += s02;
  s22 = *(unsigned char *) (s + 10);
  s11 <<= 8;
  f1 += s10;
  s23 = *(unsigned char *) (s + 11);
  s12 <<= 16;
  f1 += s11;
  s30 = *(unsigned char *) (s + 12);
  s13 <<= 24;
  f1 += s12;
  s31 = *(unsigned char *) (s + 13);
  f0 += s03;
  f1 += s13;
  s32 = *(unsigned char *) (s + 14);
  s21 <<= 8;
  f2 += s20;
  s33 = *(unsigned char *) (s + 15);
  s22 <<= 16;
  f2 += s21;
  s23 <<= 24;
  f2 += s22;
  s31 <<= 8;
  f3 += s30;
  s32 <<= 16;
  f3 += s31;
  s33 <<= 24;
  f3 += s32;
  f2 += s23;
  f3 += s33;
  *(unsigned char *) (out + 0) = f0;
  f0 >>= 8;
  *(unsigned char *) (out + 1) = f0;
  f0 >>= 8;
  *(unsigned char *) (out + 2) = f0;
  f0 >>= 8;
  *(unsigned char *) (out + 3) = f0;
  f0 >>= 8;
  f1 += f0;
  *(unsigned char *) (out + 4) = f1;
  f1 >>= 8;
  *(unsigned char *) (out + 5) = f1;
  f1 >>= 8;
  *(unsigned char *) (out + 6) = f1;
  f1 >>= 8;
  *(unsigned char *) (out + 7) = f1;
  f1 >>= 8;
  f2 += f1;
  *(unsigned char *) (out + 8) = f2;
  f2 >>= 8;
  *(unsigned char *) (out + 9) = f2;
  f2 >>= 8;
  *(unsigned char *) (out + 10) = f2;
  f2 >>= 8;
  *(unsigned char *) (out + 11) = f2;
  f2 >>= 8;
  f3 += f2;
  *(unsigned char *) (out + 12) = f3;
  f3 >>= 8;
  *(unsigned char *) (out + 13) = f3;
  f3 >>= 8;
  *(unsigned char *) (out + 14) = f3;
  f3 >>= 8;
  *(unsigned char *) (out + 15) = f3;
}
const double poly1305_53_constants[] = {
  0.00000000558793544769287109375
, 24.0
, 103079215104.0
, 6755399441055744.0
, 1770887431076116955136.0
, 29014219670751100192948224.0
, 7605903601369376408980219232256.0
, 124615124604835863084731911901282304.0
, 32667107224410092492483962313449748299776.0
, 535217884764734955396857238543560676143529984.0
, 35076039295941670036888435985190792471742381031424.0
, 9194973245195333150150082162901855101712434733101613056.0
, 0.0000000000000000000000000000000000000036734198463196484624023016788195177431833298649127735047148490821200539357960224151611328125
, 6755408030990331.0
, 29014256564239239022116864.0
, 124615283061160854719918951570079744.0
, 535219245894202480694386063513315216128475136.0
} ;
extern void aes_big(unsigned char out[16],
  const unsigned char k[16],
  const unsigned char n[16]);
extern const unsigned int aes_big_constants[1034];
void aes_big(unsigned char out[16],
  const unsigned char k[16],
  const unsigned char n[16]
)
{
  register int loop4;
  register char *table0;
  register char *table1;
  register char *table2;
  register char *table3;
  register unsigned int x0;
  register unsigned int x1;
  register unsigned int x2;
  register unsigned int x3;
  register unsigned int y0;
  register unsigned int y1;
  register unsigned int y2;
  register unsigned int y3;
  register unsigned int byte0;
  register unsigned int byte1;
  register unsigned int byte2;
  register unsigned int byte3;
  register unsigned int e;
  register unsigned int p00;
  register unsigned int p01;
  register unsigned int p02;
  register unsigned int p03;
  register unsigned int z0;
  register unsigned int z1;
  register unsigned int z2;
  register unsigned int z3;
  register unsigned int p10;
  register unsigned int p11;
  register unsigned int p12;
  register unsigned int p13;
  register unsigned int p20;
  register unsigned int p21;
  register unsigned int p22;
  register unsigned int p23;
  register unsigned int p30;
  register unsigned int p31;
  register unsigned int p32;
  register unsigned int p33;
  register unsigned int q0;
  register unsigned int q1;
  register unsigned int q2;
  register unsigned int q3;
  register unsigned int k00;
  register unsigned int k01;
  register unsigned int k02;
  register unsigned int k03;
  register unsigned int k10;
  register unsigned int k11;
  register unsigned int k12;
  register unsigned int k13;
  register unsigned int k20;
  register unsigned int k21;
  register unsigned int k22;
  register unsigned int k23;
  register unsigned int k30;
  register unsigned int k31;
  register unsigned int k32;
  register unsigned int k33;
  register unsigned int n00;
  register unsigned int n01;
  register unsigned int n02;
  register unsigned int n03;
  register unsigned int n10;
  register unsigned int n11;
  register unsigned int n12;
  register unsigned int n13;
  register unsigned int n20;
  register unsigned int n21;
  register unsigned int n22;
  register unsigned int n23;
  register unsigned int n30;
  register unsigned int n31;
  register unsigned int n32;
  register unsigned int n33;
  table0 = (char *) &aes_big_constants[10];
  table1 = table0 + 8;
  table2 = table0 + 4;
  table3 = table0 + 12;
  byte0 = 0xff;
  byte1 = 0xff00;
  byte2 = 0xff0000;
  byte3 = 0xff000000;
  loop4 = -36;
  k30 = *(unsigned char *) (k + 15);
  k31 = *(unsigned char *) (k + 14);
  k32 = *(unsigned char *) (k + 13);
  k33 = *(unsigned char *) (k + 12);
  k31 <<= 8;
  k32 <<= 16;
  k33 <<= 24;
  x3 = k30 ^ k31;
  x3 ^= k32;
  x3 ^= k33;
  k00 = *(unsigned char *) (k + 3);
  k01 = *(unsigned char *) (k + 2);
  k02 = *(unsigned char *) (k + 1);
  k03 = *(unsigned char *) (k + 0);
  k01 <<= 8;
  k02 <<= 16;
  k03 <<= 24;
  x0 = k00 ^ k01;
  x0 ^= k02;
  x0 ^= k03;
  k10 = *(unsigned char *) (k + 7);
  k11 = *(unsigned char *) (k + 6);
  k12 = *(unsigned char *) (k + 5);
  k13 = *(unsigned char *) (k + 4);
  k11 <<= 8;
  k12 <<= 16;
  k13 <<= 24;
  x1 = k10 ^ k11;
  x1 ^= k12;
  x1 ^= k13;
  k20 = *(unsigned char *) (k + 11);
  k21 = *(unsigned char *) (k + 10);
  k22 = *(unsigned char *) (k + 9);
  k23 = *(unsigned char *) (k + 8);
  k21 <<= 8;
  k22 <<= 16;
  k23 <<= 24;
  x2 = k20 ^ k21;
  x2 ^= k22;
  x2 ^= k23;
  q0 = x3 >> 12;
  q1 = x3 >> 4;
  q2 = x3 << 4;
  q3 = x3 >> 20;
  q0 &= 4080;
  q1 &= 4080;
  q2 &= 4080;
  q3 &= 4080;
  q0 = *(unsigned int *) (table2 + q0);
  q1 = *(unsigned int *) (table3 + q1);
  q2 = *(unsigned int *) (table0 + q2);
  q3 = *(unsigned int *) (table1 + q3);
  q0 &= byte3;
  q1 &= byte2;
  q2 &= byte1;
  q3 &= byte0;
  e = q0 ^ aes_big_constants[0];
  e ^= q1;
  e ^= q2;
  n00 = *(unsigned char *) (n + 3);
  n01 = *(unsigned char *) (n + 2);
  n02 = *(unsigned char *) (n + 1);
  n03 = *(unsigned char *) (n + 0);
  n01 <<= 8;
  n02 <<= 16;
  n03 <<= 24;
  y0 = x0 ^ n00;
  y0 ^= n01;
  y0 ^= n02;
  y0 ^= n03;
  n10 = *(unsigned char *) (n + 7);
  n11 = *(unsigned char *) (n + 6);
  n12 = *(unsigned char *) (n + 5);
  n13 = *(unsigned char *) (n + 4);
  n11 <<= 8;
  n12 <<= 16;
  n13 <<= 24;
  y1 = x1 ^ n10;
  y1 ^= n11;
  y1 ^= n12;
  y1 ^= n13;
  n20 = *(unsigned char *) (n + 11);
  n21 = *(unsigned char *) (n + 10);
  n22 = *(unsigned char *) (n + 9);
  n23 = *(unsigned char *) (n + 8);
  n21 <<= 8;
  n22 <<= 16;
  n23 <<= 24;
  y2 = x2 ^ n20;
  y2 ^= n21;
  y2 ^= n22;
  y2 ^= n23;
  n30 = *(unsigned char *) (n + 15);
  n31 = *(unsigned char *) (n + 14);
  n32 = *(unsigned char *) (n + 13);
  n33 = *(unsigned char *) (n + 12);
  n31 <<= 8;
  n32 <<= 16;
  n33 <<= 24;
  y3 = x3 ^ n30;
  y3 ^= n31;
  y3 ^= n32;
  y3 ^= n33;
  do {
    e ^= q3;
    x0 ^= e;
    x1 ^= x0;
    x2 ^= x1;
    x3 ^= x2;
    p00 = y0 >> 20;
    p01 = y0 >> 12;
    p02 = y0 >> 4;
    p03 = y0 << 4;
    p00 &= 4080;
    p01 &= 4080;
    p02 &= 4080;
    p03 &= 4080;
    p00 = *(unsigned int *) (table0 + p00);
    p01 = *(unsigned int *) (table1 + p01);
    p02 = *(unsigned int *) (table2 + p02);
    p03 = *(unsigned int *) (table3 + p03);
    z0 = x0 ^ p00;
    z3 = x3 ^ p01;
    z2 = x2 ^ p02;
    z1 = x1 ^ p03;
    p10 = y1 >> 20;
    p11 = y1 >> 12;
    p12 = y1 >> 4;
    p13 = y1 << 4;
    p10 &= 4080;
    p11 &= 4080;
    p12 &= 4080;
    p13 &= 4080;
    p10 = *(unsigned int *) (table0 + p10);
    p11 = *(unsigned int *) (table1 + p11);
    p12 = *(unsigned int *) (table2 + p12);
    p13 = *(unsigned int *) (table3 + p13);
    z1 ^= p10;
    z0 ^= p11;
    z3 ^= p12;
    z2 ^= p13;
    p20 = y2 >> 20;
    p21 = y2 >> 12;
    p22 = y2 >> 4;
    p23 = y2 << 4;
    p20 &= 4080;
    p21 &= 4080;
    p22 &= 4080;
    p23 &= 4080;
    p20 = *(unsigned int *) (table0 + p20);
    p21 = *(unsigned int *) (table1 + p21);
    p22 = *(unsigned int *) (table2 + p22);
    p23 = *(unsigned int *) (table3 + p23);
    z2 ^= p20;
    z1 ^= p21;
    z0 ^= p22;
    z3 ^= p23;
    p31 = y3 >> 12;
    p32 = y3 >> 4;
    p33 = y3 << 4;
    p30 = y3 >> 20;
    p31 &= 4080;
    p32 &= 4080;
    p33 &= 4080;
    p30 &= 4080;
    p31 = *(unsigned int *) (table1 + p31);
    p32 = *(unsigned int *) (table2 + p32);
    p33 = *(unsigned int *) (table3 + p33);
    p30 = *(unsigned int *) (table0 + p30);
    y2 = z2 ^ p31;
    y1 = z1 ^ p32;
    y0 = z0 ^ p33;
    y3 = z3 ^ p30;
    e = *(unsigned int *) (table0 + loop4);
    q0 = x3 >> 12;
    q1 = x3 >> 4;
    q2 = x3 << 4;
    q3 = x3 >> 20;
    q0 &= 4080;
    q1 &= 4080;
    q2 &= 4080;
    q3 &= 4080;
    q0 = *(unsigned int *) (table2 + q0);
    q1 = *(unsigned int *) (table3 + q1);
    q2 = *(unsigned int *) (table0 + q2);
    q3 = *(unsigned int *) (table1 + q3);
    q0 &= byte3;
    q1 &= byte2;
    q2 &= byte1;
    q3 &= byte0;
    e ^= q0;
    e ^= q1;
    e ^= q2;
    loop4 += 4;
  } while (loop4);
  e = e ^ q3;
  x0 ^= e;
  x1 ^= x0;
  x2 ^= x1;
  x3 ^= x2;
  p00 = y0 >> 20;
  p01 = y0 >> 12;
  p02 = y0 >> 4;
  p03 = y0 << 4;
  p00 &= 4080;
  p01 &= 4080;
  p02 &= 4080;
  p03 &= 4080;
  p00 = *(unsigned int *) (table2 + p00);
  p01 = *(unsigned int *) (table3 + p01);
  p02 = *(unsigned int *) (table0 + p02);
  p03 = *(unsigned int *) (table1 + p03);
  p00 &= byte3;
  p01 &= byte2;
  p02 &= byte1;
  p03 &= byte0;
  z0 = x0 ^ p00;
  z3 = x3 ^ p01;
  z2 = x2 ^ p02;
  z1 = x1 ^ p03;
  p10 = y1 >> 20;
  p11 = y1 >> 12;
  p12 = y1 >> 4;
  p13 = y1 << 4;
  p10 &= 4080;
  p11 &= 4080;
  p12 &= 4080;
  p13 &= 4080;
  p10 = *(unsigned int *) (table2 + p10);
  p11 = *(unsigned int *) (table3 + p11);
  p12 = *(unsigned int *) (table0 + p12);
  p13 = *(unsigned int *) (table1 + p13);
  p10 &= byte3;
  p11 &= byte2;
  p12 &= byte1;
  p13 &= byte0;
  z1 ^= p10;
  z0 ^= p11;
  z3 ^= p12;
  z2 ^= p13;
  p20 = y2 >> 20;
  p21 = y2 >> 12;
  p22 = y2 >> 4;
  p23 = y2 << 4;
  p20 &= 4080;
  p21 &= 4080;
  p22 &= 4080;
  p23 &= 4080;
  p20 = *(unsigned int *) (table2 + p20);
  p21 = *(unsigned int *) (table3 + p21);
  p22 = *(unsigned int *) (table0 + p22);
  p23 = *(unsigned int *) (table1 + p23);
  p20 &= byte3;
  p21 &= byte2;
  p22 &= byte1;
  p23 &= byte0;
  z3 ^= p23;
  z2 ^= p20;
  z1 ^= p21;
  z0 ^= p22;
  p30 = y3 >> 20;
  p31 = y3 >> 12;
  p32 = y3 >> 4;
  p33 = y3 << 4;
  p30 &= 4080;
  p31 &= 4080;
  p32 &= 4080;
  p33 &= 4080;
  p30 = *(unsigned int *) (table2 + p30);
  p31 = *(unsigned int *) (table3 + p31);
  p32 = *(unsigned int *) (table0 + p32);
  p33 = *(unsigned int *) (table1 + p33);
  p30 &= byte3;
  p31 &= byte2;
  p32 &= byte1;
  p33 &= byte0;
  y3 = z3 ^ p30;
  y2 = z2 ^ p31;
  y1 = z1 ^ p32;
  y0 = z0 ^ p33;
  *(unsigned char *) (out + 3) = y0;
  y0 >>= 8;
  *(unsigned char *) (out + 2) = y0;
  y0 >>= 8;
  *(unsigned char *) (out + 1) = y0;
  y0 >>= 8;
  *(unsigned char *) (out + 0) = y0;
  *(unsigned char *) (out + 7) = y1;
  y1 >>= 8;
  *(unsigned char *) (out + 6) = y1;
  y1 >>= 8;
  *(unsigned char *) (out + 5) = y1;
  y1 >>= 8;
  *(unsigned char *) (out + 4) = y1;
  *(unsigned char *) (out + 11) = y2;
  y2 >>= 8;
  *(unsigned char *) (out + 10) = y2;
  y2 >>= 8;
  *(unsigned char *) (out + 9) = y2;
  y2 >>= 8;
  *(unsigned char *) (out + 8) = y2;
  *(unsigned char *) (out + 15) = y3;
  y3 >>= 8;
  *(unsigned char *) (out + 14) = y3;
  y3 >>= 8;
  *(unsigned char *) (out + 13) = y3;
  y3 >>= 8;
  *(unsigned char *) (out + 12) = y3;
}
extern void aes_big(unsigned char out[16],
  const unsigned char k[16],
  const unsigned char n[16]);
extern const unsigned int aes_big_constants[1034];
const unsigned int aes_big_constants[1034] = {
0x01000000,
0x02000000,
0x04000000,
0x08000000,
0x10000000,
0x20000000,
0x40000000,
0x80000000,
0x1b000000,
0x36000000,
0xc66363a5, 0x63a5c663, 0xa5c66363, 0x6363a5c6,
0xf87c7c84, 0x7c84f87c, 0x84f87c7c, 0x7c7c84f8,
0xee777799, 0x7799ee77, 0x99ee7777, 0x777799ee,
0xf67b7b8d, 0x7b8df67b, 0x8df67b7b, 0x7b7b8df6,
0xfff2f20d, 0xf20dfff2, 0x0dfff2f2, 0xf2f20dff,
0xd66b6bbd, 0x6bbdd66b, 0xbdd66b6b, 0x6b6bbdd6,
0xde6f6fb1, 0x6fb1de6f, 0xb1de6f6f, 0x6f6fb1de,
0x91c5c554, 0xc55491c5, 0x5491c5c5, 0xc5c55491,
0x60303050, 0x30506030, 0x50603030, 0x30305060,
0x02010103, 0x01030201, 0x03020101, 0x01010302,
0xce6767a9, 0x67a9ce67, 0xa9ce6767, 0x6767a9ce,
0x562b2b7d, 0x2b7d562b, 0x7d562b2b, 0x2b2b7d56,
0xe7fefe19, 0xfe19e7fe, 0x19e7fefe, 0xfefe19e7,
0xb5d7d762, 0xd762b5d7, 0x62b5d7d7, 0xd7d762b5,
0x4dababe6, 0xabe64dab, 0xe64dabab, 0xababe64d,
0xec76769a, 0x769aec76, 0x9aec7676, 0x76769aec,
0x8fcaca45, 0xca458fca, 0x458fcaca, 0xcaca458f,
0x1f82829d, 0x829d1f82, 0x9d1f8282, 0x82829d1f,
0x89c9c940, 0xc94089c9, 0x4089c9c9, 0xc9c94089,
0xfa7d7d87, 0x7d87fa7d, 0x87fa7d7d, 0x7d7d87fa,
0xeffafa15, 0xfa15effa, 0x15effafa, 0xfafa15ef,
0xb25959eb, 0x59ebb259, 0xebb25959, 0x5959ebb2,
0x8e4747c9, 0x47c98e47, 0xc98e4747, 0x4747c98e,
0xfbf0f00b, 0xf00bfbf0, 0x0bfbf0f0, 0xf0f00bfb,
0x41adadec, 0xadec41ad, 0xec41adad, 0xadadec41,
0xb3d4d467, 0xd467b3d4, 0x67b3d4d4, 0xd4d467b3,
0x5fa2a2fd, 0xa2fd5fa2, 0xfd5fa2a2, 0xa2a2fd5f,
0x45afafea, 0xafea45af, 0xea45afaf, 0xafafea45,
0x239c9cbf, 0x9cbf239c, 0xbf239c9c, 0x9c9cbf23,
0x53a4a4f7, 0xa4f753a4, 0xf753a4a4, 0xa4a4f753,
0xe4727296, 0x7296e472, 0x96e47272, 0x727296e4,
0x9bc0c05b, 0xc05b9bc0, 0x5b9bc0c0, 0xc0c05b9b,
0x75b7b7c2, 0xb7c275b7, 0xc275b7b7, 0xb7b7c275,
0xe1fdfd1c, 0xfd1ce1fd, 0x1ce1fdfd, 0xfdfd1ce1,
0x3d9393ae, 0x93ae3d93, 0xae3d9393, 0x9393ae3d,
0x4c26266a, 0x266a4c26, 0x6a4c2626, 0x26266a4c,
0x6c36365a, 0x365a6c36, 0x5a6c3636, 0x36365a6c,
0x7e3f3f41, 0x3f417e3f, 0x417e3f3f, 0x3f3f417e,
0xf5f7f702, 0xf702f5f7, 0x02f5f7f7, 0xf7f702f5,
0x83cccc4f, 0xcc4f83cc, 0x4f83cccc, 0xcccc4f83,
0x6834345c, 0x345c6834, 0x5c683434, 0x34345c68,
0x51a5a5f4, 0xa5f451a5, 0xf451a5a5, 0xa5a5f451,
0xd1e5e534, 0xe534d1e5, 0x34d1e5e5, 0xe5e534d1,
0xf9f1f108, 0xf108f9f1, 0x08f9f1f1, 0xf1f108f9,
0xe2717193, 0x7193e271, 0x93e27171, 0x717193e2,
0xabd8d873, 0xd873abd8, 0x73abd8d8, 0xd8d873ab,
0x62313153, 0x31536231, 0x53623131, 0x31315362,
0x2a15153f, 0x153f2a15, 0x3f2a1515, 0x15153f2a,
0x0804040c, 0x040c0804, 0x0c080404, 0x04040c08,
0x95c7c752, 0xc75295c7, 0x5295c7c7, 0xc7c75295,
0x46232365, 0x23654623, 0x65462323, 0x23236546,
0x9dc3c35e, 0xc35e9dc3, 0x5e9dc3c3, 0xc3c35e9d,
0x30181828, 0x18283018, 0x28301818, 0x18182830,
0x379696a1, 0x96a13796, 0xa1379696, 0x9696a137,
0x0a05050f, 0x050f0a05, 0x0f0a0505, 0x05050f0a,
0x2f9a9ab5, 0x9ab52f9a, 0xb52f9a9a, 0x9a9ab52f,
0x0e070709, 0x07090e07, 0x090e0707, 0x0707090e,
0x24121236, 0x12362412, 0x36241212, 0x12123624,
0x1b80809b, 0x809b1b80, 0x9b1b8080, 0x80809b1b,
0xdfe2e23d, 0xe23ddfe2, 0x3ddfe2e2, 0xe2e23ddf,
0xcdebeb26, 0xeb26cdeb, 0x26cdebeb, 0xebeb26cd,
0x4e272769, 0x27694e27, 0x694e2727, 0x2727694e,
0x7fb2b2cd, 0xb2cd7fb2, 0xcd7fb2b2, 0xb2b2cd7f,
0xea75759f, 0x759fea75, 0x9fea7575, 0x75759fea,
0x1209091b, 0x091b1209, 0x1b120909, 0x09091b12,
0x1d83839e, 0x839e1d83, 0x9e1d8383, 0x83839e1d,
0x582c2c74, 0x2c74582c, 0x74582c2c, 0x2c2c7458,
0x341a1a2e, 0x1a2e341a, 0x2e341a1a, 0x1a1a2e34,
0x361b1b2d, 0x1b2d361b, 0x2d361b1b, 0x1b1b2d36,
0xdc6e6eb2, 0x6eb2dc6e, 0xb2dc6e6e, 0x6e6eb2dc,
0xb45a5aee, 0x5aeeb45a, 0xeeb45a5a, 0x5a5aeeb4,
0x5ba0a0fb, 0xa0fb5ba0, 0xfb5ba0a0, 0xa0a0fb5b,
0xa45252f6, 0x52f6a452, 0xf6a45252, 0x5252f6a4,
0x763b3b4d, 0x3b4d763b, 0x4d763b3b, 0x3b3b4d76,
0xb7d6d661, 0xd661b7d6, 0x61b7d6d6, 0xd6d661b7,
0x7db3b3ce, 0xb3ce7db3, 0xce7db3b3, 0xb3b3ce7d,
0x5229297b, 0x297b5229, 0x7b522929, 0x29297b52,
0xdde3e33e, 0xe33edde3, 0x3edde3e3, 0xe3e33edd,
0x5e2f2f71, 0x2f715e2f, 0x715e2f2f, 0x2f2f715e,
0x13848497, 0x84971384, 0x97138484, 0x84849713,
0xa65353f5, 0x53f5a653, 0xf5a65353, 0x5353f5a6,
0xb9d1d168, 0xd168b9d1, 0x68b9d1d1, 0xd1d168b9,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0xc1eded2c, 0xed2cc1ed, 0x2cc1eded, 0xeded2cc1,
0x40202060, 0x20604020, 0x60402020, 0x20206040,
0xe3fcfc1f, 0xfc1fe3fc, 0x1fe3fcfc, 0xfcfc1fe3,
0x79b1b1c8, 0xb1c879b1, 0xc879b1b1, 0xb1b1c879,
0xb65b5bed, 0x5bedb65b, 0xedb65b5b, 0x5b5bedb6,
0xd46a6abe, 0x6abed46a, 0xbed46a6a, 0x6a6abed4,
0x8dcbcb46, 0xcb468dcb, 0x468dcbcb, 0xcbcb468d,
0x67bebed9, 0xbed967be, 0xd967bebe, 0xbebed967,
0x7239394b, 0x394b7239, 0x4b723939, 0x39394b72,
0x944a4ade, 0x4ade944a, 0xde944a4a, 0x4a4ade94,
0x984c4cd4, 0x4cd4984c, 0xd4984c4c, 0x4c4cd498,
0xb05858e8, 0x58e8b058, 0xe8b05858, 0x5858e8b0,
0x85cfcf4a, 0xcf4a85cf, 0x4a85cfcf, 0xcfcf4a85,
0xbbd0d06b, 0xd06bbbd0, 0x6bbbd0d0, 0xd0d06bbb,
0xc5efef2a, 0xef2ac5ef, 0x2ac5efef, 0xefef2ac5,
0x4faaaae5, 0xaae54faa, 0xe54faaaa, 0xaaaae54f,
0xedfbfb16, 0xfb16edfb, 0x16edfbfb, 0xfbfb16ed,
0x864343c5, 0x43c58643, 0xc5864343, 0x4343c586,
0x9a4d4dd7, 0x4dd79a4d, 0xd79a4d4d, 0x4d4dd79a,
0x66333355, 0x33556633, 0x55663333, 0x33335566,
0x11858594, 0x85941185, 0x94118585, 0x85859411,
0x8a4545cf, 0x45cf8a45, 0xcf8a4545, 0x4545cf8a,
0xe9f9f910, 0xf910e9f9, 0x10e9f9f9, 0xf9f910e9,
0x04020206, 0x02060402, 0x06040202, 0x02020604,
0xfe7f7f81, 0x7f81fe7f, 0x81fe7f7f, 0x7f7f81fe,
0xa05050f0, 0x50f0a050, 0xf0a05050, 0x5050f0a0,
0x783c3c44, 0x3c44783c, 0x44783c3c, 0x3c3c4478,
0x259f9fba, 0x9fba259f, 0xba259f9f, 0x9f9fba25,
0x4ba8a8e3, 0xa8e34ba8, 0xe34ba8a8, 0xa8a8e34b,
0xa25151f3, 0x51f3a251, 0xf3a25151, 0x5151f3a2,
0x5da3a3fe, 0xa3fe5da3, 0xfe5da3a3, 0xa3a3fe5d,
0x804040c0, 0x40c08040, 0xc0804040, 0x4040c080,
0x058f8f8a, 0x8f8a058f, 0x8a058f8f, 0x8f8f8a05,
0x3f9292ad, 0x92ad3f92, 0xad3f9292, 0x9292ad3f,
0x219d9dbc, 0x9dbc219d, 0xbc219d9d, 0x9d9dbc21,
0x70383848, 0x38487038, 0x48703838, 0x38384870,
0xf1f5f504, 0xf504f1f5, 0x04f1f5f5, 0xf5f504f1,
0x63bcbcdf, 0xbcdf63bc, 0xdf63bcbc, 0xbcbcdf63,
0x77b6b6c1, 0xb6c177b6, 0xc177b6b6, 0xb6b6c177,
0xafdada75, 0xda75afda, 0x75afdada, 0xdada75af,
0x42212163, 0x21634221, 0x63422121, 0x21216342,
0x20101030, 0x10302010, 0x30201010, 0x10103020,
0xe5ffff1a, 0xff1ae5ff, 0x1ae5ffff, 0xffff1ae5,
0xfdf3f30e, 0xf30efdf3, 0x0efdf3f3, 0xf3f30efd,
0xbfd2d26d, 0xd26dbfd2, 0x6dbfd2d2, 0xd2d26dbf,
0x81cdcd4c, 0xcd4c81cd, 0x4c81cdcd, 0xcdcd4c81,
0x180c0c14, 0x0c14180c, 0x14180c0c, 0x0c0c1418,
0x26131335, 0x13352613, 0x35261313, 0x13133526,
0xc3ecec2f, 0xec2fc3ec, 0x2fc3ecec, 0xecec2fc3,
0xbe5f5fe1, 0x5fe1be5f, 0xe1be5f5f, 0x5f5fe1be,
0x359797a2, 0x97a23597, 0xa2359797, 0x9797a235,
0x884444cc, 0x44cc8844, 0xcc884444, 0x4444cc88,
0x2e171739, 0x17392e17, 0x392e1717, 0x1717392e,
0x93c4c457, 0xc45793c4, 0x5793c4c4, 0xc4c45793,
0x55a7a7f2, 0xa7f255a7, 0xf255a7a7, 0xa7a7f255,
0xfc7e7e82, 0x7e82fc7e, 0x82fc7e7e, 0x7e7e82fc,
0x7a3d3d47, 0x3d477a3d, 0x477a3d3d, 0x3d3d477a,
0xc86464ac, 0x64acc864, 0xacc86464, 0x6464acc8,
0xba5d5de7, 0x5de7ba5d, 0xe7ba5d5d, 0x5d5de7ba,
0x3219192b, 0x192b3219, 0x2b321919, 0x19192b32,
0xe6737395, 0x7395e673, 0x95e67373, 0x737395e6,
0xc06060a0, 0x60a0c060, 0xa0c06060, 0x6060a0c0,
0x19818198, 0x81981981, 0x98198181, 0x81819819,
0x9e4f4fd1, 0x4fd19e4f, 0xd19e4f4f, 0x4f4fd19e,
0xa3dcdc7f, 0xdc7fa3dc, 0x7fa3dcdc, 0xdcdc7fa3,
0x44222266, 0x22664422, 0x66442222, 0x22226644,
0x542a2a7e, 0x2a7e542a, 0x7e542a2a, 0x2a2a7e54,
0x3b9090ab, 0x90ab3b90, 0xab3b9090, 0x9090ab3b,
0x0b888883, 0x88830b88, 0x830b8888, 0x8888830b,
0x8c4646ca, 0x46ca8c46, 0xca8c4646, 0x4646ca8c,
0xc7eeee29, 0xee29c7ee, 0x29c7eeee, 0xeeee29c7,
0x6bb8b8d3, 0xb8d36bb8, 0xd36bb8b8, 0xb8b8d36b,
0x2814143c, 0x143c2814, 0x3c281414, 0x14143c28,
0xa7dede79, 0xde79a7de, 0x79a7dede, 0xdede79a7,
0xbc5e5ee2, 0x5ee2bc5e, 0xe2bc5e5e, 0x5e5ee2bc,
0x160b0b1d, 0x0b1d160b, 0x1d160b0b, 0x0b0b1d16,
0xaddbdb76, 0xdb76addb, 0x76addbdb, 0xdbdb76ad,
0xdbe0e03b, 0xe03bdbe0, 0x3bdbe0e0, 0xe0e03bdb,
0x64323256, 0x32566432, 0x56643232, 0x32325664,
0x743a3a4e, 0x3a4e743a, 0x4e743a3a, 0x3a3a4e74,
0x140a0a1e, 0x0a1e140a, 0x1e140a0a, 0x0a0a1e14,
0x924949db, 0x49db9249, 0xdb924949, 0x4949db92,
0x0c06060a, 0x060a0c06, 0x0a0c0606, 0x06060a0c,
0x4824246c, 0x246c4824, 0x6c482424, 0x24246c48,
0xb85c5ce4, 0x5ce4b85c, 0xe4b85c5c, 0x5c5ce4b8,
0x9fc2c25d, 0xc25d9fc2, 0x5d9fc2c2, 0xc2c25d9f,
0xbdd3d36e, 0xd36ebdd3, 0x6ebdd3d3, 0xd3d36ebd,
0x43acacef, 0xacef43ac, 0xef43acac, 0xacacef43,
0xc46262a6, 0x62a6c462, 0xa6c46262, 0x6262a6c4,
0x399191a8, 0x91a83991, 0xa8399191, 0x9191a839,
0x319595a4, 0x95a43195, 0xa4319595, 0x9595a431,
0xd3e4e437, 0xe437d3e4, 0x37d3e4e4, 0xe4e437d3,
0xf279798b, 0x798bf279, 0x8bf27979, 0x79798bf2,
0xd5e7e732, 0xe732d5e7, 0x32d5e7e7, 0xe7e732d5,
0x8bc8c843, 0xc8438bc8, 0x438bc8c8, 0xc8c8438b,
0x6e373759, 0x37596e37, 0x596e3737, 0x3737596e,
0xda6d6db7, 0x6db7da6d, 0xb7da6d6d, 0x6d6db7da,
0x018d8d8c, 0x8d8c018d, 0x8c018d8d, 0x8d8d8c01,
0xb1d5d564, 0xd564b1d5, 0x64b1d5d5, 0xd5d564b1,
0x9c4e4ed2, 0x4ed29c4e, 0xd29c4e4e, 0x4e4ed29c,
0x49a9a9e0, 0xa9e049a9, 0xe049a9a9, 0xa9a9e049,
0xd86c6cb4, 0x6cb4d86c, 0xb4d86c6c, 0x6c6cb4d8,
0xac5656fa, 0x56faac56, 0xfaac5656, 0x5656faac,
0xf3f4f407, 0xf407f3f4, 0x07f3f4f4, 0xf4f407f3,
0xcfeaea25, 0xea25cfea, 0x25cfeaea, 0xeaea25cf,
0xca6565af, 0x65afca65, 0xafca6565, 0x6565afca,
0xf47a7a8e, 0x7a8ef47a, 0x8ef47a7a, 0x7a7a8ef4,
0x47aeaee9, 0xaee947ae, 0xe947aeae, 0xaeaee947,
0x10080818, 0x08181008, 0x18100808, 0x08081810,
0x6fbabad5, 0xbad56fba, 0xd56fbaba, 0xbabad56f,
0xf0787888, 0x7888f078, 0x88f07878, 0x787888f0,
0x4a25256f, 0x256f4a25, 0x6f4a2525, 0x25256f4a,
0x5c2e2e72, 0x2e725c2e, 0x725c2e2e, 0x2e2e725c,
0x381c1c24, 0x1c24381c, 0x24381c1c, 0x1c1c2438,
0x57a6a6f1, 0xa6f157a6, 0xf157a6a6, 0xa6a6f157,
0x73b4b4c7, 0xb4c773b4, 0xc773b4b4, 0xb4b4c773,
0x97c6c651, 0xc65197c6, 0x5197c6c6, 0xc6c65197,
0xcbe8e823, 0xe823cbe8, 0x23cbe8e8, 0xe8e823cb,
0xa1dddd7c, 0xdd7ca1dd, 0x7ca1dddd, 0xdddd7ca1,
0xe874749c, 0x749ce874, 0x9ce87474, 0x74749ce8,
0x3e1f1f21, 0x1f213e1f, 0x213e1f1f, 0x1f1f213e,
0x964b4bdd, 0x4bdd964b, 0xdd964b4b, 0x4b4bdd96,
0x61bdbddc, 0xbddc61bd, 0xdc61bdbd, 0xbdbddc61,
0x0d8b8b86, 0x8b860d8b, 0x860d8b8b, 0x8b8b860d,
0x0f8a8a85, 0x8a850f8a, 0x850f8a8a, 0x8a8a850f,
0xe0707090, 0x7090e070, 0x90e07070, 0x707090e0,
0x7c3e3e42, 0x3e427c3e, 0x427c3e3e, 0x3e3e427c,
0x71b5b5c4, 0xb5c471b5, 0xc471b5b5, 0xb5b5c471,
0xcc6666aa, 0x66aacc66, 0xaacc6666, 0x6666aacc,
0x904848d8, 0x48d89048, 0xd8904848, 0x4848d890,
0x06030305, 0x03050603, 0x05060303, 0x03030506,
0xf7f6f601, 0xf601f7f6, 0x01f7f6f6, 0xf6f601f7,
0x1c0e0e12, 0x0e121c0e, 0x121c0e0e, 0x0e0e121c,
0xc26161a3, 0x61a3c261, 0xa3c26161, 0x6161a3c2,
0x6a35355f, 0x355f6a35, 0x5f6a3535, 0x35355f6a,
0xae5757f9, 0x57f9ae57, 0xf9ae5757, 0x5757f9ae,
0x69b9b9d0, 0xb9d069b9, 0xd069b9b9, 0xb9b9d069,
0x17868691, 0x86911786, 0x91178686, 0x86869117,
0x99c1c158, 0xc15899c1, 0x5899c1c1, 0xc1c15899,
0x3a1d1d27, 0x1d273a1d, 0x273a1d1d, 0x1d1d273a,
0x279e9eb9, 0x9eb9279e, 0xb9279e9e, 0x9e9eb927,
0xd9e1e138, 0xe138d9e1, 0x38d9e1e1, 0xe1e138d9,
0xebf8f813, 0xf813ebf8, 0x13ebf8f8, 0xf8f813eb,
0x2b9898b3, 0x98b32b98, 0xb32b9898, 0x9898b32b,
0x22111133, 0x11332211, 0x33221111, 0x11113322,
0xd26969bb, 0x69bbd269, 0xbbd26969, 0x6969bbd2,
0xa9d9d970, 0xd970a9d9, 0x70a9d9d9, 0xd9d970a9,
0x078e8e89, 0x8e89078e, 0x89078e8e, 0x8e8e8907,
0x339494a7, 0x94a73394, 0xa7339494, 0x9494a733,
0x2d9b9bb6, 0x9bb62d9b, 0xb62d9b9b, 0x9b9bb62d,
0x3c1e1e22, 0x1e223c1e, 0x223c1e1e, 0x1e1e223c,
0x15878792, 0x87921587, 0x92158787, 0x87879215,
0xc9e9e920, 0xe920c9e9, 0x20c9e9e9, 0xe9e920c9,
0x87cece49, 0xce4987ce, 0x4987cece, 0xcece4987,
0xaa5555ff, 0x55ffaa55, 0xffaa5555, 0x5555ffaa,
0x50282878, 0x28785028, 0x78502828, 0x28287850,
0xa5dfdf7a, 0xdf7aa5df, 0x7aa5dfdf, 0xdfdf7aa5,
0x038c8c8f, 0x8c8f038c, 0x8f038c8c, 0x8c8c8f03,
0x59a1a1f8, 0xa1f859a1, 0xf859a1a1, 0xa1a1f859,
0x09898980, 0x89800989, 0x80098989, 0x89898009,
0x1a0d0d17, 0x0d171a0d, 0x171a0d0d, 0x0d0d171a,
0x65bfbfda, 0xbfda65bf, 0xda65bfbf, 0xbfbfda65,
0xd7e6e631, 0xe631d7e6, 0x31d7e6e6, 0xe6e631d7,
0x844242c6, 0x42c68442, 0xc6844242, 0x4242c684,
0xd06868b8, 0x68b8d068, 0xb8d06868, 0x6868b8d0,
0x824141c3, 0x41c38241, 0xc3824141, 0x4141c382,
0x299999b0, 0x99b02999, 0xb0299999, 0x9999b029,
0x5a2d2d77, 0x2d775a2d, 0x775a2d2d, 0x2d2d775a,
0x1e0f0f11, 0x0f111e0f, 0x111e0f0f, 0x0f0f111e,
0x7bb0b0cb, 0xb0cb7bb0, 0xcb7bb0b0, 0xb0b0cb7b,
0xa85454fc, 0x54fca854, 0xfca85454, 0x5454fca8,
0x6dbbbbd6, 0xbbd66dbb, 0xd66dbbbb, 0xbbbbd66d,
0x2c16163a, 0x163a2c16, 0x3a2c1616, 0x16163a2c,
};
extern void poly1305aes_53_clamp(unsigned char kr[32]);
extern void poly1305aes_53_authenticate(unsigned char out[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_verify(const unsigned char a[16],
  const unsigned char kr[32],
  const unsigned char n[16],
  const unsigned char m[],unsigned int l);
extern int poly1305aes_53_isequal(const unsigned char x[16],
  const unsigned char y[16]);
int poly1305aes_53_isequal(const unsigned char x[16],
  const unsigned char y[16])
{
  register unsigned int d;
  register unsigned int x0;
  register unsigned int x1;
  register unsigned int x2;
  register unsigned int x3;
  register unsigned int x4;
  register unsigned int x5;
  register unsigned int x6;
  register unsigned int x7;
  register unsigned int x8;
  register unsigned int x9;
  register unsigned int x10;
  register unsigned int x11;
  register unsigned int x12;
  register unsigned int x13;
  register unsigned int x14;
  register unsigned int x15;
  register unsigned int y0;
  register unsigned int y1;
  register unsigned int y2;
  register unsigned int y3;
  register unsigned int y4;
  register unsigned int y5;
  register unsigned int y6;
  register unsigned int y7;
  register unsigned int y8;
  register unsigned int y9;
  register unsigned int y10;
  register unsigned int y11;
  register unsigned int y12;
  register unsigned int y13;
  register unsigned int y14;
  register unsigned int y15;
  x0 = *(unsigned char *) (x + 0);
  y0 = *(unsigned char *) (y + 0);
  x1 = *(unsigned char *) (x + 1);
  y1 = *(unsigned char *) (y + 1);
  x2 = *(unsigned char *) (x + 2);
  y2 = *(unsigned char *) (y + 2);
  d = y0 ^ x0;
  x3 = *(unsigned char *) (x + 3);
  y1 ^= x1;
  y3 = *(unsigned char *) (y + 3);
  d |= y1;
  x4 = *(unsigned char *) (x + 4);
  y2 ^= x2;
  y4 = *(unsigned char *) (y + 4);
  d |= y2;
  x5 = *(unsigned char *) (x + 5);
  y3 ^= x3;
  y5 = *(unsigned char *) (y + 5);
  d |= y3;
  x6 = *(unsigned char *) (x + 6);
  y4 ^= x4;
  y6 = *(unsigned char *) (y + 6);
  d |= y4;
  x7 = *(unsigned char *) (x + 7);
  y5 ^= x5;
  y7 = *(unsigned char *) (y + 7);
  d |= y5;
  x8 = *(unsigned char *) (x + 8);
  y6 ^= x6;
  y8 = *(unsigned char *) (y + 8);
  d |= y6;
  x9 = *(unsigned char *) (x + 9);
  y7 ^= x7;
  y9 = *(unsigned char *) (y + 9);
  d |= y7;
  x10 = *(unsigned char *) (x + 10);
  y8 ^= x8;
  y10 = *(unsigned char *) (y + 10);
  d |= y8;
  x11 = *(unsigned char *) (x + 11);
  y9 ^= x9;
  y11 = *(unsigned char *) (y + 11);
  d |= y9;
  x12 = *(unsigned char *) (x + 12);
  y10 ^= x10;
  y12 = *(unsigned char *) (y + 12);
  d |= y10;
  x13 = *(unsigned char *) (x + 13);
  y11 ^= x11;
  y13 = *(unsigned char *) (y + 13);
  d |= y11;
  x14 = *(unsigned char *) (x + 14);
  y12 ^= x12;
  y14 = *(unsigned char *) (y + 14);
  d |= y12;
  x15 = *(unsigned char *) (x + 15);
  y13 ^= x13;
  y15 = *(unsigned char *) (y + 15);
  d |= y13;
  y14 ^= x14;
  d |= y14;
  y15 ^= x15;
  d |= y15;
  d -= 1;
  d >>= 8;
  return d;
}


static PyObject *
_cffi_f_aes_big(PyObject *self, PyObject *args)
{
  unsigned char * x0;
  unsigned char const * x1;
  unsigned char const * x2;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;

  if (!PyArg_ParseTuple(args, "OOO:aes_big", &arg0, &arg1, &arg2))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(1), arg2) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { aes_big(x0, x1, x2); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_poly1305_53(PyObject *self, PyObject *args)
{
  unsigned char * x0;
  unsigned char const * x1;
  unsigned char const * x2;
  unsigned char const * x3;
  unsigned int x4;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:poly1305_53", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(1), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(1), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, unsigned int);
  if (x4 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { poly1305_53(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_poly1305aes_53_authenticate(PyObject *self, PyObject *args)
{
  unsigned char * x0;
  unsigned char const * x1;
  unsigned char const * x2;
  unsigned char const * x3;
  unsigned int x4;
  Py_ssize_t datasize;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:poly1305aes_53_authenticate", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(1), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(1), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, unsigned int);
  if (x4 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { poly1305aes_53_authenticate(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_poly1305aes_53_clamp(PyObject *self, PyObject *arg0)
{
  unsigned char * x0;
  Py_ssize_t datasize;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(0), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(0), arg0) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { poly1305aes_53_clamp(x0); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
_cffi_f_poly1305aes_53_isequal(PyObject *self, PyObject *args)
{
  unsigned char const * x0;
  unsigned char const * x1;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;

  if (!PyArg_ParseTuple(args, "OO:poly1305aes_53_isequal", &arg0, &arg1))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = poly1305aes_53_isequal(x0, x1); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static PyObject *
_cffi_f_poly1305aes_53_verify(PyObject *self, PyObject *args)
{
  unsigned char const * x0;
  unsigned char const * x1;
  unsigned char const * x2;
  unsigned char const * x3;
  unsigned int x4;
  Py_ssize_t datasize;
  int result;
  PyObject *arg0;
  PyObject *arg1;
  PyObject *arg2;
  PyObject *arg3;
  PyObject *arg4;

  if (!PyArg_ParseTuple(args, "OOOOO:poly1305aes_53_verify", &arg0, &arg1, &arg2, &arg3, &arg4))
    return NULL;

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg0, (char **)&x0);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x0 = alloca((size_t)datasize);
    memset((void *)x0, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x0, _cffi_type(1), arg0) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg1, (char **)&x1);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x1 = alloca((size_t)datasize);
    memset((void *)x1, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x1, _cffi_type(1), arg1) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg2, (char **)&x2);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x2 = alloca((size_t)datasize);
    memset((void *)x2, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x2, _cffi_type(1), arg2) < 0)
      return NULL;
  }

  datasize = _cffi_prepare_pointer_call_argument(
      _cffi_type(1), arg3, (char **)&x3);
  if (datasize != 0) {
    if (datasize < 0)
      return NULL;
    x3 = alloca((size_t)datasize);
    memset((void *)x3, 0, (size_t)datasize);
    if (_cffi_convert_array_from_object((char *)x3, _cffi_type(1), arg3) < 0)
      return NULL;
  }

  x4 = _cffi_to_c_int(arg4, unsigned int);
  if (x4 == (unsigned int)-1 && PyErr_Occurred())
    return NULL;

  Py_BEGIN_ALLOW_THREADS
  _cffi_restore_errno();
  { result = poly1305aes_53_verify(x0, x1, x2, x3, x4); }
  _cffi_save_errno();
  Py_END_ALLOW_THREADS

  (void)self; /* unused */
  return _cffi_from_c_int(result, int);
}

static int _cffi_const_aes_big_constants(PyObject *lib)
{
  PyObject *o;
  int res;
  unsigned int * i;
  i = (aes_big_constants);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(3));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "aes_big_constants", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return ((void)lib,0);
}

static int _cffi_const_poly1305_53_constants(PyObject *lib)
{
  PyObject *o;
  int res;
  double * i;
  i = (poly1305_53_constants);
  o = _cffi_from_c_pointer((char *)i, _cffi_type(4));
  if (o == NULL)
    return -1;
  res = PyObject_SetAttrString(lib, "poly1305_53_constants", o);
  Py_DECREF(o);
  if (res < 0)
    return -1;
  return _cffi_const_aes_big_constants(lib);
}

static int _cffi_setup_custom(PyObject *lib)
{
  return _cffi_const_poly1305_53_constants(lib);
}

static PyMethodDef _cffi_methods[] = {
  {"aes_big", _cffi_f_aes_big, METH_VARARGS, NULL},
  {"poly1305_53", _cffi_f_poly1305_53, METH_VARARGS, NULL},
  {"poly1305aes_53_authenticate", _cffi_f_poly1305aes_53_authenticate, METH_VARARGS, NULL},
  {"poly1305aes_53_clamp", _cffi_f_poly1305aes_53_clamp, METH_O, NULL},
  {"poly1305aes_53_isequal", _cffi_f_poly1305aes_53_isequal, METH_VARARGS, NULL},
  {"poly1305aes_53_verify", _cffi_f_poly1305aes_53_verify, METH_VARARGS, NULL},
  {"_cffi_setup", _cffi_setup, METH_VARARGS, NULL},
  {NULL, NULL, 0, NULL}    /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3

static struct PyModuleDef _cffi_module_def = {
  PyModuleDef_HEAD_INIT,
  "_poly1305_aes_cffi_8e2af04dxcd5de5fb",
  NULL,
  -1,
  _cffi_methods,
  NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit__poly1305_aes_cffi_8e2af04dxcd5de5fb(void)
{
  PyObject *lib;
  lib = PyModule_Create(&_cffi_module_def);
  if (lib == NULL)
    return NULL;
  if (((void)lib,0) < 0 || _cffi_init() < 0) {
    Py_DECREF(lib);
    return NULL;
  }
  return lib;
}

#else

PyMODINIT_FUNC
init_poly1305_aes_cffi_8e2af04dxcd5de5fb(void)
{
  PyObject *lib;
  lib = Py_InitModule("_poly1305_aes_cffi_8e2af04dxcd5de5fb", _cffi_methods);
  if (lib == NULL)
    return;
  if (((void)lib,0) < 0 || _cffi_init() < 0)
    return;
  return;
}

#endif
