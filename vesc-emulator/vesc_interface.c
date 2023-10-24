//
//  vesc-interface.c
//  vesc-emulator
//
//  Created by Robert Glntz on 10/16/23.
//

#include "vesc_interface.h"

#include <stdio.h>
#include "packet.h"
#include "string.h"
#include "vesc_interface.h"
#include "commands.h"
#include "crc.h"
#define INITIALIZATION_PACKET   0x01, 0x00
#define READ_FW_ID_PACKET    0x02, 0x01, 0x00, 0x00, 0x00, 0x03
#define GET_MCCONF_PACKET    0x02, 0x01, 0x0e, 0xe1, 0xce, 0x03
#define GET_APPCONF_PACKET   0x02, 0x01, 0x11, 0x02, 0x10, 0x03
#define VESC_SHORT_PACKET_START_BYTE    0x02
#define VESC_LONG_PACKET_START_BYTE     0x03
#define VESC_PACKET_OVERHEAD_LENGTH 5   // start (1), short packet length (1), crc (2), stop (1)
#define CRC16_LENGTH
#define COMMAND_CODE_LENGTH
#define START_OF_DATA_INDEX 2
#define START_OF_CRC_INDEX
#define MAX_PACKET_LENGTH 512


//static uint8_t buffer[250000];
static uint8_t buffer[2048]; // todo: BG NOTE:  bldc had huge buffer allocated. Probably for OTA of the VESC firmware.
unsigned char bleTestPacket[6] = { GET_MCCONF_PACKET };  // Write characteristic is received by VESC emulator MTU is 512


unsigned char replyPacketBuf[512];  // Read characteristic is sent by VESC emulator and notified to the app


static unsigned int write_index = 0;
static PACKET_STATE_t state;

void dump_buffer(unsigned char* buf, unsigned int len);

void send_packet_ble(unsigned char *data, unsigned int len) {
    dump_buffer(data, len);
    memcpy(buffer + write_index, data, len);
    write_index += len;
}

void reply_function(unsigned char * replyPacketBuf, unsigned int len) {
    dump_buffer(replyPacketBuf, len);
    packet_send_packet(replyPacketBuf, len, &state);

}

void dump_buffer(unsigned char* buf, unsigned int len) {
    printf("reply packet: %d bytes\r\n", len);
    for (int i = 0; i < len; i++)
    {
        printf("0x");
        printf("%02X", buf[i]);
        printf(",");
    }
    printf("\r");
}


void process_packet(unsigned char *data, unsigned int len) {
    
    uint16_t payload_length = 0;
    uint16_t checksum = 0;
    uint16_t found_checksum = 0;
    uint8_t error = 0;
    uint8_t long_packet = 0;
    
    // Get payload_lengthgth of packet
    switch (*data) {
        case VESC_SHORT_PACKET_START_BYTE:  // short pcaket start byte
            payload_length = data[1];
            break;
        case VESC_LONG_PACKET_START_BYTE: // long packet (greater than 255 bytes)
            payload_length = (data[1] * 256) + data[2];
            long_packet = 1;
            break;
        default:
            printf("RECEIVED PACKET ERROR: Invalid start byte: %0x \r\n", *data);
            payload_length = 0;
            error = -1;
    }
    
    // Check for full packet received
    if (!error) {
        if (len < payload_length + VESC_PACKET_OVERHEAD_LENGTH + long_packet)  {
            printf("RECEIVED PACKET ERROR: Incomplete\r\n");
            error = -2;
        }
    }
    // Sanity check the length - packet too large
    if (!error) {
        if ( (payload_length + VESC_PACKET_OVERHEAD_LENGTH ) > MAX_PACKET_LENGTH) {
            printf("RECEIVED PACKET ERROR: Decoded payload length too large: %0x\r\n", payload_length);
            error = -1;
        }
    }
        
    // Verify stop byte
        if (!error) {
            if ( (*(data + payload_length+ VESC_PACKET_OVERHEAD_LENGTH + long_packet -1) != 0x03 ) ) {
                printf("RECEIVED PACKET ERROR: Valid stop byte not found\r\n");
                error = -1;
            }
        }
    
    // Validate checksum
    if (!error) {
        checksum = crc16(data + START_OF_DATA_INDEX + long_packet, payload_length);
        found_checksum = (uint8_t) (*(data + long_packet + START_OF_DATA_INDEX + 1)) << 8;
        found_checksum = found_checksum + ((uint8_t) *(data + long_packet + START_OF_DATA_INDEX + 2));
        if (checksum ==  found_checksum){
                printf("Packet rx %d bytes:\r\n", payload_length);
                for (int i = 0; i < payload_length + long_packet + 3; i++)
                {
                    printf("0x");
                    printf("%02X", data[i]);
                    printf(",");
                }
                printf("\r");
            commands_process_packet(&data[START_OF_DATA_INDEX+long_packet], payload_length, reply_function);
        }
        else {
            error = -3;
            printf("RECEIVED PACKET ERROR: Bad checksum\r\n");
        }
    }
    
}

void vesc_emul(void){
    // insert code here...    // insert code here...
    printf("VESC Emulator Starting!\n");
    
    // Initialization code frm VESC bldc main
    mc_interface_init();
    commands_init();
    app_configuration *appconf = mempools_alloc_appconf();
    conf_general_read_app_configuration(appconf);
    app_set_configuration(appconf);
    mempools_free_appconf(appconf);
    
    packet_init(send_packet_ble, process_packet, &state);
    process_packet(bleTestPacket, sizeof(bleTestPacket));
    
    
}



