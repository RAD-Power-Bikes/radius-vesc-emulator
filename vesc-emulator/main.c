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
#include "commands.h"
#define READ_FW_ID_PACKET   0x01, 0x00

static uint8_t buffer[250000];
unsigned char bleTestFwId[2] = { READ_FW_ID_PACKET};  // Write characteristic is received by VESC emulator MTU is 512


unsigned char replyPacketBuf[512];  // REad characteristic is sent by VESC emulator and notified to the app


static unsigned int write_index = 0;
static PACKET_STATE_t state;

void send_packet(unsigned char *data, unsigned int len) {
    memcpy(buffer + write_index, data, len);
    write_index += len;
}

void reply_function(unsigned char * replyPacketBuf, unsigned int len) {
    printf("reply packet: %d bytes\r\n", len);
    for (int i = 0; i < len; i++)
    {
        printf("0x");
        printf("%02X", replyPacketBuf[i]);
        printf(",");
    }
    printf("\r");
    
}

void process_packet(unsigned char *data, unsigned int len) {
    printf("Packet rx (%03d bytes): %s\r\n", len, (char*)data);
    commands_process_packet(data, len, reply_function);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    packet_init(send_packet, process_packet, &state);
    process_packet(bleTestFwId, sizeof(bleTestFwId));
    
    
    return 0;
}
