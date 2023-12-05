#include <stdio.h>
#include "miniaes.h"

int main(int argc, char** argv)
{
	unsigned char p[2] = { 0b10011100, 0b01100011 }; // 1001 1100 0110 0011
	unsigned char k[2] = { 0b11000011, 0b11110000 }; // 1100 0011 1111 0000
	unsigned char buf[2]; // SHOULD BE: 0111 0010 1100 0110 (114 198)

	miniaes_encrypt(p, k, buf);

	printf("%d %d\n", buf[0], buf[1]);

	return 0;
}
