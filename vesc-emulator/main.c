//
//  main.c
//  vesc-emulator
//
//  Created by Robert Glntz on 10/15/23.
//

#include <stdio.h>
#include "packet.h"
#include "string.h"
#include "vesc_interface.h"

#define HW_NAME #define HW_NAME "RADIUS"

static uint8_t buffer[250000];
static const unsigned char bleTestFwId[6] = { 0x02, 0x01, 0x11, 0x02, 0x10, 0x03};  // Write characteristic is received by VESC emulator MTU is 512
uint8_t bleTxBuf[512];  // REad characteristic is sent by VESC emulator and notified to the app


static unsigned int write_index = 0;
static PACKET_STATE_t state;

void send_packet(unsigned char *data, unsigned int len) {
    memcpy(buffer + write_index, data, len);
    write_index += len;
}

void process_packet(unsigned char *data, unsigned int len) {
//    printf("Packet rx (%03d bytes): %s\r\n", len, (char*)data + rand_prepend);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    packet_init(send_packet, process_packet, &state);
    process_packet(bleTestFwId, sizeof(bleTestFwId));
    
    
    return 0;
}
