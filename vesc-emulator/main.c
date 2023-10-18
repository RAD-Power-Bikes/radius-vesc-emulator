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
    // insert code here...    // insert code here...
    printf("VESC Emulator Starting!\n");
    
    // Initialization code frm VESC bldc main
    commands_init();
   
//        app_uartcomm_initialize();
        app_configuration *appconf = mempools_alloc_appconf();
//        conf_general_read_app_configuration(appconf);
//        app_set_configuration(appconf);
//        app_uartcomm_start(UART_PORT_BUILTIN);
//        app_uartcomm_start(UART_PORT_EXTRA_HEADER);

//    #ifdef HW_HAS_PERMANENT_NRF
//        conf_general_permanent_nrf_found = nrf_driver_init();
//        if (conf_general_permanent_nrf_found) {
//            rfhelp_restart();
//        } else {
//            nrf_driver_stop();
//            // Set the nrf SPI pins to the general SPI interface so that
//            // an external NRF can be used with the NRF app.
//            spi_sw_change_pins(
//                    HW_SPI_PORT_NSS, HW_SPI_PIN_NSS,
//                    HW_SPI_PORT_SCK, HW_SPI_PIN_SCK,
//                    HW_SPI_PORT_MOSI, HW_SPI_PIN_MOSI,
//                    HW_SPI_PORT_MISO, HW_SPI_PIN_MISO);
//            HW_PERMANENT_NRF_FAILED_HOOK();
//        }
//    #endif

        // Threads
//        chThdCreateStatic(led_thread_wa, sizeof(led_thread_wa), NORMALPRIO, led_thread, NULL);
//        chThdCreateStatic(periodic_thread_wa, sizeof(periodic_thread_wa), NORMALPRIO, periodic_thread, NULL);
//        chThdCreateStatic(flash_integrity_check_thread_wa, sizeof(flash_integrity_check_thread_wa), LOWPRIO, flash_integrity_check_thread, NULL);

//        timeout_init();
//        timeout_configure(appconf->timeout_msec, appconf->timeout_brake_current, appconf->kill_sw_mode);

        mempools_free_appconf(appconf);
    
    packet_init(send_packet, process_packet, &state);
    process_packet(bleTestFwId, sizeof(bleTestFwId));
    
    
    return 0;
}
