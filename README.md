# bigint.h

A single-file C library for performing big-integer arithmetic,
including functions for converting from/to strings.

This isn't meant to be the fastest thing in the world, I did
this mostly as a learning exercise.

## Building

In one source file, define `BIGINT_IMPLEMENTATION`, then include
`bigint.h`.

```c
#define BIGINT_IMPLEMENTATION
#include "bigint.h"
```

By default, all math operations use 32-bit, unsigned integers
with 64-bit unsigned integers for temporary additions, carries,
etc.

If for some reason you don't have a larger type available for
multiplication, you can define `BIGINT_SINGLE_WORD_ONLY`, and
the library will fall back to cross-product multiplication.

You can customize the width by setting `BIGINT_WORD_WIDTH` to any of:

* `1` (uses `uint8_t`)
* `2` (uses `uint16_t)`)
* `4` (uses `uint32_t`)
* `8` (uses `uint64_t`)

In the case of using a 64-bit word width, the library will
try to perform 128-bit multiplications if support is detected via
C macros, and fall back to cross-product multiplication otherwise.

If you need some other type you can define `BIGINT_WORD_TYPE`. You
still need to also define the type width, in bytes. You can also
specify a double-width type, `BIGINT_DWORD_TYPE`.

By default, all bigint objects are limited to using 4096 bytes of
memory, this can be customized per-object by setting the `limit`
field. The intention is for you to not, say, run out of memory while
parsing a never-ending string of data. You can also set the default
by defining `BIGINT_DEFAULT_LIMIT`.

Finally, if you want to have everything be statically-allocated, you
can define `BIGINT_NO_MALLOC`. This will cause all bigints to use
a fixed-size number of bytes (set to `BIGINT_DEFAULT_LIMIT`).

If you define any of:

* `BIGINT_WORD_WIDTH`
* `BIGINT_WORD_TYPE`
* `BIGINT_DEFAULT_LIMIT`
* `BIGINT_NO_MALLOC`

They will need to be defined before any `#include` of the library,
since those values will affect things like function signatures and
the structure definition.

## Usage

bigint objects can be allocated however you wish, then initialized
with either `bigint_init()` or using the defined `BIGINT_INIT`, ie:

```c
bigint a = BIGINT_INIT; /* static initialization */
bigint *b = malloc(sizeof(bigint));
bigint_init(b); /* initialize with function */
```

When no longer used, you'll need to use `bigint_free()` to free used
memory.

By default, bigint objects are initialized to represent 0.

You can set a starting value from various integer types:

```c
bigint_from_u8(&b,5);
bigint_from_i32(&b,-2147483648);
```

Or parse a string, provide the string and the base to use (2, 8, 10, 16, or 0 for auto):

```c
bigint_from_cstring(&b,"12345",10);
bigint_from_cstring(&b,"-12345",10);
bigint_from_cstring(&b,"0x100",16);
bigint_from_cstring(&b,"-0x100",16);
bigint_from_cstring(&b,"-0b100",2);

/* bigint_from_cstring is a wrapper around bigint_from_string - bigint_from_string
takes a length parameter, bigint_from_cstring just uses strlen() */
bigint_from_string(&b,"12345",5,10);
bigint_from_string(&b,"-12345",6,10);
bigint_from_string(&b,"0x100",5,16);
bigint_from_string(&b,"-0x100",6,16);
bigint_from_string(&b,"-0b100",6,2);
```

You can copy bigints:

```c
bigint_copy(&dup,&b); /* dup now has the same data as b */
```

You can increment, decrement, add, subtract, multiply, and divide bigints.

Functions do not modify your input arguments.

```c
/* equivalent to c = b + 1 */
bigint_inc(&c, &b);

/* equivalent to c = b - 1 */
bigint_dec(&c, &b);

/* equivalent to c = a + b */
bigint_add(&c, &a, &b);

/* equivalent to c = a - b */
bigint_sub(&c, &a, &b);

/* equivalent to c = a * b */
bigint_mul(&c, &a, &b);

/* returns the quotient and remainder of an operation at once,
   similar to:
     q = a / b
     r = a % b
*/
bigint_div_mod(&q, &r, &a, &b);
```

There's also left-shifting and right-shifting:

```c
bigint_lshift(&res, &a, 128); /* equivalent to res = a << 128 */
bigint_rshift(&res, &a, 128); /* equivalent to res = a >> 128 */
```

All functions work on temporary values and perform a copy at the end,
meaning it's safe to do things like:

```c
bigint_add(&b, &b, &a); /* equivalent to b += a
```

In some cases there's faster versions of math operations if
overwriting the original is OK (for example, left-shifting and
right-shifting). Those functions are:

```c
bigint_lshift_overwrite(bigint *b, size_t bits); /* equivalent to b <<= bits */
bigint_rshift_overwrite(bigint *b, size_t bits); /* equivalent to b >>= bits */

/* this one takes bigint_words as parameters (default uint32_t), it's a much faster division,
equivalent to:
  remainder = numerator % denominator
  numerator /= denominator
*/
bigint_div_mod_word(bigint *numerator, bigint_word* remainder, bigint_word denominator);
```

Finally there's a function to write the bigint out as a string. It accepts
a buffer, the size of the buffer, and the base to use. Notably, it does NOT write out a NULL
character. It *does* include a prefix based on the base:

* base 2 - prefix with '0b'
* base 8 - prefix with '0'
* base 16 - prefix with '0x'

```c
char buffer[101];
/* always pass size-1 to leave room for null terminator */
buffer[bigint_to_string(buffer,100,&b,10)] = '\0';
```

If you need a length estimate you can pass `NULL` as the buffer:

```c
size_t needed = bigint_to_string(NULL,0, &b, 10);
if(!needed) return some_kind_of_error;

/* add extra byte for '\0' terminator */
char *buffer = malloc(needed+1);
buffer[bigint_to_string(buffer,needed,&b,10)] = '\0';
```

All functions (besides `bigint_to_string`) return an integer,
with 0 meaning success, or one of the following error codes:

* `BIGINT_ENOMEM` - an attempt to resize a bigint ran out of memory.
* `BIGINT_ELIMIT` - an attempt to resize a bigint would exceed the `limit` field.
* `BIGINT_EINVAL` - returned when parsing a string with invalid data.

`bigint_to_string` returns the number of characters required/written,
returning `0` indicates some kind of error.

## LICENSE

BSD Zero Clause (see the `LICENSE` file).

Some files are third-party and have their own licensing:

* `utest.h`: Public Domain / Unlicense, see file for details.
    * Note, `utest.h` is not required for library usage, this is for running automated tests.
