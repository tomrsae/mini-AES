#include "keysched.h"

/*
 * Because mini-AES works with 2x2 matrices consisting of nibbles (see miniAES.pdf),
 * the functions found in this file operates on char arrays of length = 2, hereby
 * referred to as "word"s.
 * One char (byte) represents one matrix row (two nibbles).
 * This optimizes memory usage and simplifies some of the operations.
*/

static const unsigned char SBOX[16] =
{
    0b1110, 0b0100, 0b1101, 0b0001,
    0b0010, 0b1111, 0b1011, 0b1000,
    0b0011, 0b1010, 0b0110, 0b1100,
    0b0101, 0b1001, 0b0000, 0b0111
};

static const unsigned char INVERSE_SBOX[16] =
{
    0b1110, 0b0011, 0b0100, 0b1000,
    0b0001, 0b1100, 0b1010, 0b1111,
    0b0111, 0b1101, 0b1001, 0b0110,
    0b1011, 0b0010, 0b0000, 0b0101
};

// m(x) = x⁴ + x + 1
static const MOFX = 0b10011;

// Adapted from John Regehr's rotl32.
// retrieved 16.11.2023 from https://blog.regehr.org/archives/1063
unsigned char rotl_byte(unsigned char x, int n)
{
  return (x << n) | (x >> (8 - n));
}

void nibble_sub(unsigned char* word)
{
    // a0 a2 -> s(a0) s(a2) = b0 b2
    // a1 a3 -> s(a1) s(a3) = b1 b3 
    word[0] = SBOX[word[0] >> 4] | SBOX[word[0] & 0x0F];
    word[1] = SBOX[word[1] >> 4] | SBOX[word[1] & 0x0F];
}

void shift_row(unsigned char* word)
{
    // b0 b2 -> b0 b2 = c0 c2
    // b1 b3 -> b3 b1 = c1 c3
    word[1] = rotl_byte(word[1], 4);
}

void mix_colum(unsigned char* word)
{
    // c0 c2 -> d0 d2
    // c1 c3 -> d1 d3
    char c0 = word[0] >> 4;
    char c1 = word[1] >> 4;
    char c2 = word[0] & 0x0F;
    char c3 = word[1] & 0x0F;

    char d0 = (3 * c0 % MOFX) + (2 * c1 % MOFX);
    char d1 = (2 * c0 % MOFX) + (3 * c1 % MOFX);
    char d2 = (3 * c2 % MOFX) + (2 * c3 % MOFX);
    char d3 = (2 * c2 % MOFX) + (3 * c3 % MOFX);

    word[0] = (d0 << 4) | d2;
    word[1] = (d1 << 4) | d3;
}