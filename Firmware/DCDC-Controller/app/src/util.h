#include <stdint.h>
#include <stdio.h>

static void printHex(const uint8_t* buf, uint16_t size) {
	printf("       ");
	for(uint8_t i = 0; i < 16; i++) {
		printf("%1X  ", i);
	}
	printf("\n%04X  ", 0);
	for(uint16_t i = 0; i < size; i++) {
		printf("%02X ", buf[i]);
		if(i % 16 == 15) {
			printf("\n%04X  ", i+1);
		}
	}
	printf("\n");
}