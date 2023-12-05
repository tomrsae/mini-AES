#include "miniaes.h"
#include "keysched.h"

void miniaes_encrypt(
    const unsigned char* p,
    const unsigned char* k,
    unsigned char* buf
) {
    unsigned char round_keys[3][2];

    expand_key(round_keys, k);

    unsigned char word[2];
    word[0] = (p[0] & 0xF0) | (p[1] >> 4);
    word[1] = (p[0] << 4) | (p[1] & 0x0F);

    // First KA
    key_addition(word, round_keys[0]);

    // Round 1
    nibble_sub(word);
    shift_row(word);
    mix_colum(word);

    key_addition(word, round_keys[1]);

    // Round 2
    nibble_sub(word);
    shift_row(word);
    
    key_addition(word, round_keys[2]);

    buf[0] = (word[0] & 0xF0) | (word[1] >> 4);
    buf[1] = (word[0] << 4) | (word[1] & 0x0F); 
}

void miniaes_decrypt(
    const unsigned char* c,
    const unsigned char* k,
    unsigned char* buf
) {

}