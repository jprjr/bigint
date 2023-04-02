#define BIGINT_IMPLEMENTATION
#include "bigint.h"
#include "utest.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#define BIG_STRING_HEX "80000000002f2a884000000000235fe63"
#define BIG_STRING_DEC "2722258935368420033567525629681967955555"
#define BIG_STRING_OCT "40000000000000571250410000000000000215377143"
#define BIG_STRING_BIN "100000000000000000000000000000000000000000101111001010101000100001000000000000000000000000000000000000000010001101011111111001100011"

#define BIG_STRING_HEX_PREFIX "0x80000000002f2a884000000000235fe63"
#define BIG_STRING_OCT_PREFIX "040000000000000571250410000000000000215377143"
#define BIG_STRING_BIN_PREFIX "0b100000000000000000000000000000000000000000101111001010101000100001000000000000000000000000000000000000000010001101011111111001100011"

#define BIG_STRING_HEX_PREFIX_ALT "x80000000002f2a884000000000235fe63"
#define BIG_STRING_BIN_PREFIX_ALT "b100000000000000000000000000000000000000000101111001010101000100001000000000000000000000000000000000000000010001101011111111001100011"

#define BIG_STRING_HEX_NEGATIVE "-80000000002f2a884000000000235fe63"
#define BIG_STRING_DEC_NEGATIVE "-2722258935368420033567525629681967955555"
#define BIG_STRING_OCT_NEGATIVE "-40000000000000571250410000000000000215377143"
#define BIG_STRING_BIN_NEGATIVE "-100000000000000000000000000000000000000000101111001010101000100001000000000000000000000000000000000000000010001101011111111001100011"

#define BIG_STRING_HEX_NEGATIVE_PREFIX  "-0x80000000002f2a884000000000235fe63"
#define BIG_STRING_OCT_NEGATIVE_PREFIX  "-040000000000000571250410000000000000215377143"
#define BIG_STRING_BIN_NEGATIVE_PREFIX  "-0b100000000000000000000000000000000000000000101111001010101000100001000000000000000000000000000000000000000010001101011111111001100011"

#define BIG_STRING_HEX_TRUNC "80000000002f2a884000000000235"
#define BIG_STRING_DEC_TRUNC "272225893536842003356752562968196795"
#define BIG_STRING_OCT_TRUNC "4000000000000057125041000000000000021537"
#define BIG_STRING_BIN_TRUNC "10000000000000000000000000000000000000000010111100101010100010000100000000000000000000000000000000000000001000110101111111100110"

#define ASSERT_WEQ(a,b) ASSERT_EQ((bigint_word)a,(bigint_word)b)

#if BIGINT_WORD_WIDTH == 1
    static uint8_t BIG_STRING_B00 = 0x63;
    static uint8_t BIG_STRING_B01 = 0xfe;
    static uint8_t BIG_STRING_B02 = 0x35;
    static uint8_t BIG_STRING_B03 = 0x02;
    static uint8_t BIG_STRING_B04 = 0x00;
    static uint8_t BIG_STRING_B05 = 0x00;
    static uint8_t BIG_STRING_B06 = 0x00;
    static uint8_t BIG_STRING_B07 = 0x00;
    static uint8_t BIG_STRING_B08 = 0x84;
    static uint8_t BIG_STRING_B09 = 0xa8;
    static uint8_t BIG_STRING_B10 = 0xf2;
    static uint8_t BIG_STRING_B11 = 0x02;
    static uint8_t BIG_STRING_B12 = 0x00;
    static uint8_t BIG_STRING_B13 = 0x00;
    static uint8_t BIG_STRING_B14 = 0x00;
    static uint8_t BIG_STRING_B15 = 0x00;
    static uint8_t BIG_STRING_B16 = 0x08;


#define INIT_BIGSTRING(x) \
    ASSERT_EQ(bigint_resize(&(x),17),0); \
    x.words[0] = BIG_STRING_B00; \
    x.words[1] = BIG_STRING_B01; \
    x.words[2] = BIG_STRING_B02; \
    x.words[3] = BIG_STRING_B03; \
    x.words[4] = BIG_STRING_B04; \
    x.words[5] = BIG_STRING_B05; \
    x.words[6] = BIG_STRING_B06; \
    x.words[7] = BIG_STRING_B07; \
    x.words[8] = BIG_STRING_B08; \
    x.words[9] = BIG_STRING_B09; \
    x.words[10] = BIG_STRING_B10; \
    x.words[11] = BIG_STRING_B11; \
    x.words[12] = BIG_STRING_B12; \
    x.words[13] = BIG_STRING_B13; \
    x.words[14] = BIG_STRING_B14; \
    x.words[15] = BIG_STRING_B15; \
    x.words[16] = BIG_STRING_B16;

#define ASSERT_BIGSTRING(x) \
    ASSERT_WEQ(a.size,17); \
    ASSERT_WEQ(x.words[0],BIG_STRING_B00); \
    ASSERT_WEQ(x.words[1],BIG_STRING_B01); \
    ASSERT_WEQ(x.words[2],BIG_STRING_B02); \
    ASSERT_WEQ(x.words[3],BIG_STRING_B03); \
    ASSERT_WEQ(x.words[4],BIG_STRING_B04); \
    ASSERT_WEQ(x.words[5],BIG_STRING_B05); \
    ASSERT_WEQ(x.words[6],BIG_STRING_B06); \
    ASSERT_WEQ(x.words[7],BIG_STRING_B07); \
    ASSERT_WEQ(x.words[8],BIG_STRING_B08); \
    ASSERT_WEQ(x.words[9],BIG_STRING_B09); \
    ASSERT_WEQ(x.words[10],BIG_STRING_B10); \
    ASSERT_WEQ(x.words[11],BIG_STRING_B11); \
    ASSERT_WEQ(x.words[12],BIG_STRING_B12); \
    ASSERT_WEQ(x.words[13],BIG_STRING_B13); \
    ASSERT_WEQ(x.words[14],BIG_STRING_B14); \
    ASSERT_WEQ(x.words[15],BIG_STRING_B15); \
    ASSERT_WEQ(x.words[16],BIG_STRING_B16);
#elif BIGINT_WORD_WIDTH == 2
    static uint16_t BIG_STRING_S0 = 0xfe63;
    static uint16_t BIG_STRING_S1 = 0x0235;
    static uint16_t BIG_STRING_S2 = 0x0000;
    static uint16_t BIG_STRING_S3 = 0x0000;
    static uint16_t BIG_STRING_S4 = 0xa884;
    static uint16_t BIG_STRING_S5 = 0x02f2;
    static uint16_t BIG_STRING_S6 = 0x0000;
    static uint16_t BIG_STRING_S7 = 0x0000;
    static uint16_t BIG_STRING_S8 = 0x0008;

#define INIT_BIGSTRING(x) \
    ASSERT_EQ(bigint_resize(&(x),9),0); \
    x.words[0] = BIG_STRING_S0; \
    x.words[1] = BIG_STRING_S1; \
    x.words[2] = BIG_STRING_S2; \
    x.words[3] = BIG_STRING_S3; \
    x.words[4] = BIG_STRING_S4; \
    x.words[5] = BIG_STRING_S5; \
    x.words[6] = BIG_STRING_S6; \
    x.words[7] = BIG_STRING_S7; \
    x.words[8] = BIG_STRING_S8;

#define ASSERT_BIGSTRING(x) \
    ASSERT_WEQ(a.size,9); \
    ASSERT_WEQ(x.words[0],BIG_STRING_S0); \
    ASSERT_WEQ(x.words[1],BIG_STRING_S1); \
    ASSERT_WEQ(x.words[2],BIG_STRING_S2); \
    ASSERT_WEQ(x.words[3],BIG_STRING_S3); \
    ASSERT_WEQ(x.words[4],BIG_STRING_S4); \
    ASSERT_WEQ(x.words[5],BIG_STRING_S5); \
    ASSERT_WEQ(x.words[6],BIG_STRING_S6); \
    ASSERT_WEQ(x.words[7],BIG_STRING_S7); \
    ASSERT_WEQ(x.words[8],BIG_STRING_S8);
#elif BIGINT_WORD_WIDTH == 4

    static uint32_t BIG_STRING_W0 = 0x0235fe63;
    static uint32_t BIG_STRING_W1 = 0x00000000;
    static uint32_t BIG_STRING_W2 = 0x02f2a884;
    static uint32_t BIG_STRING_W3 = 0x00000000;
    static uint32_t BIG_STRING_W4 = 0x00000008;

#define INIT_BIGSTRING(x) \
    ASSERT_EQ(bigint_resize(&(x),5),0); \
    x.words[0] = BIG_STRING_W0; \
    x.words[1] = BIG_STRING_W1; \
    x.words[2] = BIG_STRING_W2; \
    x.words[3] = BIG_STRING_W3; \
    x.words[4] = BIG_STRING_W4;

#define ASSERT_BIGSTRING(x) \
    ASSERT_WEQ(a.size,5); \
    ASSERT_WEQ(x.words[0],BIG_STRING_W0); \
    ASSERT_WEQ(x.words[1],BIG_STRING_W1); \
    ASSERT_WEQ(x.words[2],BIG_STRING_W2); \
    ASSERT_WEQ(x.words[3],BIG_STRING_W3); \
    ASSERT_WEQ(x.words[4],BIG_STRING_W4);
#elif BIGINT_WORD_WIDTH == 8

    static uint64_t BIG_STRING_Q0 = 0x000000000235fe63;
    static uint64_t BIG_STRING_Q1 = 0x0000000002f2a884;
    static uint64_t BIG_STRING_Q2 = 0x0000000000000008;

#define INIT_BIGSTRING(x) \
    ASSERT_EQ(bigint_resize(&(x),3),0); \
    x.words[0] = BIG_STRING_Q0; \
    x.words[1] = BIG_STRING_Q1; \
    x.words[2] = BIG_STRING_Q2;

#define ASSERT_BIGSTRING(x) \
    ASSERT_WEQ(a.size,3); \
    ASSERT_WEQ(x.words[0],BIG_STRING_Q0); \
    ASSERT_WEQ(x.words[1],BIG_STRING_Q1); \
    ASSERT_WEQ(x.words[2],BIG_STRING_Q2);
#else
#error unknownwidth
#endif

#define PREAMBLE \
    bigint a = BIGINT_INIT; \
    bigint b = BIGINT_INIT; \
    bigint c = BIGINT_INIT; \
    bigint d = BIGINT_INIT;

#define CLEANUP \
    bigint_free(&a); \
    bigint_free(&b); \
    bigint_free(&c); \
    bigint_free(&d);

void dump_uint32_spaced(uint32_t val, int first) {
    uint32_t hi, lo;
    hi = val >> 16;
    lo = val & 0xFFFF;
    if(first) {
        if(hi) {
            printf("%x%04x", hi, lo);
        } else if(lo) {
            printf("%x",lo);
        }
    } else {
        printf("%04x%04x", hi, lo);
    }
}

void dump_bigint(const bigint* b) {
    size_t i;
    printf("bits: %lu, sign: %lu, size: %lu, alloc: %lu, value: ",
      bigint_bitlength(b),b->sign,b->size,b->alloc);
    i = b->size;
    if(i == 0) {
        printf("empty");
    } else {
        printf("0x");
        while(i--) {
            dump_uint32_spaced(b->words[i], i == b->size - 1);
        }
    }
    printf("\n");
    fflush(stdout);
}


UTEST(bigint,size) {
#if BIGINT_WORD_WIDTH == 1
    ASSERT_EQ(sizeof(bigint_word),sizeof(uint8_t));
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_EQ(sizeof(bigint_word),sizeof(uint16_t));
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_EQ(sizeof(bigint_word),sizeof(uint32_t));
#elif BIGINT_WORD_WIDTH == 8
    ASSERT_EQ(sizeof(bigint_word),sizeof(uint64_t));
#else
    abort();
#endif
}

UTEST(bigint,init) {
    bigint a;
    bigint_init(&a);
#ifndef BIGINT_NO_MALLOC
    ASSERT_EQ(a.words,NULL);
#endif
    ASSERT_EQ(a.size,(size_t)0);
    ASSERT_EQ(a.alloc,(size_t)0);

    bigint_free(&a);
}

UTEST(bigint,limit) {
    bigint a;
    bigint_init(&a);

    a.limit = 8;
    ASSERT_EQ(bigint_resize(&a,9),BIGINT_ELIMIT);

    bigint_free(&a);
}

UTEST(bigint,from_string_base2) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string_base2(&a,"garbage",7),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string_base2(&a,BIG_STRING_BIN,strlen(BIG_STRING_BIN)),0);
    ASSERT_BIGSTRING(a);

    bigint_free(&a);
}

UTEST(bigint,from_string_base8) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string_base8(&a,"garbage",7),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string_base8(&a,BIG_STRING_OCT,strlen(BIG_STRING_OCT)),0);
    ASSERT_BIGSTRING(a);

    bigint_free(&a);
}

UTEST(bigint,from_string_base10) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string_base10(&a,"garbage",7),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string_base10(&a,BIG_STRING_DEC,strlen(BIG_STRING_DEC)),0);
    ASSERT_BIGSTRING(a);

    bigint_free(&a);
}

UTEST(bigint,from_string_base16) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string_base16(&a,"garbage",7),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string_base16(&a,BIG_STRING_HEX,strlen(BIG_STRING_HEX)),0);
    ASSERT_BIGSTRING(a);

    bigint_free(&a);
}

UTEST(bigint,from_string_einval) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,"garbage",7,0),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,"garbage",7,2),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,"garbage",7,8),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,"garbage",7,16),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,"garbage",7,1),BIGINT_EINVAL);

    ASSERT_EQ(bigint_from_cstring(&a,"garbage",0),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_cstring(&a,"garbage",2),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_cstring(&a,"garbage",8),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_cstring(&a,"garbage",16),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_cstring(&a,"garbage",1),BIGINT_EINVAL);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX,strlen(BIG_STRING_BIN_PREFIX),8),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX,strlen(BIG_STRING_BIN_PREFIX),10),BIGINT_EINVAL);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_PREFIX,strlen(BIG_STRING_OCT_PREFIX),2),BIGINT_EINVAL);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX,strlen(BIG_STRING_HEX_PREFIX),2),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX,strlen(BIG_STRING_HEX_PREFIX),8),BIGINT_EINVAL);
    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX,strlen(BIG_STRING_HEX_PREFIX),10),BIGINT_EINVAL);

    ASSERT_EQ(bigint_from_string(&a,"0x",2,16),BIGINT_EINVAL);

    bigint_free(&a);
}

UTEST(bigint,from_string_zero) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,"0",1,0),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"0",1,2),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"0",1,8),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"0",1,10),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"0",1,16),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"b0",2,0),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"b0",2,2),0);
    ASSERT_WEQ(a.size,0);

    ASSERT_EQ(bigint_from_string(&a,"0x0",3,16),0);
    ASSERT_WEQ(a.size,0);

    bigint_free(&a);
}

UTEST(bigint,from_string_2) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN,strlen(BIG_STRING_BIN),2),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX,strlen(BIG_STRING_BIN_PREFIX),2),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX_ALT,strlen(BIG_STRING_BIN_PREFIX_ALT),2),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX,strlen(BIG_STRING_BIN_PREFIX),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_PREFIX_ALT,strlen(BIG_STRING_BIN_PREFIX_ALT),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_NEGATIVE,strlen(BIG_STRING_BIN_NEGATIVE),2),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_NEGATIVE_PREFIX,strlen(BIG_STRING_BIN_NEGATIVE_PREFIX),0),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_BIN_NEGATIVE_PREFIX,strlen(BIG_STRING_BIN_NEGATIVE_PREFIX),2),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    bigint_free(&a);
}

UTEST(bigint,from_string_8) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT,strlen(BIG_STRING_OCT),8),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_PREFIX,strlen(BIG_STRING_OCT_PREFIX),8),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_PREFIX,strlen(BIG_STRING_OCT_PREFIX),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_NEGATIVE,strlen(BIG_STRING_OCT_NEGATIVE),8),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_NEGATIVE_PREFIX,strlen(BIG_STRING_OCT_NEGATIVE_PREFIX),8),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_OCT_NEGATIVE_PREFIX,strlen(BIG_STRING_OCT_NEGATIVE_PREFIX),0),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    bigint_free(&a);
}

UTEST(bigint,from_string_10) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_DEC,strlen(BIG_STRING_DEC),10),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_DEC,strlen(BIG_STRING_DEC),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_DEC_NEGATIVE,strlen(BIG_STRING_DEC_NEGATIVE),10),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_DEC_NEGATIVE,strlen(BIG_STRING_DEC_NEGATIVE),0),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    bigint_free(&a);
}

UTEST(bigint,from_string_16) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX,strlen(BIG_STRING_HEX),16),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_NEGATIVE,strlen(BIG_STRING_HEX_NEGATIVE),16),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX,strlen(BIG_STRING_HEX_PREFIX),16),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX_ALT,strlen(BIG_STRING_HEX_PREFIX_ALT),16),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX,strlen(BIG_STRING_HEX_PREFIX),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_PREFIX_ALT,strlen(BIG_STRING_HEX_PREFIX_ALT),0),0);
    ASSERT_BIGSTRING(a);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_NEGATIVE_PREFIX,strlen(BIG_STRING_HEX_NEGATIVE_PREFIX),16),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);

    ASSERT_EQ(bigint_from_string(&a,BIG_STRING_HEX_NEGATIVE_PREFIX,strlen(BIG_STRING_HEX_NEGATIVE_PREFIX),0),0);
    ASSERT_BIGSTRING(a);
    ASSERT_EQ(a.sign,(size_t)1);


    bigint_free(&a);
}

UTEST(bigint,from_strings) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_string_base16(&a,"deadbeefc0123456789",strlen("deadbeefc0123456789")),0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,10);
    ASSERT_WEQ(a.words[0],0x89);
    ASSERT_WEQ(a.words[1],0x67);
    ASSERT_WEQ(a.words[2],0x45);
    ASSERT_WEQ(a.words[3],0x23);
    ASSERT_WEQ(a.words[4],0x01);
    ASSERT_WEQ(a.words[5],0xfc);
    ASSERT_WEQ(a.words[6],0xee);
    ASSERT_WEQ(a.words[7],0xdb);
    ASSERT_WEQ(a.words[8],0xea);
    ASSERT_WEQ(a.words[9],0x0d);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,5);
    ASSERT_WEQ(a.words[0],0x6789);
    ASSERT_WEQ(a.words[1],0x2345);
    ASSERT_WEQ(a.words[2],0xfc01);
    ASSERT_WEQ(a.words[3],0xdbee);
    ASSERT_WEQ(a.words[4],0x0dea);
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_WEQ(a.size,3);
    ASSERT_WEQ(a.words[0],0x23456789);
    ASSERT_WEQ(a.words[1],0xdbeefc01);
    ASSERT_WEQ(a.words[2],0x00000dea);
#elif BIGINT_WORD_WIDTH == 8
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0xdbeefc0123456789);
    ASSERT_WEQ(a.words[1],0x0000000000000dea);
#endif

    a.size = 0;
    ASSERT_EQ(bigint_from_string_base16(&a,"DEADBEEFC0123456789",strlen("DEADBEEFC0123456789")),0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,10);
    ASSERT_WEQ(a.words[0],0x89);
    ASSERT_WEQ(a.words[1],0x67);
    ASSERT_WEQ(a.words[2],0x45);
    ASSERT_WEQ(a.words[3],0x23);
    ASSERT_WEQ(a.words[4],0x01);
    ASSERT_WEQ(a.words[5],0xfc);
    ASSERT_WEQ(a.words[6],0xee);
    ASSERT_WEQ(a.words[7],0xdb);
    ASSERT_WEQ(a.words[8],0xea);
    ASSERT_WEQ(a.words[9],0x0d);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,5);
    ASSERT_WEQ(a.words[0],0x6789);
    ASSERT_WEQ(a.words[1],0x2345);
    ASSERT_WEQ(a.words[2],0xfc01);
    ASSERT_WEQ(a.words[3],0xdbee);
    ASSERT_WEQ(a.words[4],0x0dea);
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_WEQ(a.size,3);
    ASSERT_WEQ(a.words[0],0x23456789);
    ASSERT_WEQ(a.words[1],0xdbeefc01);
    ASSERT_WEQ(a.words[2],0x00000dea);
#elif BIGINT_WORD_WIDTH == 8
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0xdbeefc0123456789);
    ASSERT_WEQ(a.words[1],0x0000000000000dea);
#endif

    a.size = 0;
    ASSERT_EQ(bigint_from_string_base8(&a,"07123456123456712",strlen("07123456123456712")),0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,6);
    ASSERT_WEQ(a.words[0],0xca);
    ASSERT_WEQ(a.words[1],0x5d);
    ASSERT_WEQ(a.words[2],0x4e);
    ASSERT_WEQ(a.words[3],0x71);
    ASSERT_WEQ(a.words[4],0x39);
    ASSERT_WEQ(a.words[5],0xe5);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,3);
    ASSERT_WEQ(a.words[0],0x5dca);
    ASSERT_WEQ(a.words[1],0x714e);
    ASSERT_WEQ(a.words[2],0xe539);
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x714e5dca);
    ASSERT_WEQ(a.words[1],0x0000e539);
#elif BIGINT_WORD_WIDTH == 8
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x0000e539714e5dca);
#endif

    bigint_free(&a);
}

UTEST(bigint,to_strings) {
    char buffer[100];
    bigint a = BIGINT_INIT;

#if BIGINT_WORD_WIDTH == 1
    ASSERT_EQ(bigint_resize(&a,10),0);
    a.words[0] = 0x89;
    a.words[1] = 0x67;
    a.words[2] = 0x45;
    a.words[3] = 0x23;
    a.words[4] = 0x01;
    a.words[5] = 0xfc;
    a.words[6] = 0xee;
    a.words[7] = 0xdb;
    a.words[8] = 0xea;
    a.words[9] = 0x0d;
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_EQ(bigint_resize(&a,5),0);
    a.words[0] = 0x6789;
    a.words[1] = 0x2345;
    a.words[2] = 0xfc01;
    a.words[3] = 0xdbee;
    a.words[4] = 0x0dea;
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_EQ(bigint_resize(&a,3),0);
    a.words[0] = 0x23456789;
    a.words[1] = 0xdbeefc01;
    a.words[2] = 0x00000dea;
#elif BIGINT_WORD_WIDTH == 8
    ASSERT_EQ(bigint_resize(&a,2),0);
    a.words[0] = 0xdbeefc0123456789L;
    a.words[1] = 0x0000000000000deaL;
#endif

    ASSERT_WEQ(bigint_to_string_base16(buffer,100,&a),19);

    buffer[19] = '\0';
    ASSERT_STREQ(buffer,"deadbeefc0123456789");

    bigint_free(&a);
}

UTEST(bigint_zero,to_string_base16) {
    char buffer[2];

    bigint a = BIGINT_INIT;

    ASSERT_WEQ(bigint_to_string_base16(buffer,2,&a),1);
    buffer[1] = '\0';
    ASSERT_STREQ(buffer,"0");
}

UTEST(bigint,to_string_base16) {
    char buffer[34];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base16(buffer,34,&a),33);
    buffer[33] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_HEX);
    bigint_free(&a);
}

UTEST(bigint,to_string_base16_trunc) {
    char buffer[30];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base16(buffer,29,&a),29);
    buffer[29] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_HEX_TRUNC);
    bigint_free(&a);
}

UTEST(bigint,to_string_base16_odd) {
    char buffer[35];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    a.words[a.size-1] |= 0x10;

    ASSERT_WEQ(bigint_to_string_base16(buffer,35,&a),34);
    bigint_free(&a);
}

UTEST(bigint,to_string_badbase) {
    char buffer[35];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    a.words[a.size-1] |= 0x10;

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,9),0);
    ASSERT_WEQ(bigint_to_string(buffer,35,&a,9),0);

    bigint_free(&a);
}

UTEST(bigint,to_string_base16_null) {
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    a.words[a.size-1] |= 0x10;

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,16),36);

    bigint_free(&a);
}

UTEST(bigint,to_string_base16_len) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)

    len = 2 + (bigint_bitlength(&a) / 4);

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,16),len);

    bigint_free(&a);
}

UTEST(bigint,positive_to_string_base16) {
    char buffer[36];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(buffer,36,&a,16),35);
    buffer[35] = '\0';
    ASSERT_STREQ(buffer, BIG_STRING_HEX_PREFIX);
    bigint_free(&a);
}

UTEST(bigint,negative_to_string_base16) {
    char buffer[37];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)
    a.sign = 1;

    ASSERT_WEQ(bigint_to_string(buffer,37,&a,16),36);
    buffer[36] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_HEX_NEGATIVE_PREFIX);
}

UTEST(bigint_zero,to_string_base10) {
    char buffer[2];

    bigint a = BIGINT_INIT;

    ASSERT_WEQ(bigint_to_string_base10(buffer,2,&a),1);
    buffer[1] = '\0';
    ASSERT_STREQ(buffer,"0");
}

UTEST(bigint_zero,to_string_base8) {
    char buffer[2];

    bigint a = BIGINT_INIT;

    ASSERT_WEQ(bigint_to_string_base8(buffer,2,&a),1);
    buffer[1] = '\0';
    ASSERT_STREQ(buffer,"0");
}

UTEST(bigint_zero,to_string_base2) {
    char buffer[2];

    bigint a = BIGINT_INIT;

    ASSERT_WEQ(bigint_to_string_base2(buffer,2,&a),1);
    buffer[1] = '\0';
    ASSERT_STREQ(buffer,"0");
}

UTEST(bigint,to_string_base10) {
    char buffer[41];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base10(buffer,41,&a),40);
    buffer[40] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC);
    bigint_free(&a);
}

UTEST(bigint,to_string_base10_trunc) {
    char buffer[37];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base10(buffer,36,&a),36);
    buffer[36] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC_TRUNC);
    bigint_free(&a);
}

UTEST(bigint,to_string_base10_mem) {
    /* the to_length function for base10 will return
     * 2, even though the actual tostring only needs 1 */
    char buffer[3];
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 0x09;

    ASSERT_WEQ(bigint_to_string_base10(buffer,2,&a),1);
    buffer[1] = '\0';
    ASSERT_STREQ(buffer,"9");
    bigint_free(&a);
}

UTEST(bigint,to_string_base10_null) {
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,10),40);
    bigint_free(&a);
}

UTEST(bigint,to_string_base10_length) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_resize(&a,1),0);

    a.words[0] = 0x07;
    ASSERT_WEQ(bigint_to_string(NULL,0,&a,10),1);

    a.words[0] = 0x08;
    ASSERT_WEQ(bigint_to_string(NULL,0,&a,10),2);

    a.words[0] = 0x40;
    ASSERT_WEQ(bigint_to_string(NULL,0,&a,10),3);

    bigint_free(&a);
}

UTEST(bigint,positive_to_string_base10) {
    char buffer[41];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(buffer,41,&a,10),40);
    buffer[40] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC);
    bigint_free(&a);
}

UTEST(bigint,negative_to_string_base10) {
    char buffer[42];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)
    a.sign = 1;

    ASSERT_WEQ(bigint_to_string(buffer,42,&a,10),41);
    buffer[41] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC_NEGATIVE);
    bigint_free(&a);
}

UTEST(bigint,to_string_base8) {
    char buffer[45];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base8(buffer,45,&a),44);
    buffer[44] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_OCT);
    bigint_free(&a);
}

UTEST(bigint,to_string_base8_trunc) {
    char buffer[41];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base8(buffer,40,&a),40);
    buffer[40] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_OCT_TRUNC);
    bigint_free(&a);
}

UTEST(bigint,to_string_base8_null) {
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,8),45);
    bigint_free(&a);
}

UTEST(bigint,to_string_base8_len) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)
    a.words[a.size - 1] |= 0x10;

    len = 1 + ( (bigint_bitlength(&a) / 3) + (bigint_bitlength(&a) % 3 != 0) );

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,8),len);

    bigint_free(&a);
}

UTEST(bigint,to_string_base8_len2) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)
    a.words[a.size - 1] |= 0x30;

    len = 1 + ( (bigint_bitlength(&a) / 3) + (bigint_bitlength(&a) % 3 != 0) );

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,8),len);

    bigint_free(&a);
}

UTEST(bigint,to_string_base8_len3) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)
    a.words[a.size - 1] |= 0x70;

    len = 1 + ((bigint_bitlength(&a) / 3) + (bigint_bitlength(&a) % 3 != 0));

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,8),len);

    bigint_free(&a);
}

UTEST(bigint,to_string_base8_len4) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)
    a.words[a.size - 1] |= 0x80;

    len = 1 + ((bigint_bitlength(&a) / 3) + (bigint_bitlength(&a) % 3 != 0) );

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,8),len);

    bigint_free(&a);
}

UTEST(bigint,positive_to_string_base8) {
    char buffer[46];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(buffer,46,&a,8),45);
    buffer[45] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_OCT_PREFIX);
    bigint_free(&a);
}

UTEST(bigint,negative_to_string_base8) {
    char buffer[47];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)
    a.sign = 1;

    ASSERT_WEQ(bigint_to_string(buffer,47,&a,8),46);
    buffer[46] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_OCT_NEGATIVE_PREFIX);
    bigint_free(&a);
}

UTEST(bigint,to_string_base2) {
    char buffer[133];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base2(buffer,133,&a),132);
    buffer[132] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_BIN);
    bigint_free(&a);
}

UTEST(bigint,to_string_base2_trunc) {
    char buffer[129];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string_base2(buffer,128,&a),128);
    buffer[128] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_BIN_TRUNC);
    bigint_free(&a);
}

UTEST(bigint,to_string_base2_null) {
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,2),134);

    bigint_free(&a);
}

UTEST(bigint,to_string_base2_len) {
    bigint a = BIGINT_INIT;
    size_t len;

    INIT_BIGSTRING(a)

    len = 2 + bigint_bitlength(&a);

    ASSERT_WEQ(bigint_to_string(NULL,0,&a,2),len);

    bigint_free(&a);
}


UTEST(bigint,positive_to_string_base2) {
    char buffer[135];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(buffer,135,&a,2),134);
    buffer[134] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_BIN_PREFIX);
}

UTEST(bigint,negative_to_string_base2) {
    char buffer[136];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a);
    a.sign = 1;

    ASSERT_WEQ(bigint_to_string(buffer,136,&a,2),135);
    buffer[135] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_BIN_NEGATIVE_PREFIX);
}

UTEST(bigint,positive_to_string_base0) {
    char buffer[41];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)

    ASSERT_WEQ(bigint_to_string(buffer,41,&a,0),40);
    buffer[40] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC);
    bigint_free(&a);
}

UTEST(bigint,negative_to_string_base0) {
    char buffer[42];
    bigint a = BIGINT_INIT;

    INIT_BIGSTRING(a)
    a.sign = 1;

    ASSERT_WEQ(bigint_to_string(buffer,42,&a,0),41);
    buffer[41] = '\0';
    ASSERT_STREQ(buffer,BIG_STRING_DEC_NEGATIVE);
    bigint_free(&a);
}

UTEST(bigint_word,add) {
    bigint_word a, b;
    a = (bigint_word)-1;
    b = 1;
    ASSERT_EQ(bigint_word_add(&a,b),1);
    ASSERT_WEQ(a,0);
    ASSERT_EQ(bigint_word_add(&a,(bigint_word)-1),0);
    ASSERT_WEQ(a,-1);
}

UTEST(bigint_word,sub) {
    bigint_word a, b;
    a = 0;
    b = 1;
    ASSERT_EQ(bigint_word_sub(&a,b),1);
    ASSERT_EQ(a,(bigint_word)-1);
    ASSERT_EQ(bigint_word_sub(&a,(bigint_word)-1),0);
    ASSERT_EQ(a,(bigint_word)0);
}

UTEST(bigint_word,mul) {
    bigint_word a, b, c, carry;
    a = (bigint_word)-1;
    b = 2;

    c = a << 1;

    carry = bigint_word_mul(&a,b);
    ASSERT_WEQ(carry,1);
    ASSERT_WEQ(a,c);

    a = (bigint_word)-1;
    b = 4;

    c = a << 2;

    carry = bigint_word_mul(&a,b);
    ASSERT_WEQ(carry,0x03);
    ASSERT_WEQ(a,c);

    a = (bigint_word)-1;
    b = (bigint_word)-1;

    carry = bigint_word_mul(&a,b);
    ASSERT_WEQ(a,0x00000001UL);
    ASSERT_WEQ(carry,-2);
}

UTEST(bigint_word,bitlength) {
    bigint_word a;

    a = (bigint_word)-1;

    ASSERT_WEQ(bigint_word_bitlength(a),sizeof(bigint_word) * CHAR_BIT);
    ASSERT_WEQ(bigint_word_bitlength(1),1);
}

UTEST(bigint,from_word) {
    bigint a;
    bigint_init(&a);
    bigint_from_word(&a,1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.words[0],(bigint_word)1);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_from_word(&a,(bigint_word)-1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.words[0],(bigint_word)-1);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_free(&a);
}

UTEST(bigint,add_word) {
    bigint a;
    bigint_init(&a);
    bigint_add_word(&a,1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_add_word(&a,(bigint_word)-1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_free(&a);
}

UTEST(bigint,mul_word_zero) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_resize(&a,1),0);

    a.words[0] = (bigint_word)-1;
    a.size = 1;

    ASSERT_EQ(bigint_mul_word(&a,0),0);
    ASSERT_WEQ(a.size,0);

    bigint_free(&a);
}

UTEST(bigint,mul_word_one) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_resize(&a,1),0);

    a.words[0] = (bigint_word)-1;
    a.size = 1;

    ASSERT_EQ(bigint_mul_word(&a,1),0);
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],(bigint_word)-1);

    bigint_free(&a);
}

UTEST(bigint,mul_word_two) {
    bigint a = BIGINT_INIT;
    bigint_word res;

    ASSERT_EQ(bigint_resize(&a,1),0);

    a.words[0] = (bigint_word)-1;
    res = a.words[0] << 1;
    a.size = 1;

    ASSERT_EQ(bigint_mul_word(&a,2),0);

    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],res);
    ASSERT_WEQ(a.words[1],0x01);

    bigint_free(&a);
}

UTEST(bigint,mul_word_four) {
    bigint a = BIGINT_INIT;
    bigint_word res;

    ASSERT_EQ(bigint_resize(&a,1),0);

    a.words[0] = (bigint_word)-1;
    res = a.words[0] << 2;
    a.size = 1;

    ASSERT_EQ(bigint_mul_word(&a,4),0);
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],res);
    ASSERT_WEQ(a.words[1],0x03);

    bigint_free(&a);
}

UTEST(bigint,realloc) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_add_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],(bigint_word)-1);

#ifndef BIGINT_NO_MALLOC
    a.size = a.alloc;
#else
    a.size = 8;
#endif
    memset(a.words,0xFF,a.size * sizeof(bigint_word));

    /* this will trigger a realloc */
    ASSERT_EQ(bigint_add_word(&a,1),0);
    ASSERT_EQ(a.size,(size_t)9);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],0);
    ASSERT_WEQ(a.words[2],0);
    ASSERT_WEQ(a.words[3],0);
    ASSERT_WEQ(a.words[4],0);
    ASSERT_WEQ(a.words[5],0);
    ASSERT_WEQ(a.words[6],0);
    ASSERT_WEQ(a.words[7],0);
    ASSERT_WEQ(a.words[8],1);

    bigint_free(&a);
}

UTEST(bigint,add_unsigned) {
    bigint a;
    bigint b;

    bigint_init(&a);
    bigint_init(&b);

    bigint_from_word(&a,(bigint_word)-1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],-1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_from_word(&b,(bigint_word)1);
    ASSERT_NE(b.words,NULL);
    ASSERT_EQ(b.size,(size_t)1);
    ASSERT_WEQ(b.words[0],1);
    ASSERT_EQ(a.sign,(size_t)0);

    ASSERT_EQ(bigint_add_unsigned(&a,&b),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);

    bigint_reset(&b);
    ASSERT_EQ(bigint_add_unsigned(&a,&b),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);

    a.words[0] = (bigint_word)-1;
    a.words[1] = (bigint_word)-1;

    ASSERT_EQ(bigint_from_word(&b,1),0);
    ASSERT_EQ(bigint_add_unsigned(&a,&b),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)3);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],0);
    ASSERT_WEQ(a.words[2],1);

    bigint_free(&a);
    bigint_free(&b);
}

UTEST(bigint,sub_unsigned) {
    bigint a;
    bigint b;

    bigint_init(&a);
    bigint_init(&b);

    bigint_from_word(&a,(bigint_word)-1);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.words[0],(bigint_word)-1);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)0);

    bigint_from_word(&b,(bigint_word)-1);
    ASSERT_NE(b.words,NULL);
    ASSERT_EQ(b.size,(size_t)1);
    ASSERT_EQ(b.words[0],(bigint_word)-1);
    ASSERT_EQ(a.sign,(size_t)0);

    ASSERT_EQ(bigint_sub_unsigned(&a,&b),0);

    ASSERT_EQ(a.size,(size_t)0);

    bigint_from_word(&a,(bigint_word)-1);
    bigint_from_word(&b,1);
    ASSERT_EQ(bigint_sub_unsigned(&a,&b),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.words[0],((bigint_word)-1) - 1);

    bigint_reset(&b);
    ASSERT_EQ(bigint_sub_unsigned(&a,&b),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.words[0],((bigint_word)-1) - 1);

    bigint_from_word(&b,2);
    ASSERT_EQ(bigint_add_unsigned(&a,&b),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_EQ(a.words[0],(bigint_word)0);
    ASSERT_EQ(a.words[1],(bigint_word)1);

    bigint_from_word(&b,1);
    ASSERT_EQ(bigint_sub_unsigned(&a,&b),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.words[0],((bigint_word)-1));

    bigint_free(&a);
    bigint_free(&b);
}

UTEST(bigint_positive,bitlength) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);

    a.words[0] = 0;
    a.words[1] = 1;

    ASSERT_WEQ(bigint_bitlength(&a),(sizeof(bigint_word) * CHAR_BIT) + 1);

    CLEANUP
}

UTEST(bigint_positive,get_set_bit) {
    PREAMBLE
    size_t i;

    ASSERT_EQ(bigint_resize(&a,2),0);

    a.words[0] = 0;
    a.words[1] = 1;

    ASSERT_EQ(bigint_get_bit(&a,sizeof(bigint_word) * CHAR_BIT),1);
    ASSERT_EQ(bigint_get_bit(&a,(sizeof(bigint_word) * CHAR_BIT) + 1),0);
    ASSERT_EQ(bigint_get_bit(&a,(sizeof(bigint_word) * CHAR_BIT) - 1),0);

    for(i=0;i<sizeof(bigint_word) * CHAR_BIT; i++) {
        ASSERT_EQ(bigint_set_bit(&a, i, 1), 0);
    }
    ASSERT_WEQ(a.words[0],((bigint_word)-1));
    ASSERT_WEQ(a.words[1],1);

    ASSERT_EQ(bigint_set_bit(&a, 2 * ((sizeof(bigint_word) * CHAR_BIT)), 1), 0);
    ASSERT_WEQ(a.size,3);
    ASSERT_WEQ(a.words[0],((bigint_word)-1));
    ASSERT_WEQ(a.words[1],1);
    ASSERT_WEQ(a.words[2],1);

    CLEANUP
}

UTEST(bigint_positive,add_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,0),0);
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],-1);

    CLEANUP
}

UTEST(bigint_positive,sub_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,0),0);
    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],-1);

    CLEANUP
}

UTEST(bigint_positive,add_positive_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);

    CLEANUP
}

UTEST(bigint_positive,add_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,((bigint_word)-1) - 1),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.words[0],(bigint_word)-1);

    CLEANUP
}

UTEST(bigint_positive,add_negative_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&a,1),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);

    ASSERT_EQ(bigint_from_word(&b,1),0);
    b.sign = 1;
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],-1);

    CLEANUP
}

UTEST(bigint_positive,add_negative_carry_fromzero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&b,1),0);
    b.words[0] = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);

    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,add_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],-1);

    ASSERT_EQ(bigint_from_word(&b,1),0);
    b.sign = 1;
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],((bigint_word)-1) - 1);

    CLEANUP
}

UTEST(bigint_positive,add_negative_nocarry_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,1),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],1);

    ASSERT_EQ(bigint_from_word(&b,1),0);
    b.sign = 1;
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,add_negative_nocarry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,1),0);

    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_WEQ(a.words[0],1);

    ASSERT_EQ(bigint_from_word(&b,2),0);
    b.sign = 1;
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_positive,add_negative_carry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&a,1),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);

    ASSERT_EQ(bigint_from_word(&b,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&b,2),0);
    ASSERT_NE(b.words,NULL);
    ASSERT_EQ(b.size,(size_t)2);
    ASSERT_WEQ(b.words[0],1);
    ASSERT_WEQ(b.words[1],1);
    b.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_positive,sub_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);
    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],((bigint_word)-1) - 1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,sub_positive_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&a,1),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);
    ASSERT_EQ(a.sign,(size_t)0);

    ASSERT_EQ(bigint_from_word(&b,1),0);
    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],(bigint_word)-1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,sub_positive_nocarry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,1),0);
    ASSERT_EQ(bigint_from_word(&b,2),0);
    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,sub_positive_carry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&a,1),0);
    ASSERT_NE(a.words,NULL);
    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_WEQ(a.words[0],0);
    ASSERT_WEQ(a.words[1],1);
    ASSERT_EQ(a.sign,(size_t)0);

    ASSERT_EQ(bigint_from_word(&b,(bigint_word)-1),0);
    ASSERT_EQ(bigint_add_word(&b,2),0);
    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,mul_long_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_long_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_long_positive_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_long_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;
    b.sign = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,mul_long_negative_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }
    b.sign = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,mul_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_positive_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,mul_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;
    b.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,mul_negative_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }
    b.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,div_positive) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = ((bigint_word)-1);
    b.words[0] = 2;

    ASSERT_EQ(bigint_div_mod(&d,&c,&a,&b),0);

    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],1);
    ASSERT_EQ(c.sign,(size_t)0);

    ASSERT_EQ(d.size,(size_t)1);
    ASSERT_WEQ(d.words[0], (((bigint_word)-1) >> 1));
    ASSERT_EQ(d.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,div_positive_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 0;
    a.words[1] = 1;
    b.words[0] = 2;

    ASSERT_EQ(bigint_div_mod(&d,&c,&a,&b),0);

    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    ASSERT_EQ(d.size,(size_t)1);
    ASSERT_WEQ(d.words[0], (( 1LL << ((sizeof(bigint_word) * CHAR_BIT) - 1))));
    ASSERT_EQ(d.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_positive,div_negative) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = ((bigint_word)-1);
    b.words[0] = 2;
    b.sign = 1;

    ASSERT_EQ(bigint_div_mod(&d,&c,&a,&b),0);

    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],1);
    ASSERT_EQ(c.sign,(size_t)0);

    ASSERT_EQ(d.size,(size_t)1);
    ASSERT_WEQ(d.words[0], (((bigint_word)-1) >> 1));
    ASSERT_EQ(d.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,div_negative_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 0;
    a.words[1] = 1;
    b.words[0] = 2;
    b.sign = 1;

    ASSERT_EQ(bigint_div_mod(&d,&c,&a,&b),0);

    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    ASSERT_EQ(d.size,(size_t)1);
    ASSERT_WEQ(d.words[0], (( 1LL << ((sizeof(bigint_word) * CHAR_BIT) - 1))));
    ASSERT_EQ(d.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_positive,lshift_whole_word) {
    PREAMBLE;

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 1;

    ASSERT_EQ(bigint_lshift(&c,&a,sizeof(bigint_word) * CHAR_BIT),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);

    CLEANUP
}

UTEST(bigint_positive,lshift_half_word) {
    PREAMBLE;

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 1;

    ASSERT_EQ(bigint_lshift(&c,&a,(sizeof(bigint_word) * CHAR_BIT) / 2),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0], (1LL << ((sizeof(bigint_word) * CHAR_BIT) / 2)));

    CLEANUP
}

UTEST(bigint_positive,rshift_whole_word) {
    PREAMBLE;

    ASSERT_EQ(bigint_resize(&a,2),0);
    a.words[0] = 0;
    a.words[1] = 1;

    ASSERT_EQ(bigint_rshift(&c,&a,sizeof(bigint_word) * CHAR_BIT),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_positive,rshift_half_word) {
    PREAMBLE;

    ASSERT_EQ(bigint_resize(&a,2),0);
    a.words[0] = 0;
    a.words[1] = 1LL << (sizeof(bigint_word) * CHAR_BIT)/2;

    ASSERT_EQ(bigint_rshift(&c,&a,(sizeof(bigint_word) * CHAR_BIT)/2),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);

    CLEANUP
}

UTEST(bigint_positive,div_mod_word) {
    PREAMBLE;
    bigint_word rem;

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 255;

    bigint_div_mod_word(&a, &rem, 2);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)0);
    ASSERT_WEQ(a.words[0],127);
    ASSERT_WEQ(rem,1);

    CLEANUP
}

UTEST(bigint_negative,add_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,0),0);
    a.sign = 1;
    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],-1);

    CLEANUP
}

UTEST(bigint_negative,add_positive_nocarry_to_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,1),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);
    a.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,add_positive_carry_to_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);
    ASSERT_EQ(bigint_resize(&b,2),0);

    a.words[0] = 0;
    a.words[1] = 1;
    a.sign = 1;

    b.words[0] = 0;
    b.words[1] = 1;
    b.sign = 0;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,add_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_from_word(&a,(bigint_word)-1),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);
    a.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],((bigint_word)-1)-1);

    CLEANUP
}

UTEST(bigint_negative,add_positive_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);
    ASSERT_EQ(bigint_from_word(&b,1),0);

    ASSERT_EQ(a.size,(size_t)2);
    a.words[0] = 0;
    a.words[1] = 1;
    a.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],((bigint_word)-1));

    CLEANUP
}

UTEST(bigint_negative,add_positive_nocarry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_from_word(&b,2),0);

    ASSERT_EQ(a.size,(size_t)1);
    a.words[0] = 1;
    a.sign = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_negative,add_positive_carry_signflip) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,2),0);
    ASSERT_EQ(bigint_resize(&b,2),0);

    ASSERT_EQ(a.size,(size_t)2);
    ASSERT_EQ(b.size,(size_t)2);
    a.words[0] = 0;
    a.words[1] = 1;
    a.sign = 1;
    b.words[0] = 1;
    b.words[1] = 1;
    b.sign = 0;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)0);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_negative,sub_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);

    a.sign = 1;
    b.sign = 0;

    a.words[0] = 1;
    b.words[0] = 1;

    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],2);

    CLEANUP
}

UTEST(bigint_negative,sub_positive_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);

    a.sign = 1;
    b.sign = 0;

    a.words[0] = (bigint_word)-1;
    b.words[0] = 1;

    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);

    CLEANUP
}

UTEST(bigint_negative,sub_positive_carry_fromzero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&b,1),0);

    b.sign = 0;
    b.words[0] = 1;

    ASSERT_EQ(bigint_sub(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_EQ(c.sign,(size_t)1);
    ASSERT_WEQ(c.words[0],1);

    CLEANUP
}

UTEST(bigint_negative,add_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.sign = 1;
    b.sign = 1;
    a.words[0] = 1;
    b.words[0] = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);

    CLEANUP
}

UTEST(bigint_negative,add_negative_carry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.sign = 1;
    b.sign = 1;
    a.words[0] = (bigint_word)-1;
    b.words[0] = 1;

    ASSERT_EQ(bigint_add(&c,&a,&b),0);
    ASSERT_NE(c.words,NULL);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);

    CLEANUP
}

UTEST(bigint_negative,mul_long_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.sign = 1;
    a.words[0] = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,mul_long_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.sign = 1;
    a.words[0] = 1;
    b.words[0] = 2;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_negative,mul_long_positive_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.sign = 1;
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_negative,mul_long_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;
    a.sign = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,mul_long_negative_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }
    a.sign = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,mul_zero) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    a.words[0] = 1;
    a.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,mul_positive_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;
    a.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_negative,mul_positive_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }
    a.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)1);

    CLEANUP
}

UTEST(bigint_negative,mul_negative_nocarry) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 2;
    a.sign = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)1);
    ASSERT_WEQ(c.words[0],2);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_negative,mul_negative_carry) {
    PREAMBLE

    size_t bits = sizeof(bigint_word) * CHAR_BIT;
    bits /= 2;

    ASSERT_EQ(bigint_resize(&a,1),0);
    ASSERT_EQ(bigint_resize(&b,1),0);
    a.words[0] = 1;
    b.words[0] = 1;
    while(bits--) {
        a.words[0] <<= 1;
        b.words[0] <<= 1;
    }
    a.sign = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_mul(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)2);
    ASSERT_WEQ(c.words[0],0);
    ASSERT_WEQ(c.words[1],1);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_zero,mul_long_positive) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&b,1),0);
    b.words[0] = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_zero,mul_long_negative) {
    PREAMBLE

    ASSERT_EQ(bigint_resize(&b,1),0);
    b.words[0] = 1;
    b.sign = 1;

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_zero,mul_long) {
    PREAMBLE

    ASSERT_EQ(bigint_mul_long(&c,&a,&b),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_zero,rshift) {
    PREAMBLE;

    ASSERT_EQ(bigint_rshift(&c,&a,1),0);
    ASSERT_EQ(c.size,(size_t)0);
    ASSERT_EQ(c.sign,(size_t)0);

    CLEANUP
}

UTEST(bigint_zero,inc) {
    PREAMBLE

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)0);
    ASSERT_WEQ(a.words[0],1);

    CLEANUP
}

UTEST(bigint_zero,dec) {
    PREAMBLE

    ASSERT_EQ(bigint_dec(&a,&a),0);
    ASSERT_EQ(a.size,(size_t)1);
    ASSERT_EQ(a.sign,(size_t)1);
    ASSERT_WEQ(a.words[0],1);

    CLEANUP
}

UTEST(bigint,from_int8) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_u8(&a,128),0);
    ASSERT_EQ(a.sign,(size_t)0);
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x80);

    ASSERT_EQ(bigint_from_i8(&a,-128),0);
    ASSERT_EQ(a.sign,(size_t)1);
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x80);

    bigint_free(&a);
}

UTEST(bigint,from_int16) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_u16(&a,32768),0);
    ASSERT_EQ(a.sign,(size_t)0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x80);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x8000);
#endif

    ASSERT_EQ(bigint_from_i16(&a,-32768),0);
    ASSERT_EQ(a.sign,(size_t)1);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x80);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x8000);
#endif

    ASSERT_EQ(bigint_from_i16(&a,INT16_MAX),0);
    ASSERT_EQ(a.sign,(size_t)0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0xFF);
    ASSERT_WEQ(a.words[1],0x7F);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x7FFF);
#endif

    bigint_free(&a);
}

UTEST(bigint,from_int32) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_u32(&a,((uint32_t)INT32_MAX) + 1),0);
    ASSERT_EQ(a.sign,(size_t)0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,4);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x00);
    ASSERT_WEQ(a.words[2],0x00);
    ASSERT_WEQ(a.words[3],0x80);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x0000);
    ASSERT_WEQ(a.words[1],0x8000);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x80000000);
#endif

    ASSERT_EQ(bigint_from_i32(&a,INT32_MIN),0);
    ASSERT_EQ(a.sign,(size_t)1);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,4);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x00);
    ASSERT_WEQ(a.words[2],0x00);
    ASSERT_WEQ(a.words[3],0x80);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x0000);
    ASSERT_WEQ(a.words[1],0x8000);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x80000000);
#endif

    bigint_free(&a);
}

UTEST(bigint,from_int64) {
    bigint a = BIGINT_INIT;

    ASSERT_EQ(bigint_from_u64(&a,((uint64_t)INT64_MAX) + 1), 0);
    ASSERT_EQ(a.sign,(size_t)0);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,8);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x00);
    ASSERT_WEQ(a.words[2],0x00);
    ASSERT_WEQ(a.words[3],0x00);
    ASSERT_WEQ(a.words[4],0x00);
    ASSERT_WEQ(a.words[5],0x00);
    ASSERT_WEQ(a.words[6],0x00);
    ASSERT_WEQ(a.words[7],0x80);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,4);
    ASSERT_WEQ(a.words[0],0x0000);
    ASSERT_WEQ(a.words[1],0x0000);
    ASSERT_WEQ(a.words[2],0x0000);
    ASSERT_WEQ(a.words[3],0x8000);
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x00000000);
    ASSERT_WEQ(a.words[1],0x80000000);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x8000000000000000);
#endif

    ASSERT_EQ(bigint_from_i64(&a,INT64_MIN),0);
    ASSERT_EQ(a.sign,(size_t)1);
#if BIGINT_WORD_WIDTH == 1
    ASSERT_WEQ(a.size,8);
    ASSERT_WEQ(a.words[0],0x00);
    ASSERT_WEQ(a.words[1],0x00);
    ASSERT_WEQ(a.words[2],0x00);
    ASSERT_WEQ(a.words[3],0x00);
    ASSERT_WEQ(a.words[4],0x00);
    ASSERT_WEQ(a.words[5],0x00);
    ASSERT_WEQ(a.words[6],0x00);
    ASSERT_WEQ(a.words[7],0x80);
#elif BIGINT_WORD_WIDTH == 2
    ASSERT_WEQ(a.size,4);
    ASSERT_WEQ(a.words[0],0x0000);
    ASSERT_WEQ(a.words[1],0x0000);
    ASSERT_WEQ(a.words[2],0x0000);
    ASSERT_WEQ(a.words[3],0x8000);
#elif BIGINT_WORD_WIDTH == 4
    ASSERT_WEQ(a.size,2);
    ASSERT_WEQ(a.words[0],0x00000000);
    ASSERT_WEQ(a.words[1],0x80000000);
#else
    ASSERT_WEQ(a.size,1);
    ASSERT_WEQ(a.words[0],0x8000000000000000);
#endif

    bigint_free(&a);
}

UTEST(bigint,cmp) {
    PREAMBLE

    ASSERT_EQ(bigint_cmp(&a,&b),0);
    ASSERT_EQ(bigint_cmp(&b,&a),0);

    ASSERT_EQ(bigint_from_u8(&a,1),0);
    ASSERT_EQ(bigint_cmp(&a,&b),1);
    ASSERT_EQ(bigint_cmp(&b,&a),-1);

    ASSERT_EQ(bigint_from_u8(&b,1),0);

    ASSERT_EQ(bigint_cmp(&a,&b),0);
    ASSERT_EQ(bigint_cmp(&b,&a),0);

    ASSERT_EQ(bigint_from_u8(&a,0),0);
    ASSERT_EQ(bigint_cmp(&a,&b),-1);
    ASSERT_EQ(bigint_cmp(&b,&a),1);

    ASSERT_EQ(bigint_from_u8(&a,1),0);
    a.sign = 1;
    ASSERT_EQ(bigint_cmp(&a,&b),-1);
    ASSERT_EQ(bigint_cmp(&b,&a),1);

    b.sign = 1;
    ASSERT_EQ(bigint_cmp(&a,&b),0);
    ASSERT_EQ(bigint_cmp(&b,&a),0);

    a.sign = 0;
    ASSERT_EQ(bigint_cmp(&a,&b),1);
    ASSERT_EQ(bigint_cmp(&b,&a),-1);

    CLEANUP
}

UTEST(bigint,to_uint8) {
    PREAMBLE;

    uint8_t val = 255;

    ASSERT_EQ(bigint_from_u8(&a,0),0);
    ASSERT_EQ(bigint_to_u8(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u8(&a,255),0);
    ASSERT_EQ(bigint_to_u8(&val,&a),0);
    ASSERT_EQ(val,255);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_u8(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_int8) {
    PREAMBLE;

    int8_t val = 0x7F;

    ASSERT_EQ(bigint_from_u8(&a,0),0);
    ASSERT_EQ(bigint_to_i8(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u8(&a,0x7F),0);
    ASSERT_EQ(bigint_to_i8(&val,&a),0);
    ASSERT_EQ(val,0x7F);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_i8(&val,&a),1);

    a.sign = 1;
    ASSERT_EQ(bigint_to_i8(&val,&a),0);
    ASSERT_EQ(val,-0x80);

    ASSERT_EQ(bigint_dec(&a,&a),0);
    ASSERT_EQ(bigint_to_i8(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_uint16) {
    PREAMBLE;

    uint16_t val = UINT16_MAX;

    ASSERT_EQ(bigint_from_u16(&a,0),0);
    ASSERT_EQ(bigint_to_u16(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u16(&a,UINT16_MAX),0);
    ASSERT_EQ(bigint_to_u16(&val,&a),0);
    ASSERT_EQ(val,UINT16_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_u16(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_int16) {
    PREAMBLE;

    int16_t val = INT16_MAX;

    ASSERT_EQ(bigint_from_u16(&a,0),0);
    ASSERT_EQ(bigint_to_i16(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u16(&a,INT16_MAX),0);
    ASSERT_EQ(bigint_to_i16(&val,&a),0);
    ASSERT_EQ(val,INT16_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_i16(&val,&a),1);

    a.sign = 1;
    ASSERT_EQ(bigint_to_i16(&val,&a),0);
    ASSERT_EQ(val,INT16_MIN);

    ASSERT_EQ(bigint_dec(&a,&a),0);
    ASSERT_EQ(bigint_to_i16(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_uint32) {
    PREAMBLE;

    uint32_t val = UINT32_MAX;

    ASSERT_EQ(bigint_from_u32(&a,0),0);
    ASSERT_EQ(bigint_to_u32(&val,&a),0);
    ASSERT_WEQ(val,0);

    ASSERT_EQ(bigint_from_u32(&a,UINT32_MAX),0);
    ASSERT_EQ(bigint_to_u32(&val,&a),0);
    ASSERT_EQ(val,UINT32_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_u32(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_int32) {
    PREAMBLE;

    int32_t val = INT32_MAX;

    ASSERT_EQ(bigint_from_u32(&a,0),0);
    ASSERT_EQ(bigint_to_i32(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u32(&a,INT32_MAX),0);
    ASSERT_EQ(bigint_to_i32(&val,&a),0);
    ASSERT_EQ(val,INT32_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_i32(&val,&a),1);

    a.sign = 1;
    ASSERT_EQ(bigint_to_i32(&val,&a),0);
    ASSERT_EQ(val,INT32_MIN);

    ASSERT_EQ(bigint_dec(&a,&a),0);
    ASSERT_EQ(bigint_to_i32(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_uint64) {
    PREAMBLE;

    uint64_t val = UINT64_MAX;

    ASSERT_EQ(bigint_from_u64(&a,0),0);
    ASSERT_EQ(bigint_to_u64(&val,&a),0);
    ASSERT_WEQ(val,0);

    ASSERT_EQ(bigint_from_u64(&a,UINT64_MAX),0);
    ASSERT_EQ(bigint_to_u64(&val,&a),0);
    ASSERT_EQ(val,UINT64_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_u64(&val,&a),1);

    CLEANUP;
}

UTEST(bigint,to_int64) {
    PREAMBLE;

    int64_t val = INT64_MAX;

    ASSERT_EQ(bigint_from_u64(&a,0),0);
    ASSERT_EQ(bigint_to_i64(&val,&a),0);
    ASSERT_EQ(val,0);

    ASSERT_EQ(bigint_from_u64(&a,INT64_MAX),0);
    ASSERT_EQ(bigint_to_i64(&val,&a),0);
    ASSERT_EQ(val,INT64_MAX);

    ASSERT_EQ(bigint_inc(&a,&a),0);
    ASSERT_EQ(bigint_to_i64(&val,&a),1);

    a.sign = 1;
    ASSERT_EQ(bigint_to_i64(&val,&a),0);
    ASSERT_EQ(val,INT64_MIN);

    ASSERT_EQ(bigint_dec(&a,&a),0);
    ASSERT_EQ(bigint_to_i64(&val,&a),1);

    CLEANUP;
}

UTEST_MAIN();
