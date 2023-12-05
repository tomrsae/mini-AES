#include "keysched.h"
#include "gfmp.h"

#define MOFX 0x13 // m(x) = x⁴ + x + 1 => 10011
#define RCON1 0x1
#define RCON2 0x2

/*
 * Because mini-AES works with 2x2 matrices consisting of nibbles (see miniAES.pdf),
 * the functions found in this file operates on char arrays of length = 2, hereby
 * referred to as "word"s.
 * One char (byte) represents one matrix row (two nibbles).
 * This helps optimize memory usage and simplifies some of the operations.
*/

static const unsigned char SBOX[16] =
{
    0xE, 0x4, 0xD, 0x1,
    0x2, 0xF, 0xB, 0x8,
    0x3, 0xA, 0x6, 0xC,
    0x5, 0x9, 0x0, 0x7
};

static const unsigned char INVERSE_SBOX[16] =
{
    0xE, 0x3, 0x4, 0x8,
    0x1, 0xC, 0xA, 0xF,
    0x7, 0xD, 0x9, 0x6,
    0xB, 0x2, 0x0, 0x5
};

// Adapted from John Regehr's rotl32.
// Retrieved 16.11.2023 from https://blog.regehr.org/archives/1063
static unsigned char rotl_byte(unsigned char x, int n)
{
  return (x << n) | (x >> (8 - n));
}

void nibble_sub(unsigned char* word)
{
    // a0 a2 -> s(a0) s(a2) = b0 b2
    // a1 a3 -> s(a1) s(a3) = b1 b3 
    word[0] = (SBOX[word[0] >> 4] << 4) | SBOX[word[0] & 0x0F];
    word[1] = (SBOX[word[1] >> 4] << 4) | SBOX[word[1] & 0x0F];
}

void shift_row(unsigned char* word)
{
    // b0 b2 -> b0 b2 = c0 c2
    // b1 b3 -> b3 b1 = c1 c3
    word[1] = rotl_byte(word[1], 4);
}

void mix_column(unsigned char* word)
{
    // c0 c2 -> d0 d2
    // c1 c3 -> d1 d3
    unsigned char c0 = word[0] >> 4;
    unsigned char c1 = word[1] >> 4;
    unsigned char c2 = word[0] & 0x0F;
    unsigned char c3 = word[1] & 0x0F;

    // unsigned char d0 = (3 * c0 % MOFX) ^ (2 * c1 % MOFX);
    // unsigned char d1 = (2 * c0 % MOFX) ^ (3 * c1 % MOFX);
    // unsigned char d2 = (3 * c2 % MOFX) ^ (2 * c3 % MOFX);
    // unsigned char d3 = (2 * c2 % MOFX) ^ (3 * c3 % MOFX);
    unsigned char d0 = multiply(3, c0) ^ multiply(2, c1);
    unsigned char d1 = multiply(2, c0) ^ multiply(3, c1);
    unsigned char d2 = multiply(3, c2) ^ multiply(2, c3);
    unsigned char d3 = multiply(2, c2) ^ multiply(3, c3);

    word[0] = (d0 << 4) | d2;
    word[1] = (d1 << 4) | d3;
}

void key_addition(
    unsigned char* word,
    const unsigned char* key
) {
    word[0] = word[0] ^ key[0];
    word[1] = word[1] ^ key[1];
}

void expand_key(
    unsigned char (*round_keys)[2],
    const unsigned char* key
) {
    unsigned char w0 = key[0] >> 4;
    unsigned char w1 = key[0] & 0x0F;
    unsigned char w2 = key[1] >> 4;
    unsigned char w3 = key[1] & 0x0F;

    round_keys[0][0] = (w0 << 4) | w2;
    round_keys[0][1] = (w1 << 4) | w3;

    unsigned char w4 = w0 ^ SBOX[w3] ^ RCON1;
    unsigned char w5 = w1 ^ w4;
    unsigned char w6 = w2 ^ w5;
    unsigned char w7 = w3 ^ w6;

    round_keys[1][0] = (w4 << 4) | w6;
    round_keys[1][1] = (w5 << 4) | w7;

    unsigned char w8 = w4 ^ SBOX[w7] ^ RCON2;
    unsigned char w9 = w5 ^ w8;
    unsigned char w10 = w6 ^ w9;
    unsigned char w11 = w7 ^ w10;

    round_keys[2][0] = (w8 << 4) | w10;
    round_keys[2][1] = (w9 << 4) | w11;
}