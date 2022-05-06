#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string>


uint64_t* memory_region;


int main(int argc, char* argv[]) {
	if (argc <= 1 || argc >= 5) {
		//std::cerr << "Usage: " << argv[0] << " [region_size_bytes] [trials]" << std::endl;
		return -1;
	}
	uint64_t region_size = std::stoll(argv[1]);
	uint64_t trials = std::stoll(argv[2]);


	memory_region = (uint64_t*)malloc(region_size);
	
	for (uint64_t t = 0; t < trials; t++) {
		// Start benchmak
		uint8_t bank = 0;
		for (uint64_t i = 0; i < region_size / sizeof(uint64_t); i+=8) {
			uint64_t data = 0x0;
			data += (((uint64_t)memory_region[i+8*0] << (bank * 8)) >>  0) & 0xFF00000000000000;
			data += (((uint64_t)memory_region[i+8*1] << (bank * 8)) >>  8) & 0x00FF000000000000;
			data += (((uint64_t)memory_region[i+8*2] << (bank * 8)) >> 16) & 0x0000FF0000000000;
			data += (((uint64_t)memory_region[i+8*3] << (bank * 8)) >> 24) & 0x000000FF00000000;
			data += (((uint64_t)memory_region[i+8*4] << (bank * 8)) >> 32) & 0x00000000FF000000;
			data += (((uint64_t)memory_region[i+8*5] << (bank * 8)) >> 40) & 0x0000000000FF0000;
			data += (((uint64_t)memory_region[i+8*6] << (bank * 8)) >> 48) & 0x000000000000FF00;
			data += (((uint64_t)memory_region[i+8*7] << (bank * 8)) >> 56) & 0x00000000000000FF;
			memory_region[i] = data;
		}
		// Stop benchmark
	}
	return 0;
}
