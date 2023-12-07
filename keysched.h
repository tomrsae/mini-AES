void nibble_sub(unsigned char* word);

void inv_nibble_sub(unsigned char* word);

void shift_row(unsigned char* word);

void inv_shift_row(unsigned char* word);

void mix_column(unsigned char* word);

void inv_mix_column(unsigned char* word);

void key_addition(
    unsigned char* word,
    const unsigned char* key
);

void expand_key(
    unsigned char (*round_keys)[2],
    const unsigned char* key
);