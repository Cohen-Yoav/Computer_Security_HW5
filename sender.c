#include "util.h"

ADDR_PTR covert_addr;

void init_covert_channel()
{
    covert_addr = init_shm();
}

inline void send_bit_over_covert_channel(bit val)
{
    if (val == (bit)1) {
        clflush(covert_addr);
    }
}

static void internal_send_byte_over_covert_channel(int val, bool is_EOF)
{
    bit val_in_bits[BYTE_SIZE] = {0};
    byte_to_bits((char)val, val_in_bits);

    int i, j;
    for (i = 0; i < BYTE_SIZE; i++) {
        for (j = 0; j <= NOISE_FILTER; j++) {
            if (!is_EOF) {
                sender_wait_for_notification();
            }
            send_bit_over_covert_channel(val_in_bits[i]);
            notify_receiver();
        }
    }
}

void send_byte_over_covert_channel(int val)
{
    internal_send_byte_over_covert_channel(val, false);
}

void send_byte_over_covert_channel_no_sender_wait(int val)
{
    internal_send_byte_over_covert_channel(val, true);
}


int main(int argc, char **argv)
{
    init_covert_channel();
    int val;
    // Read the file from stdin, byte-by-byte
    //
    do { 
	 val = getchar();

	 send_byte_over_covert_channel(val);
    } while(val != EOF);
    send_byte_over_covert_channel_no_sender_wait(val);
    notify_receiver();
    return 0;
}
