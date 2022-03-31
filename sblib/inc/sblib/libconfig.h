/**************************************************************************//**
 * @addtogroup SBLIB Selfbus KNX-Library sblib
 * @defgroup SBLIB_CORE CORE
 * @ingroup SBLIB
 * @brief    set pre-processor symbols which apply to all sblib build-configs
 * @note     for serial debugging output it is recommended to use
 *           PIO2_7 and PIO2_8 at high baud rate e.g 1.5MB
 *           define below SERIAL_RX_PIN and SERIAL_TX_PIN
 *           or set it in your application
 *           serial.setRxPin(PIO2_7);
 *           serial.setTxPin(PIO2_8);
 *           serial.begin(1500000);  1.5Mbaud
 *           serial object is defined in serial.h
 *
 * @{
 *
 * @file   libconfig.h
 * @author Darthyson <darth@maptrack.de> Copyright (c) 2021
 * @author Horst Rauch Copyright (c) 2021
 * @bug No known bugs.
 ******************************************************************************/

/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 -----------------------------------------------------------------------------*/

#ifndef SBLIB_LIBCONFIG_H_
#define SBLIB_LIBCONFIG_H_

/**************************************************************************//**
 * Things to configure in the sblib
 ******************************************************************************/

#if defined (__LPC11XX__)
// #   define SERIAL_RX_PIN PIO3_1 //!< on a TS_ARM Controller
// #   define SERIAL_TX_PIN PIO3_0 //!< on a TS_ARM Controller
#   define SERIAL_RX_PIN PIO2_7 //!< on a 4TE-ARM Controller pin 1 on connector SV3 (ID_SEL)
#   define SERIAL_TX_PIN PIO2_8 //!< on a 4TE-ARM Controller pin 2 on connector SV3 (ID_SEL)
#endif

//#define SERIAL_SPEED 115200 //!< baudrate e.g. 115200, 230400, 576000 serial port should run for debugging
//#define SERIAL_SPEED 576000
//#define SERIAL_SPEED 1500000

//#define INCLUDE_SERIAL      //!< library should open serial for us

//#define ROUTER /// \todo ROUTER not implemented, Lib will be compiled for usage in a router- Phy Addr 0.0.0 is allowed
#define STARTBIT_CAP_RESET  //!< enable capture event for startbit is resetting the counter
#define TIMER_BASED_RX //!< used 104us timer for sampling rx bits instead of capture of a falling edge





/**************************************************************************//**
 * Debugging stuff follows below
 *
 * for every debugging or dumping #define, make sure to add a #undef below,
 * so it won't make it into the release version
 ******************************************************************************/

//#define DEBUG_BUS           //!< enable dumping of state machine interrupt data e.g timer values, mapping of ports in serial.cpp
//#define DEBUG_BUS_BITLEVEL  //!< extension used with DEBUG_BUS to dump interrupt of each bit - use with care due to easy overflow of the trace buffer
//#define BUSMONITOR				//!< enables bus monitoring: dump of all bus traffic incl timing info, no sending, no processing of rx-telegrams
//to avoid trace buffer overflow DUMP_TELEGRAMS should not be used in parallel with DEBUG_BUS or DEBUG_BUS_BITLEVEL"
//#define DUMP_TELEGRAMS  //!< dump rx and tx telegrams, incl received ack and timing info over serial interface
//#define USEPIO_FOR_TEL_END_IND //<! toggle PIN PIO1_4 at end of a received telegram
//#define DUMP_COM_OBJ    //!< dump object handling information on app-server level over serial interface
//#define DUMP_MEM_OPS    //!< dump APCI_MEMORY_WRITE_PDU and APCI_MEMORY_READ_PDU over serial interface
//#define DUMP_PROPERTIES //!< dump interface object and property handling over serial interface
//#define DUMP_TL4        //!< dump transport layer 4 protocol handling over serial interface

/// \todo following #defines should be moved to this libconfig.h file
// IAP_EMULATION        /// \todo from platform.h & analog_pin.cpp (used for catch-unit tests of the sblib)
// SPI_BLOCK_TRANSFER   /// \todo from spi.h & spi.cpp
// DEBUG                /// \todo from utils.h
// SERIAL_WRITE_DIRECT  /// \todo from serial.cpp
// BH1750_DEBUG         /// \todo from in bh1750.h
// LOGGING              /// \todo from in bh1750.h
// DEBUG_ACTIVE         /// \todo from in debug.cpp

/// \todo find a better place for this bcu-type information
/// variants of BCU_TYPE: BCU1_TYPE, BCU1_12, 0x10, 0x20, 20, 0x0701, 0x0705, 0x7B0
///     BCU1_TYPE
///     BIM112_TYPE
///     SYSTEM_B_TYPE

/// BCU_NAME
/// MASK_VERSION

/**************************************************************************//**
 *
 * Change things below only, if you know what you are doing.
 *
 * for every debugging or dumping #define above, make sure to add a #undef below
 *
 ******************************************************************************/

// remove any debugging and dumping stuff from release versions
#ifndef DEBUG
#   undef DEBUG_BUS
#   undef DEBUG_BUS_BITLEVEL
#   undef BUSMONITOR
#   undef DUMP_TELEGRAMS
#	undef USEPIO_FOR_TEL_END_IND
#   undef DUMP_COM_OBJ
#   undef DUMP_MEM_OPS
#   undef DUMP_PROPERTIES
#   undef DUMP_TL4
#   undef LOGGING
#   undef BH1750_DEBUG
#   undef DEBUG_ACTIVE
#endif

// make sure if DEBUG_BUS_BITLEVEL is defined also DEBUG_BUS is defined
#if defined(DEBUG_BUS_BITLEVEL) && !defined(DEBUG_BUS)
#  define DEBUG_BUS
#  warning "DEBUG_BUS_BITLEVEL, can only be used together with DEBUG_BUS"
#endif

// make sure if BUSMONITOR is defined also DUMP_TELEGRAMS is defined
#if defined(BUSMONITOR) && !defined(DUMP_TELEGRAMS)
#  define DUMP_TELEGRAMS
#  warning "BUSMONITOR can only be used together with DUMP_TELEGRAMS"
#endif

//to avoid trace buffer overflow DUMP_TELEGRAMS should not be used in parallel with DEBUG_BUS or DEBUG_BUS_BITLEVEL
#if defined(DUMP_TELEGRAMS) && (defined(DEBUG_BUS) || defined(DEBUG_BUS_BITLEVEL))
#   warning "DUMP_TELEGRAMS should not be used in parallel with DEBUG_BUS or DEBUG_BUS_BITLEVEL"
#endif

// list here all defines which need the serial port
#if defined(DEBUG_BUS) || defined(DEBUG_BUS_BITLEVEL) || defined(DUMP_TELEGRAMS) || defined(DUMP_COM_OBJ) || \
    defined(DUMP_MEM_OPS) || defined(DUMP_PROPERTIES) || defined(DUMP_TL4)
#   define INCLUDE_SERIAL
#endif

#if defined(INCLUDE_SERIAL) && !defined(SERIAL_SPEED)
#   warning "Default debugging serial port speed set to 115200"
#   define SERIAL_SPEED 115200
#endif

#endif /* SBLIB_LIBCONFIG_H_ */

/** @}*/
