#ifndef BIGINTH
#define BIGINTH

/*
Copyright (c) 2023 John Regan

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/

#if !defined(BIGINT_API)
    #ifdef _WIN32
        #define BIGINT_DLL_IMPORT  __declspec(dllimport)
        #define BIGINT_DLL_EXPORT  __declspec(dllexport)
    #else
        #if defined(__GNUC__) && __GNUC__ >= 4
            #define BIGINT_DLL_IMPORT  __attribute__((visibility("default")))
            #define BIGINT_DLL_EXPORT  __attribute__((visibility("default")))
        #else
            #define BIGINT_DLL_IMPORT
            #define BIGINT_DLL_EXPORT
        #endif
    #endif

    #ifdef BIGINT_IMPLEMENTATION
        #define BIGINT_API  BIGINT_DLL_EXPORT
    #else
        #define BIGINT_API  BIGINT_DLL_IMPORT
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define BIGINT_ENOMEM 1
#define BIGINT_ELIMIT 2
#define BIGINT_EINVAL 4

/* if you override any of:
 *   BIGINT_WORD_WIDTH
 *   BIGINT_WORD_TYPE
 *   BIGINT_DEFAULT_LIMIT
 *   BIGINT_NO_MALLOC
 *
 * They needs to be defined before *every* #include
 * (not just the implementation include) since that
 * affects function signatures, the BIGINT_INIT
 * initializer, and (for NO_MALLOC) the struct itself. */

#ifndef BIGINT_DEFAULT_LIMIT
#define BIGINT_DEFAULT_LIMIT 4096
#endif

#ifndef BIGINT_WORD_WIDTH
#define BIGINT_WORD_WIDTH 4
#endif

#ifndef BIGINT_WORD_TYPE
#if BIGINT_WORD_WIDTH == 1
#define BIGINT_WORD_TYPE uint8_t
#elif BIGINT_WORD_WIDTH == 2
#define BIGINT_WORD_TYPE uint16_t
#elif BIGINT_WORD_WIDTH == 4
#define BIGINT_WORD_TYPE uint32_t
#elif BIGINT_WORD_WIDTH == 8
#define BIGINT_WORD_TYPE uint64_t
#else
#error unknown_type
#endif
#endif

typedef BIGINT_WORD_TYPE bigint_word;

typedef struct bigint {
    size_t size;
    size_t alloc;
    size_t limit;
    size_t sign;
#ifdef BIGINT_NO_MALLOC
    bigint_word words[(BIGINT_DEFAULT_LIMIT/BIGINT_WORD_WIDTH)];
    /* TODO:
     * compiling with BIGINT_NO_MALLOC defined and the word type set to uint8_t
     * generates the following warning:
     * bigint.h:409:24: warning: writing 1 byte into a region of size 0 [-Wstringop-overflow=]
     *   409 |     b->words[old_size] = val;
     *       |     ~~~~~~~~~~~~~~~~~~~^~~~~
     * bigint.h: In function ‘bigint_from_string_base10’:
     * bigint.h:82:17: note: at offset -1 into destination object ‘words’ of size 4096
     *    82 |     bigint_word words[(BIGINT_DEFAULT_LIMIT/BIGINT_WORD_WIDTH)];
     * .... why? */
#else
    bigint_word *words;
#endif
} bigint;

#define BIGINT_WORD_SIZE sizeof(bigint_word)
#define BIGINT_WORD_BIT (BIGINT_WORD_SIZE * CHAR_BIT)
#define BIGINT_WORD_MASK ((bigint_word)-1)

#define BIGINT_HALF_WORD_BIT  (BIGINT_WORD_BIT/2)
#define BIGINT_HALF_WORD_MASK (BIGINT_WORD_MASK >> BIGINT_HALF_WORD_BIT)

#ifdef BIGINT_NO_MALLOC
#define BIGINT_INIT { .size = 0, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT }
#else
#define BIGINT_INIT { .size = 0, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT, .words = NULL }
#endif
extern const bigint* BIGINT_ZERO;

BIGINT_API
void bigint_init(bigint* b);

BIGINT_API
void bigint_free(bigint* b);

BIGINT_API
int bigint_copy(bigint* b, const bigint* a);

BIGINT_API
int bigint_from_word(bigint* b, bigint_word val);

BIGINT_API
int bigint_from_string(bigint* b, const char* str, size_t len, unsigned int base);

/* convenience function for NULL-terminated strings */
BIGINT_API
int bigint_from_cstring(bigint* b, const char* str, unsigned int base);

BIGINT_API
size_t bigint_to_string(char* str, size_t len, const bigint* b, unsigned int base);

BIGINT_API
int bigint_inc(bigint* c, const bigint *a);

BIGINT_API
int bigint_dec(bigint* c, const bigint *b);

BIGINT_API
int bigint_add(bigint* c, const bigint* a, const bigint* b);

BIGINT_API
int bigint_sub(bigint* c, const bigint* a, const bigint* b);

BIGINT_API
int bigint_mul(bigint* c, const bigint* a, const bigint* b);

BIGINT_API
int bigint_div_mod(bigint* quotient, bigint* remainder, const bigint* numerator, const bigint* denominator);

/* faster div_mod for when you're dividing by a regular, positive word type - note that this
 * overwrites numerator! roughly equivalent to:
 * remainder = numerator % denominator
 * numerator /= denominator */
BIGINT_API
void bigint_div_mod_word(bigint* numerator, bigint_word* remainder, bigint_word denominator);

BIGINT_API
int bigint_lshift(bigint* c, const bigint* a, size_t bits);

BIGINT_API
int bigint_rshift(bigint* c, const bigint* a, size_t bits);

BIGINT_API
int bigint_lshift_overwrite(bigint* a, size_t bits);

BIGINT_API
int bigint_rshift_overwrite(bigint* a, size_t bits);

#ifdef __cplusplus
}
#endif

#endif

#ifdef BIGINT_IMPLEMENTATION

#define BIGINT_BLOCK_SIZE 8

#if __GNUC__ > 4 || \
   (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#define BIGINT_UNREACHABLE __builtin_unreachable()
#define BIGINT_UNREACHABLE_RETURN(val) __builtin_unreachable()
#endif

#if __GNUC__ > 3 || \
   (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)
#define BIGINT_NONNULL1 __attribute__((__nonnull__(1)))
#endif

#if _MSC_VER >= 1310
#define BIGINT_UNREACHABLE __assume(0)
#define BIGINT_UNREACHABLE_RETURN(val) __builtin_unreachable()
#endif

#ifndef BIGINT_UNREACHABLE
#define BIGINT_UNREACHABLE assert(0)
#endif

#ifndef BIGINT_NONNULL1
#define BIGINT_NONNULL1
#endif

#ifndef BIGINT_UNREACHABLE_RETURN
#define BIGINT_UNREACHABLE_RETURN(val) return val
#endif

#ifndef BIGINT_SINGLE_WORD_ONLY
#ifndef BIGINT_DWORD_TYPE
#if BIGINT_WORD_WIDTH == 8
#if defined(__SIZEOF_INT128__)
#define BIGINT_DWORD_TYPE unsigned __int128
#endif
#elif BIGINT_WORD_WIDTH == 4
#define BIGINT_DWORD_TYPE uint64_t
#elif BIGINT_WORD_WIDTH == 2
#define BIGINT_DWORD_TYPE uint32_t
#elif BIGINT_WORD_WIDTH == 1
#define BIGINT_DWORD_TYPE uint16_t
#endif
#endif /* BIGINT_DWORD_TYPE */
#endif /* !SINGLE_WORD_ONLY */

#ifdef _WIN32
#include <intrin.h>
#if _MSC_VER
#if BIGINT_WORD_WIDTH == 8 && defined(_WIN64)
#pragma intrinsic(_BitScanReverse64)
#pragma intrinsic(_umul128)
#elif BIGINT_WORD_WIDTH <= 4
#pragma intrinsic(_BitScanReverse)
#endif
#endif
#endif


#ifdef BIGINT_NO_MALLOC
static const bigint BIGINT_ZERO_STORAGE = { .size = 0, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT };
#else
static const bigint BIGINT_ZERO_STORAGE = { .words = NULL, .size = 0, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT };
#endif
const bigint* BIGINT_ZERO = &BIGINT_ZERO_STORAGE;

static void bigint_reset(bigint* b);
static int  bigint_resize(bigint* b, size_t size);
static inline void bigint_truncate(bigint* b);
static int  bigint_append(bigint* b, bigint_word val);

static int bigint_add_word(bigint* b, bigint_word val);

static inline size_t bigint_word_bitlength(bigint_word a);
static int bigint_word_add(bigint_word *a, bigint_word b);
static int bigint_word_sub(bigint_word *a, bigint_word b);
static bigint_word bigint_word_mul(bigint_word *a, bigint_word b);

static int bigint_add_unsigned(bigint* a, const bigint* b);
static int bigint_sub_unsigned(bigint* a, const bigint* b);

static int bigint_add_signed(bigint* c, const bigint* a, size_t a_sign, const bigint* b, size_t b_sign);

static int bigint_cmp_abs(const bigint* a, const bigint* b);

static int bigint_mul_long(bigint* c, const bigint* a, const bigint* b);

static size_t bigint_bitlength(const bigint* a);
static int bigint_get_bit(const bigint* a, size_t b);
static int bigint_set_bit(bigint* a, size_t b, uint8_t val);
static int bigint_from_string_base16(bigint* b, const char* str, size_t len);
static int bigint_from_string_base10(bigint* b, const char* str, size_t len);
static int bigint_from_string_base8(bigint* b, const char* str, size_t len);
static int bigint_from_string_base2(bigint* b, const char* str, size_t len);
static int bigint_from_string_base0(bigint* b, const char* str, size_t len);

/* returns the length of a string for the given base */
static size_t bigint_len_string_base16(const bigint* b);
static size_t bigint_len_string_base10(const bigint* b);
static size_t bigint_len_string_base8(const bigint* b);
static size_t bigint_len_string_base2(const bigint* b);

BIGINT_NONNULL1
static size_t bigint_to_string_base16(char* str, size_t len, bigint* b);
BIGINT_NONNULL1
static size_t bigint_to_string_base10(char* str, size_t len, bigint* b);
BIGINT_NONNULL1
static size_t bigint_to_string_base8(char* str, size_t len, bigint* b);
BIGINT_NONNULL1
static size_t bigint_to_string_base2(char* str, size_t len, bigint* b);

static
void bigint_reset(bigint* b) {
    b->size = 0;
    b->sign = 0;
#ifndef BIGINT_NO_MALLOC
    if(b->alloc)
#endif
        b->words[0] = 0;
}

static
int bigint_resize(bigint* b, size_t size) {
#ifdef BIGINT_NO_MALLOC
    size_t alloc = size;
    if(alloc * BIGINT_WORD_SIZE > BIGINT_DEFAULT_LIMIT) return BIGINT_ENOMEM;
#else
    size_t alloc = ((size + BIGINT_BLOCK_SIZE-1)) & -BIGINT_BLOCK_SIZE;
#endif

    if(alloc * BIGINT_WORD_SIZE > b->limit) return BIGINT_ELIMIT;

#ifndef BIGINT_NO_MALLOC
    if(b->alloc < alloc) {
        if(b->words != NULL) {
            b->words = (bigint_word *)realloc(b->words,alloc * BIGINT_WORD_SIZE);
        } else {
            b->words = (bigint_word *)malloc(alloc * BIGINT_WORD_SIZE);
        }
        if(b->words == NULL) return BIGINT_ENOMEM;
        memset(&b->words[b->alloc],0,(alloc - b->alloc) * BIGINT_WORD_SIZE);
        b->alloc = alloc;
    }
#endif

    if(b->size > size) { /* shrinking */
        b->words[size] = 0;
    } else if(b->size < size) { /* growing */
        b->words[b->size] = 0;
    }

    b->size = size;
    return 0;
}

static inline
void bigint_truncate(bigint* b) {
    while(b->size && b->words[b->size - 1] == 0) b->size--;
    b->sign = b->size && b->sign;
}

static inline
size_t bigint_word_bitlength(bigint_word a) {
#if defined(_WIN32) && BIGINT_WORD_WIDTH <= 4
   unsigned long index;
   unsigned long m = (unsigned long)a;
   _BitScanReverse(&index,m);
   return (size_t)++index;
#elif defined(_WIN64) && BIGINT_WORD_WIDTH == 8
   unsigned long index;
   _BitScanReverse64(&index,a);
   return (size_t)++index;
#elif __GNUC__ > 4 || \
   (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
   assert(a);
#if BIGINT_WORD_WIDTH == 8
   return (size_t) ((sizeof(unsigned long long) * CHAR_BIT) - __builtin_clzll((unsigned long long)a));
#else
   return (size_t) ((sizeof(unsigned long) * CHAR_BIT) - __builtin_clzl((unsigned long)a));
#endif /* BIGINT_WORD_WIDTH == 8 */

#else /* all other platforms */
    size_t i = BIGINT_WORD_BIT;

    assert(a);
    while(i-- > 0) {
        if(a >> i & 1) return i+1;
    }

    BIGINT_UNREACHABLE_RETURN(0);
#endif
}

static inline
int bigint_get_bit(const bigint* a, size_t b) {
    size_t word = b / BIGINT_WORD_BIT;
    size_t bit = b % BIGINT_WORD_BIT;
    if(word > a->size) return 0;
    return (a->words[word] >> bit) & 1;
}

static inline
int bigint_set_bit(bigint* a, size_t b, uint8_t val) {
    size_t word = b / BIGINT_WORD_BIT;
    size_t bit = b % BIGINT_WORD_BIT;
    int r;

    if(a->size <= word) {
        if( (r = bigint_resize(a,word+1)) != 0) return r;
    }

#if 0 /* this is the obvious way, for reference */
    if(val) {
        a->words[word] |= ((bigint_word)1) << bit;
    } else {
        a->words[word] &= ~(((bigint_word)1) << bit);
    }
#else
    /* adopted from https://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching */
    a->words[word] ^= (-val ^ a->words[word]) & (((bigint_word)1) << bit);
#endif
    return 0;
}

static
size_t bigint_bitlength(const bigint* a) {
    size_t l;
    if(a->size == 0) return 0;
    l = a->size - 1;
    return bigint_word_bitlength(a->words[l]) + (l * BIGINT_WORD_BIT);
}

static
int bigint_append(bigint* b, bigint_word val) {
    int r;
    size_t old_size = b->size;
    r = bigint_resize(b,b->size+1);
    if(r) return r;
    b->words[old_size] = val;
    return 0;
}

static int bigint_word_add(bigint_word *a, bigint_word b) {
    return (*a += b) < b;
}

static int bigint_word_sub(bigint_word *a, bigint_word b) {
    bigint_word tmp = *a;
    return (*a -= b) > tmp;
}

/* performs *a *= b and returns the carry, using
 * only the single-word data type */
static inline
bigint_word bigint_word_mul_cross(bigint_word *a, bigint_word b) {
    /* assuming a 32-bit word size -
     * using cross-products we can say:
     * (64-bit result) = 
     *   a_lo * bi_lo
     *   + (a_hi * b_lo << 16)
     *   + (a_lo * b_hi << 16)
     *   + (a_hi * b_hi << 32)
     *
     * when doing additions, we need to mind overflow
     * and carry into res_hi.
     * It's way easier and faster to just use a double-width
     * type, this variant was mostly a learning exercise in how
     * to multiply numbers with carry/overflow */

    bigint_word a_hi, b_hi, a_lo, b_lo, tmp, res_lo, res_hi;
    a_hi = (*a) >> BIGINT_HALF_WORD_BIT;
    a_lo = (*a) & BIGINT_HALF_WORD_MASK;
    b_hi = b >> BIGINT_HALF_WORD_BIT;
    b_lo = b & BIGINT_HALF_WORD_MASK;

    res_lo = a_lo * b_lo;
    res_hi = a_hi * b_hi;

    tmp = a_hi * b_lo;
    res_hi += bigint_word_add(&res_lo,tmp << BIGINT_HALF_WORD_BIT);
    res_hi += (tmp >> BIGINT_HALF_WORD_BIT);

    tmp = a_lo * b_hi;
    res_hi += bigint_word_add(&res_lo,tmp << BIGINT_HALF_WORD_BIT);
    res_hi += (tmp >> BIGINT_HALF_WORD_BIT);

    *a = res_lo;
    return res_hi;
}

#ifndef BIGINT_SINGLE_WORD_ONLY
/* attempt to find method for multiplication with a larger type */

#ifdef BIGINT_DWORD_TYPE
static inline
bigint_word bigint_word_mul_double(bigint_word *a, bigint_word b) {
    BIGINT_DWORD_TYPE res;
    res = *a;
    res *= b;
    *a = res & BIGINT_WORD_MASK;
    res >>= BIGINT_WORD_BIT;
    return res;
}
#define BIGINT_WORD_MUL_DOUBLE
#elif BIGINT_WORD_WIDTH == 8 && defined(_WIN64)
static inline
bigint_word bigint_word_mul_double(bigint_word *a, bigint_word b) {
    uint64_t lowProduct;
    uint64_t highProduct;

    lowProduct = _umul128(*a,b,&highProduct);

    *a = lowProduct;
    return highProduct;
}
#define BIGINT_WORD_MUL_DOUBLE
#endif
#endif

/* performs *a *= b and returns the carry */
static bigint_word bigint_word_mul(bigint_word *a, bigint_word b) {
#if defined(BIGINT_SINGLE_WORD_ONLY) || !defined(BIGINT_WORD_MUL_DOUBLE)
    return bigint_word_mul_cross(a,b);
#else /* !SINGLE_WORD_ONLY */
    return bigint_word_mul_double(a,b);
#endif
}

#if defined(BIGINT_SINGLE_WORD_ONLY) || !defined(BIGINT_WORD_MUL_DOUBLE)
#define BIGINT_WORD_MUL_CONST(x) \
static inline bigint_word bigint_word_mul_const_ ## x (bigint_word *a) { \
    bigint_word a_hi, b_hi, a_lo, b_lo, tmp, res_lo, res_hi; \
    a_hi = (*a) >> BIGINT_HALF_WORD_BIT; \
    a_lo = (*a) & BIGINT_HALF_WORD_MASK; \
    b_hi = ((bigint_word)x) >> BIGINT_HALF_WORD_BIT; \
    b_lo = ((bigint_word)x) & BIGINT_HALF_WORD_MASK; \
    res_lo = a_lo * b_lo; \
    res_hi = a_hi * b_hi; \
    tmp = a_hi * b_lo; \
    res_hi += bigint_word_add(&res_lo,tmp << BIGINT_HALF_WORD_BIT); \
    res_hi += (tmp >> BIGINT_HALF_WORD_BIT); \
    tmp = a_lo * b_hi; \
    res_hi += bigint_word_add(&res_lo,tmp << BIGINT_HALF_WORD_BIT); \
    res_hi += (tmp >> BIGINT_HALF_WORD_BIT); \
    *a = res_lo; \
    return res_hi; \
}
#elif defined(BIGINT_DWORD_TYPE)
#define BIGINT_WORD_MUL_CONST(x) \
static inline bigint_word bigint_word_mul_const_ ## x (bigint_word *a) { \
    BIGINT_DWORD_TYPE res = *a; \
    res *= x; \
    *a = res & BIGINT_WORD_MASK; \
    res >>= BIGINT_WORD_BIT; \
    return res; \
}
#else
#define BIGINT_WORD_MUL_CONST(x) \
static inline bigint_word bigint_word_mul_const_ ## x (bigint_word *a) { \
    return bigint_word_mul_double(a,x);
}
#endif

BIGINT_WORD_MUL_CONST(10)

static int bigint_add_word(bigint* a, bigint_word val) {
    int r;
    size_t i = 0;
    bigint_word carry = 0;

    if(!val) return 0; /* adding 0 */
    if(!a->size) {
        if( (r = bigint_resize(a,1)) != 0) return r;
    }

    carry = bigint_word_add(&a->words[0],val);

    i = 1;
    while(i < a->size && carry) {
        carry = bigint_word_add(&a->words[i],carry);
        i++;
    }
    if(carry) {
        if( (r = bigint_append(a, carry)) != 0) return r;
    }

    bigint_truncate(a);
    return 0;
}

static int bigint_add_unsigned(bigint* a, const bigint* b) {
    int r;
    size_t i = 0;
    size_t a_size, b_size, max_size;
    bigint_word carry = 0;

    if(b->size == 0) return 0;

    a_size = a->size;
    b_size = b->size;
    max_size = a_size > b_size ? a_size : b_size;

    if( (r = bigint_resize(a,max_size)) != 0) return r;

    i = 0;
    while(i < b->size) {
        carry = bigint_word_add(&a->words[i],carry);
        carry += bigint_word_add(&a->words[i],b->words[i]);
        i++;
    }
    while(i < max_size && carry) {
        carry = bigint_word_add(&a->words[i],carry);
        i++;
    }
    if(carry) {
        if( (r = bigint_append(a, carry)) != 0) return r;
    }

    bigint_truncate(a);
    return 0;
}

static int bigint_sub_unsigned(bigint* a, const bigint* b) {
    size_t i;
    bigint_word carry = 0;

    if(b->size == 0) return 0;

    i = 0;
    while(i < b->size) {
        carry = bigint_word_sub(&a->words[i],carry);
        carry += bigint_word_sub(&a->words[i],b->words[i]);
        i++;
    }
    while(i < a->size && carry) {
        carry = bigint_word_sub(&a->words[i],carry);
        i++;
    }
    bigint_truncate(a);
    return 0;
}

static
int bigint_cmp_abs(const bigint* a, const bigint* b) {
    size_t i;
    if(a->size != b->size) return a->size < b->size ? -1 : 1;
    i = a->size;
    while(i--) {
        if(a->words[i] != b->words[i])
            return a->words[i] < b->words[i] ? -1 : 1;
    }
    return 0;
}

BIGINT_API
int bigint_lshift_overwrite(bigint* c, size_t bits) {
    int r;
    size_t words, expanded, i;
    bigint_word lo, hi;

    if(c->size == 0) return 0;

    words = bits / BIGINT_WORD_BIT;
    bits %= BIGINT_WORD_BIT;

    expanded = c->size + words + (bits != 0);

    if( (r = bigint_resize(c,expanded)) != 0) return r;

    if(bits == 0) {
        i = expanded;
        while(i-- > words) {
            c->words[i] = c->words[i - words];
        }
    } else {
        lo = hi = 0;
        i = expanded;
        while(--i > words) {
            lo = c->words[i - words - 1];
            c->words[i] = (hi << bits) | (lo >> (BIGINT_WORD_BIT - bits));
            hi = lo;
        }
        c->words[words] = hi << bits;
    }
    i = 0;
    while(i<words) {
        c->words[i] = 0;
        i++;
    }
    bigint_truncate(c);
    return 0;
}

BIGINT_API
int bigint_lshift(bigint* c, const bigint* a, size_t bits) {
    int r;

    if( (r = bigint_copy(c,a)) != 0) return r;
    return bigint_lshift_overwrite(c,bits);
}


BIGINT_API
int bigint_rshift_overwrite(bigint* c, size_t bits) {
    int r;
    size_t words, i;
    bigint_word lo, hi;

    words = bits / BIGINT_WORD_BIT;
    if(words >= c->size) { /* right-shiting by more words than we actually have */
        c->size = 0;
        c->sign = 0;
        return 0;
    }

    bits %= BIGINT_WORD_BIT;
    i = 0;

    if(bits == 0) {
        while(i < c->size - words) {
            c->words[i] = c->words[i + words];
            i++;
        }
    } else {
        lo = hi = c->words[words];
        while(i < c->size - words - 1) {
            hi = c->words[i + words + 1];
            c->words[i] = (hi << (BIGINT_WORD_BIT - bits)) | lo >> bits;
            lo = hi;
            i++;
        }
        c->words[c->size - words - 1] = lo >> bits;
    }
    if( (r = bigint_resize(c, c->size - words)) != 0) return r;
    bigint_truncate(c);
    return 0;
}

#define BIGINT_RSHIFT_OVERWRITE_SMALL(x) \
static inline void bigint_rshift_overwrite_small_ ## x (bigint* c) { \
    size_t i; \
    bigint_word lo, hi; \
    i = 0; \
    lo = hi = c->words[0]; \
    while(i < c->size - 1) { \
        hi = c->words[i + 1]; \
        c->words[i] = (hi << (BIGINT_WORD_BIT - x)) | lo >> x; \
        lo = hi; \
        i++; \
    } \
    c->words[i] = lo >> x; \
    bigint_truncate(c); \
}

BIGINT_RSHIFT_OVERWRITE_SMALL(4)
BIGINT_RSHIFT_OVERWRITE_SMALL(3)
BIGINT_RSHIFT_OVERWRITE_SMALL(1)

BIGINT_API
int bigint_rshift(bigint* c, const bigint* a, size_t bits) {
    int r;
    if( (r = bigint_copy(c,a)) != 0) return r;
    return bigint_rshift_overwrite(c,bits);
}

static
int bigint_mul_long(bigint* c, const bigint* a, const bigint* b) {
    int r;
    size_t expanded;
    size_t ia, ib;
    bigint_word carry;
#if defined(BIGINT_SINGLE_WORD_ONLY) || !defined(BIGINT_DWORD_TYPE)
    bigint_word lo, hi;
#else
    BIGINT_DWORD_TYPE res;
#endif

    expanded = a->size + b->size + 1;

    if( (r = bigint_resize(c, expanded)) != 0) return r;
    c->sign = a->sign ^ b->sign;

    for(ib = 0; ib < b->size; ib++) {
        carry = 0;
        for(ia = 0; ia < a->size; ia++) {
#if defined(BIGINT_SINGLE_WORD_ONLY) || !defined(BIGINT_DWORD_TYPE)
            lo = a->words[ia];
            hi = bigint_word_mul(&lo,b->words[ib]);
            hi += bigint_word_add(&lo,c->words[ia+ib]);
            hi += bigint_word_add(&lo,carry);
            c->words[ia + ib] = lo;
            carry = hi;
#else
            res = a->words[ia];
            res *= b->words[ib];
            res += c->words[ia + ib];
            res += carry;
            c->words[ia + ib] = res & BIGINT_WORD_MASK;
            carry = res >> BIGINT_WORD_BIT;
#endif
        }
        c->words[ib + a->size] = carry;
    }

    bigint_truncate(c);
    return 0;
}

static int bigint_mul_word(bigint* a, bigint_word val) {
    int r;
    size_t i = 0;
    bigint_word carry = 0;
    bigint_word c = 0;

    if(!val) {
        a->size = 0;
        a->sign = 0;
        return 0;
    }

    carry = 0;
    for(i=0;i<a->size;i++) {
        c = bigint_word_mul(&a->words[i],val);
        c += bigint_word_add(&a->words[i],carry);
        carry = c;
    }
    if(carry) {
        if( (r = bigint_append(a, carry)) != 0) return r;
    }

    bigint_truncate(a);
    return 0;
}

#define BIGINT_MUL_CONST(x) \
static inline int bigint_mul_const_ ## x (bigint* a) { \
    int r; \
    size_t i = 0; \
    bigint_word carry = 0; \
    bigint_word c = 0; \
    for(i=0;i<a->size;i++) { \
        c = bigint_word_mul_const_ ## x (&a->words[i]); \
        c += bigint_word_add(&a->words[i],carry); \
        carry = c; \
    } \
    if(carry) { \
        if( (r = bigint_append(a, carry)) != 0) return r; \
    } \
    bigint_truncate(a); \
    return 0; \
}

BIGINT_MUL_CONST(10)

/* does NOT accept a string with a leading 0x - that's handled in
 * bigint_from_string. does NOT parse a leading +/-, also handled
 * in bigint_from_string */
static int bigint_from_string_base16(bigint* b, const char* str, size_t len) {
    bigint_word word;
    size_t i = 0;
    int r;

    while(i < len && str[i]) {
        if( (r = bigint_lshift_overwrite(b,4)) != 0) return r;
        switch(str[i]) {
            case '0': word = 0; break;
            case '1': word = 1; break;
            case '2': word = 2; break;
            case '3': word = 3; break;
            case '4': word = 4; break;
            case '5': word = 5; break;
            case '6': word = 6; break;
            case '7': word = 7; break;
            case '8': word = 8; break;
            case '9': word = 9; break;
            case 'A': word = 10; break;
            case 'a': word = 10; break;
            case 'B': word = 11; break;
            case 'b': word = 11; break;
            case 'C': word = 12; break;
            case 'c': word = 12; break;
            case 'D': word = 13; break;
            case 'd': word = 13; break;
            case 'E': word = 14; break;
            case 'e': word = 14; break;
            case 'F': word = 15; break;
            case 'f': word = 15; break;
            default: return BIGINT_EINVAL;
        }
        if( (r = bigint_add_word(b,word)) != 0) return r;
        i++;
    }

    return 0;
}

/* does NOT parse a leading +/-, handled in bigint_from_string */
static int bigint_from_string_base10(bigint* b, const char* str, size_t len) {
    bigint_word word;
    size_t i = 0;
    int r;

    while(i < len && str[i]) {
        if( (r = bigint_mul_const_10(b)) != 0) return r;
        switch(str[i]) {
            case '0': word = 0; break;
            case '1': word = 1; break;
            case '2': word = 2; break;
            case '3': word = 3; break;
            case '4': word = 4; break;
            case '5': word = 5; break;
            case '6': word = 6; break;
            case '7': word = 7; break;
            case '8': word = 8; break;
            case '9': word = 9; break;
            default: return BIGINT_EINVAL;
        }
        if( (r = bigint_add_word(b,word)) != 0) return r;
        i++;
    }

    return 0;
}

/* does NOT parse a leading +/-, handled in bigint_from_string */
static int bigint_from_string_base8(bigint* b, const char* str, size_t len) {
    bigint_word word;
    size_t i = 0;
    int r;

    while(i < len && str[i]) {
        if( (r = bigint_lshift_overwrite(b,3)) != 0) return r;
        switch(str[i]) {
            case '0': word = 0; break;
            case '1': word = 1; break;
            case '2': word = 2; break;
            case '3': word = 3; break;
            case '4': word = 4; break;
            case '5': word = 5; break;
            case '6': word = 6; break;
            case '7': word = 7; break;
            default: return BIGINT_EINVAL;
        }
        if( (r = bigint_add_word(b,word)) != 0) return r;
        i++;
    }

    return 0;
}

/* does NOT parse a leading +/-, handled in bigint_from_string */
static int bigint_from_string_base2(bigint* b, const char* str, size_t len) {
    bigint_word word;
    size_t i = 0;
    int r;

    while(i < len && str[i]) {
        if( (r = bigint_lshift_overwrite(b,1)) != 0) return r;
        switch(str[i]) {
            case '0': word = 0; break;
            case '1': word = 1; break;
            default: return BIGINT_EINVAL;
        }
        if( (r = bigint_add_word(b,word)) != 0) return r;
        i++;
    }

    return 0;
}

static int bigint_from_string_base0(bigint* b, const char* str, size_t len) {
    if(str[0] == '0') {
        str++;
        len--;
        if(len == 0) {
            bigint_reset(b);
            return 0;
        }

        if(str[0] == 'x' || str[0] == 'X') {
            str++;
            len--;
            if(len == 0) return BIGINT_EINVAL;
            return bigint_from_string_base16(b,str,len);
        }

        if(str[0] == 'b' || str[0] == 'B') {
            str++;
            len--;
            if(len == 0) return BIGINT_EINVAL;
            return bigint_from_string_base2(b,str,len);
        }

        return bigint_from_string_base8(b,str,len);
    }

    if(len > 1 && str[0] == 'x') {
        str++;
        len--;
        return bigint_from_string_base16(b,str,len);
    }

    if(len > 1 && str[0] == 'b') {
        str++;
        len--;
        return bigint_from_string_base2(b,str,len);
    }

    return bigint_from_string_base10(b,str,len);
}

/* allows strings like:
 *  -b010101
 *  -0b010101
 *  01231 (octal)
 *  1234 (base10)
 *  x1234 (base16)
 *  0x1234 (base16)
 */

BIGINT_API
int bigint_from_string(bigint* b, const char* str, size_t len, unsigned int base) {
    int r = 0;
    size_t sign = 0;

    bigint_reset(b);

    if(len == 0) return BIGINT_EINVAL;

    if(str[0] == '-') {
        sign = 1;
        str++;
        len--;
        if(len == 0) return BIGINT_EINVAL;
    }

    switch(base) {
        case 2: {
            if(len > 1 && (str[0] == 'b')) {
                str++;
                len--;
                if(len == 0) return BIGINT_EINVAL;
            }
            else if(len > 2 && str[0] == '0' && (str[1] == 'b' || str[1] == 'B')) {
                str += 2;
                len -= 2;
            }
            r = bigint_from_string_base2(b,str,len); break;
        }
        case 8: {
            r = bigint_from_string_base8(b,str,len); break;
        }
        case 10: r = bigint_from_string_base10(b,str,len); break;
        case 16: {
            if(len > 1 && str[0] == 'x') {
                str++;
                len--;
                if(len == 0) return BIGINT_EINVAL;
            }
            else if(len > 2 && str[0] == '0' && (str[1] == 'X' || str[1] == 'x')) {
                str += 2;
                len -= 2;
            }
            r = bigint_from_string_base16(b,str,len); break;
        }
        case 0: r = bigint_from_string_base0(b,str,len); break;
        default: return BIGINT_EINVAL;
    }
    if(r == 0) {
        b->sign = sign;
    }
    return r;
}

BIGINT_API
int bigint_from_cstring(bigint* b, const char* str, unsigned int base) {
    return bigint_from_string(b,str,strlen(str),base);
}

BIGINT_API
void bigint_init(bigint* b) {
    *b = *BIGINT_ZERO;
    b->limit = BIGINT_DEFAULT_LIMIT;
}

BIGINT_API
void bigint_free(bigint* b) {
    bigint_reset(b);
#ifndef BIGINT_NO_MALLOC
    if(b->alloc != 0) {
        free(b->words);
        b->words = NULL;
        b->alloc = 0;
    }
#endif
}

BIGINT_API
int bigint_copy(bigint *dest, const bigint *src) {
    int r;
    size_t limit;
    if(src == dest) return 0;
    limit = dest->limit;
    dest->limit = src->limit;
    if(src->size != dest->size) {
        r = bigint_resize(dest,src->size);
        if(r) {
            dest->limit = limit;
            return r;
        }
    }

    /* we may be copying a zero-length */
    if(dest->size == 0) {
        dest->sign = 0;
        return 0;
    }

    assert(dest->words != NULL);
    memcpy(dest->words,src->words,dest->size * BIGINT_WORD_SIZE);
    dest->sign = src->sign;
    return 0;
}

BIGINT_API
int bigint_from_word(bigint* b, bigint_word val) {
    bigint_reset(b);
    return bigint_append(b,val);
}

BIGINT_API
int bigint_from_u8(bigint* b, uint8_t val) {
    bigint_reset(b);
    return bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK) );
}

BIGINT_API
int bigint_from_u16(bigint* b, uint16_t val) {
    int r;
    bigint_reset(b);
#if BIGINT_WORD_WIDTH < 2
    while(val) {
        r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
        if(r) return r;
        val >>= BIGINT_WORD_BIT;
    }
#else
    r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
#endif
    return r;
}

BIGINT_API
int bigint_from_u32(bigint* b, uint32_t val) {
    int r;
    bigint_reset(b);
#if BIGINT_WORD_WIDTH < 4
    while(val) {
        r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
        if(r) return r;
        val >>= BIGINT_WORD_BIT;
    }
#else
    r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
#endif
    return r;
}

BIGINT_API
int bigint_from_u64(bigint* b, uint64_t val) {
    int r;
    bigint_reset(b);
#if BIGINT_WORD_WIDTH < 8
    while(val) {
        r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
        if(r) return r;
        val >>= BIGINT_WORD_BIT;
    }
#else
    r = bigint_append(b, (bigint_word)(val & BIGINT_WORD_MASK));
#endif
    return r;
}

BIGINT_API
int bigint_from_i8(bigint* b, int8_t val) {
    int r;
    uint8_t uval = val < 0 ? -val : val;

    r = bigint_from_u8(b, uval);
    if(r) return r;
    b->sign = val < 0;
    return 0;
}

BIGINT_API
int bigint_from_i16(bigint* b, int16_t val) {
    int r;
    uint16_t uval = val < 0 ? -val : val;

    r = bigint_from_u16(b, uval);
    if(r) return r;
    b->sign = val < 0;
    return 0;
}

BIGINT_API
int bigint_from_i32(bigint* b, int32_t val) {
    int r;
    uint32_t uval = val < 0 ? -val : val;

    r = bigint_from_u32(b, uval);
    if(r) return r;
    b->sign = val < 0;
    return 0;
}

BIGINT_API
int bigint_from_i64(bigint* b, int64_t val) {
    int r;
    uint64_t uval = val < 0 ? -val : val;

    r = bigint_from_u64(b, uval);
    if(r) return r;
    b->sign = val < 0;
    return 0;
}

static
int bigint_add_signed(bigint* c, const bigint* a, size_t a_sign, const bigint* b, size_t b_sign) {
    int r;
    int cmp;

    if(a_sign == b_sign) {
        if( (r = bigint_copy(c, a)) != 0) return r;
        if( (r = bigint_add_unsigned(c, b)) != 0) return r;
        c->sign = a_sign;
        return 0;
    }

    cmp = bigint_cmp_abs(a,b);

    if(cmp == 0) {
        /* adding positive and negative numbers with the same
         * value, short-circuit to returning zero */
        c->size = 0;
        c->sign = 0;
        return 0;
    }

    if(cmp > 0) {
        if( (r = bigint_copy(c, a)) != 0) return r;
        if( (r = bigint_sub_unsigned(c, b)) != 0) return r;
        c->sign = a_sign;
        return 0;
    }

    if( (r = bigint_copy(c, b)) != 0) return r;
    if( (r = bigint_sub_unsigned(c, a)) != 0) return r;
    c->sign = b_sign;
    return 0;
}

BIGINT_API
int bigint_add(bigint* c, const bigint* a, const bigint* b) {
    int r;
    bigint tmp = BIGINT_INIT;

    if( (r = bigint_copy(&tmp,c)) != 0) return r;
    if( (r = bigint_add_signed(&tmp, a, a->sign, b, b->sign)) != 0) goto cleanup;
    if( (r = bigint_copy(c,&tmp)) != 0) goto cleanup;

    cleanup:
    bigint_free(&tmp);
    return r;
}

BIGINT_API
int bigint_sub(bigint* c, const bigint* a, const bigint* b) {
    int r;
    bigint tmp = BIGINT_INIT;

    if( (r = bigint_copy(&tmp,c)) != 0) return r;
    if( (r = bigint_add_signed(&tmp, a, a->sign, b, !b->sign)) != 0) goto cleanup;
    if( (r = bigint_copy(c,&tmp)) != 0) goto cleanup;

    cleanup:
    bigint_free(&tmp);
    return r;
}

BIGINT_API
int bigint_mul(bigint* c, const bigint* a, const bigint* b) {
    int r;
    bigint tmp = BIGINT_INIT;

    if( (r = bigint_copy(&tmp,c)) != 0) return r;
    if( (r = bigint_mul_long(&tmp, a, b)) != 0) goto cleanup;
    if( (r = bigint_copy(c,&tmp)) != 0) goto cleanup;

    cleanup:
    bigint_free(&tmp);
    return r;
}

BIGINT_API
void bigint_div_mod_word(bigint* numerator, bigint_word* remainder, bigint_word denominator) {
    size_t i;
    size_t j;

    bigint_word dst_word;
    bigint_word src_word;
    bigint_word parts[2];
    bigint_word r = 0;

    bigint_word div_word;
    bigint_word mod_word;

    i = numerator->size;

    while(i-- > 0) {
        dst_word = 0;
        src_word = numerator->words[i];

        parts[0] = src_word >> BIGINT_HALF_WORD_BIT;
        parts[1] = src_word & BIGINT_HALF_WORD_MASK;

        for(j=0;j<2;j++) {
            r <<= BIGINT_HALF_WORD_BIT;
            r |= parts[j];

            div_word = r / denominator;
            mod_word = r % denominator;
            r = mod_word;

            dst_word <<= BIGINT_HALF_WORD_BIT;
            dst_word |= div_word;
        }

        numerator->words[i] = dst_word;
    }

    bigint_truncate(numerator);
    *remainder = r;
}

/* div_mod_word hard-coded to some value to allow compiler-time optimizations */
#define BIGINT_DIV_MOD_WORD(x) \
static inline \
void bigint_div_mod_ ## x (bigint* numerator, bigint_word* remainder) { \
    size_t i; \
    size_t j; \
    bigint_word dst_word; \
    bigint_word src_word; \
    bigint_word parts[2]; \
    bigint_word r = 0; \
    bigint_word div_word; \
    bigint_word mod_word; \
    i = numerator->size; \
    while(i-- > 0) { \
        dst_word = 0; \
        src_word = numerator->words[i]; \
        parts[0] = src_word >> BIGINT_HALF_WORD_BIT; \
        parts[1] = src_word & BIGINT_HALF_WORD_MASK; \
        for(j=0;j<2;j++) { \
            r <<= BIGINT_HALF_WORD_BIT; \
            r |= parts[j]; \
            div_word = r / x; \
            mod_word = r % x; \
            r = mod_word; \
            dst_word <<= BIGINT_HALF_WORD_BIT; \
            dst_word |= div_word; \
        } \
        numerator->words[i] = dst_word; \
    } \
    bigint_truncate(numerator); \
    *remainder = r; \
}

BIGINT_DIV_MOD_WORD(10)

BIGINT_API
int bigint_div_mod(bigint* quotient, bigint* remainder, const bigint* numerator, const bigint* denominator) {
    int r;
    size_t n;

    bigint quo = BIGINT_INIT;
    bigint rem = BIGINT_INIT;
    bigint den = BIGINT_INIT;

    if( (r = bigint_copy(&rem, numerator)) != 0) goto cleanup;
    if( (r = bigint_copy(&den, denominator)) != 0) goto cleanup;

    if(bigint_cmp_abs(&rem,denominator) >= 0) {
        n = bigint_bitlength(numerator) - bigint_bitlength(denominator);
        if( (r = bigint_lshift(&den, denominator, n)) != 0) goto cleanup;
        n++; /* ensure we have at least 1 iteration */
        while(n--) {
            if(bigint_cmp_abs(&rem,&den) >= 0) {
                if( (r = bigint_sub_unsigned(&rem, &den)) != 0) goto cleanup;
                if( (r = bigint_set_bit(&quo, (size_t)n, 1)) != 0) goto cleanup;
            }
            if( (r = bigint_rshift_overwrite(&den,1)) != 0) goto cleanup;
        }
    }

    quo.sign = numerator->sign ^ denominator->sign;
    rem.sign = numerator->sign;

    if( (r = bigint_copy(quotient, &quo)) != 0) goto cleanup;
    if( (r = bigint_copy(remainder, &rem)) != 0) goto cleanup;

    cleanup:
    bigint_free(&quo);
    bigint_free(&rem);
    bigint_free(&den);
    return r;
}

static const bigint_word BIGINT_ONE_VALUE = 1;
#ifdef BIGINT_NO_MALLOC
static const bigint BIGINT_ONE_STORAGE = { .words[0] = BIGINT_ONE_VALUE, .size = 1, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT };
#else
static const bigint BIGINT_ONE_STORAGE = { .words = (bigint_word*)&BIGINT_ONE_VALUE, .size = 1, .alloc = 0, .sign = 0, .limit = BIGINT_DEFAULT_LIMIT };
#endif
static const bigint* BIGINT_ONE = &BIGINT_ONE_STORAGE;

BIGINT_API
int bigint_inc(bigint* c, const bigint *a) {
    return bigint_add(c, a, BIGINT_ONE);
}

BIGINT_API
int bigint_dec(bigint* c, const bigint *a) {
    return bigint_sub(c, a, BIGINT_ONE);
}

/* we don't take the sign into account on these functions,
 * that's handled in the bigint_to_string function */
static size_t bigint_len_string_base16(const bigint* b) {
    size_t blen = bigint_bitlength(b);

    return (blen >> 2) + (!!(blen & 0x03));
}

/* pre-computed 1.0 / log2(10.0); */
static const double base10_scale = 0x1.34413509f79ffp-2;
static size_t bigint_len_string_base10(const bigint* b) {
    size_t blen = (size_t)(((double)bigint_bitlength(b)) * base10_scale);

    return ++blen;
}

static size_t bigint_len_string_base8(const bigint* b) {
    size_t blen = bigint_bitlength(b);

    return (blen / 3) + (!!(blen % 3));
}

static size_t bigint_len_string_base2(const bigint* b) {
    return bigint_bitlength(b);
}

static const char* const bigint_alphabet = "0123456789abcdef";

BIGINT_NONNULL1
static size_t bigint_to_string_base16(char* str, size_t len, bigint* b) {
    size_t u = 0;

    if(b->size == 0) {
        str[0] = '0';
        return 1;
    }

    while( (u = bigint_len_string_base16(b)) > len) {
        bigint_rshift_overwrite_small_4(b);
    }
    len = u;

    while(b->size) {
        str[--u] = bigint_alphabet[b->words[0] & 0x0F];
        bigint_rshift_overwrite_small_4(b);
    }

    return len;
}

BIGINT_NONNULL1
static size_t bigint_to_string_base10(char* str, size_t len, bigint* b) {
    bigint_word rem;
    size_t u = 0;

    if(b->size == 0) {
        str[0] = '0';
        return 1;
    }

    while( (u = bigint_len_string_base10(b)) > len) {
        bigint_div_mod_10(b,&rem);
    }
    len = u;

    while(b->size) {
        bigint_div_mod_10(b,&rem);
        str[--u] = bigint_alphabet[rem];
    }

    if(u) {
        memmove(&str[0],&str[u],len-u);
        len -= u;
    }

    return len;
}

BIGINT_NONNULL1
static size_t bigint_to_string_base8(char* str, size_t len, bigint* b) {
    size_t u = 0;

    if(b->size == 0) {
        str[0] = '0';
        return 1;
    }

    while( (u = bigint_len_string_base8(b)) > len) {
        bigint_rshift_overwrite_small_3(b);
    }
    len = u;

    while(b->size) {
        str[--u] = bigint_alphabet[b->words[0] & 0x07];
        bigint_rshift_overwrite_small_3(b);
    }

    return len;
}

BIGINT_NONNULL1
static size_t bigint_to_string_base2(char* str, size_t len, bigint* b) {
    size_t u = 0;

    if(b->size == 0) {
        str[0] = '0';
        return 1;
    }

    while( (u = bigint_bitlength(b)) > len) {
        bigint_rshift_overwrite_small_1(b);
    }
    len = u;

    while(b->size) {
        str[--u] = bigint_alphabet[b->words[0] & 0x01];
        bigint_rshift_overwrite_small_1(b);
    }

    return len;
}

BIGINT_API
size_t bigint_to_string(char* str, size_t len, const bigint* b, unsigned int base) {
    bigint tmp = BIGINT_INIT;
    int r;
    size_t u = 0;
    size_t res = 0;

    if(str == NULL) {
        switch(base) {
            case 2: return 2 + b->sign + bigint_len_string_base2(b);
            case 8: return 1 + b->sign + bigint_len_string_base8(b);
            case 0: /* fall-through */
            case 10: return b->sign + bigint_len_string_base10(b);
            case 16: return 2 + b->sign + bigint_len_string_base16(b);
            default: break;
        }
        return 0;
    }

    if(len == 0) return 0;

    if(b->sign) {
        *str = '-';
        str++;
        len--;
        u++;
    }
    if(len == 0) return u;

    if(base == 2) {
        *str = '0';
        str++;
        len--;
        u++;
        if(len == 0) return u;
        *str = 'b';
        str++;
        len--;
        u++;
    } else if(base == 8) {
        *str = '0';
        str++;
        len--;
        u++;
    } else if(base == 16) {
        *str = '0';
        str++;
        len--;
        u++;
        if(len == 0) return u;
        *str = 'x';
        str++;
        len--;
        u++;
    }
    if(len == 0) return u;

    /* TODO can this copy be avoided? */
    if( (r = bigint_copy(&tmp,b)) != 0) return 0;
    switch(base) {
        case 2: res = bigint_to_string_base2(str,len,&tmp); break;
        case 8: res = bigint_to_string_base8(str,len,&tmp); break;
        case 0: /* fall-through */
        case 10: res = bigint_to_string_base10(str,len,&tmp); break;
        case 16: res = bigint_to_string_base16(str,len,&tmp); break;
        default: break;
    }
    bigint_free(&tmp);

    if(res != 0) res += u;
    return res;
}

#endif
