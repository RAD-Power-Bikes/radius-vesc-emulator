//
//  vesc_interface.h
//  vesc-emulator
//
//  Created by Robert Glntz on 10/16/23.
//

#ifndef vesc_interface_h
#define vesc_interface_h

#include <stdint.h>
#include<pthread.h>
#include<stdlib.h>
#include <stdint.h>
#include<unistd.h>
#include "mempools.h"
#include "conf_general.h"
#include "confgenerator.h"
#include "appconf_default.h"
#include "appconf_custom.h"
#include "app.h"
#include "mc_interface.h"

//#define HW_NAME "RADIUS"
//
//// Firmware version
//#define FW_VERSION_MAJOR            5
//#define FW_VERSION_MINOR            03
//// Set to 0 for building a release and iterate during beta test builds
//#define FW_TEST_VERSION_NUMBER        0
//#define STM32_UUID_8                0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40  // required for build.  Identifies MCU type to VESC client

// chbios entities converted to c
#define mutex_t pthread_mutex_t
#define chMtxLock pthread_mutex_lock
#define chMtxUnlock pthread_mutex_unlock
#define chMtxObjectInit(a)  pthread_mutex_init(a,NULL)
void vesc_emul(void);
void process_packet(unsigned char *data, unsigned int len);
void process_vesc_app_data( uint8_t* app_packet_data, uint16_t app_packet_len, uint8_t* reply_packet_buffer, uint16_t* reply_packet_length);


#endif /* vesc_interface_h */
