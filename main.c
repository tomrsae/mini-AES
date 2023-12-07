#include <stdio.h>
#include "miniaes.h"

int main(int argc, char** argv)
{
	unsigned char p[2] = { 0b10011100, 0b01100011 };
	unsigned char k[2] = { 0b11000011, 0b11110000 };
	unsigned char c_buf[2];

	printf("Plaintext: %d %d\n", p[0], p[1]);

	miniaes_encrypt(p, k, c_buf);

	printf("Ciphertext: %d %d\n", c_buf[0], c_buf[1]);

	unsigned char p_buf[2];
	miniaes_decrypt(c_buf, k, p_buf);

	printf("Decrypted Plaintext: %d %d\n", p_buf[0], p_buf[1]);

	return 0;
}
