void nibble_sub(unsigned char* word);

void shift_row(unsigned char* word);

void mix_colum(unsigned char* word);

void key_addition(
    unsigned char* word,
    const unsigned char* key
);

void expand_key(
    unsigned char (*round_keys)[2],
    const unsigned char* key
);