//
//  vesc_interface.h
//  vesc-emulator
//
//  Created by Robert Glntz on 10/16/23.
//

#ifndef vesc_interface_h
#define vesc_interface_h

#include <stdint.h>

#define HW_NAME "RADIUS"

// Firmware version
#define FW_VERSION_MAJOR            5
#define FW_VERSION_MINOR            03
// Set to 0 for building a release and iterate during beta test builds
#define FW_TEST_VERSION_NUMBER        0
#define STM32_UUID_8                ((uint8_t*)0x1FFF7A10)  // required for build.  Identifies MCU type to VESC client

#endif /* vesc_interface_h */
