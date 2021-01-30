#include "util.h"

ADDR_PTR covert_addr;

void init_covert_channel()
{
	covert_addr = init_shm();
}

inline bit receive_bit_over_covert_channel()
{
	bit recovered_bit;
	CYCLES latency_check = measure_access_time_to_addr(covert_addr);
	if (latency_check > MISS_LATENCY) {
		recovered_bit = 1;
	}
	else {
		recovered_bit = 0;
	}
	return recovered_bit;
}

int receive_val_over_covert_channel()
{
	bit restored_val_in_bits[BYTE_SIZE] = {0};
	notify_sender();

	int i, j, res;
	char recovered_val = 0;

	for (i = 0; i < BYTE_SIZE; i++) {
		res = 0;
		for(j = 0; j <= NOISE_FILTER; j++) {
			receiver_wait_for_notification();
			res += receive_bit_over_covert_channel();
			notify_sender();
		}

		restored_val_in_bits[i] = (res >= NOISE_FILTER / 2); 
	}

	bits_to_byte(restored_val_in_bits, &recovered_val);

	return (int)recovered_val;
}

void receive_over_covert_channel()
{
	int val;
       	do {
		// Read the file over the covert-channel bit-by-bit.
		// 
		val = receive_val_over_covert_channel();
		if (val != EOF) {
			putchar(val);
		}		
	} while (val != EOF);
}

int main(int argc, char **argv)
{
	init_covert_channel();
	receive_over_covert_channel();

	return 0;
}

