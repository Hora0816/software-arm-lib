/**************************************************************************//**
 * @addtogroup SBLIB_BOOTLOADER Selfbus Bootloader
 * @addtogroup SBLIB_BOOT_BLOCK_DESCRIPTOR Application Boot Block Descriptor
 * @ingroup SBLIB_BOOTLOADER
 *
 * @{
 *
 * @file   boot_descriptor_block.cpp
 * @author Martin Glueck <martin@mangari.org> Copyright (c) 2015
 * @author Stefan Haller Copyright (c) 2020
 * @author Darthyson <darth@maptrack.de> Copyright (c) 2021
 * @bug No known bugs.
 ******************************************************************************/

/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 3 as
 published by the Free Software Foundation.
 -----------------------------------------------------------------------------*/

// #include <sblib/internal/iap.h>
#include "boot_descriptor_block.h"
#include "crc.h"


extern unsigned int __base_Flash;   //!< marks the beginning of the flash memory (inserted by the linker)
                                    //!< used to protect the updater from killing itself with a new application downloaded over the bus
extern unsigned int  __top_Flash;   //!< marks the end of the flash memory (inserted by the linker)
                                    //!< used to protect the updater from killing itself with a new application downloaded over the bus
extern unsigned int _image_start;   //!< marks the beginning of the bootloader firmware (inserted by the linker)
                                    //!< used to protect the updater from killing itself with a new application downloaded over the bus
extern unsigned int _image_end;     //!< marks the end of the bootloader firmware (inserted by the linker)
                                    //!< used to protect the updater from killing itself with a new application downloaded over the bus
extern unsigned int _image_size;    //!< marks the size of the bootloader firmware (inserted by the linker)
                                    //!< used to protect the updater from killing itself with a new application downloaded over the bus

__attribute__((unused)) unsigned char bl_id_string[BL_ID_STRING_LENGTH] = BL_ID_STRING; // actually it's used in getAppVersion,
                                                                                               // this is just to suppress compiler warning


/**
 * @brief This is the sanity check as described in 26.3.3 of the UM10398 user guide
 *
 * @note However, this shouldn't matter since we boot into the BL anyways and just it's vector table
 *       needs to be correct.
 *       This test get's important if we like to bypass the KNX bootloader.
 *       Note that the LPC device as a build in BL ROM for UART ISP which is always started first.
 *       This ROM BL checks if the vector table is correct to determine if the KNX BL can be started.
 *
 * @param start
 * @return
 */
inline unsigned int checkVectorTable(unsigned int start)
{
    unsigned int i;
    unsigned int * address;
    unsigned int cs = 0;
    address = (unsigned int *) start;	// Vector table start always at base address, each entry is 4 byte

    for (i = 0; i < 7; i++)				// Checksum is 2's complement of entries 0 through 6
        cs += address[i];
    //if (address[7])

    return (~cs+1);
    return (address[0]);
}

unsigned int checkApplication(AppDescriptionBlock * block)
{
    // if ((block->startAddress < APPLICATION_FIRST_SECTOR) || (block->startAddress > flashLastAddress())) // we have just 64k of Flash
    if ((block->startAddress < bootLoaderLastAddress()) || (block->startAddress > flashLastAddress())) // we have just 64k of Flash
    {
        return (0);
    }
    if (block->endAddress > flashLastAddress())	// we have just 64k of Flash
    {
        return (0);
    }
    if (block->startAddress >= block->endAddress)
    {
        return (0);
    }

    unsigned int blockSize = block->endAddress - block->startAddress;
    unsigned int crc = crc32(0xFFFFFFFF, (unsigned char *) block->startAddress, blockSize);

    if (crc == block->crc)
    {
        return (1);
        // see note from checkVectorTable
        // return checkVectorTable(block->startAddress);
    }
    return (0);
}

unsigned char* getAppVersion(AppDescriptionBlock * block)
{
    if ((block->appVersionAddress > bootLoaderLastAddress()) &&
        (block->appVersionAddress < flashLastAddress() - sizeof(block->appVersionAddress)))
    {
        return ((unsigned char*) (block->appVersionAddress));
    }
    else
    {
        return (&bl_id_string[0]); // Bootloader ID if invalid (address out of range)
    }
}

/**
 * @brief Return start address of application
 *
 * @param block Application description block to get the start address from
 * @return      Start address of application in case of valid descriptor block,
 *              otherwise base address of firmware area, directly behind bootloader
 */
unsigned char * getFirmwareStartAddress(AppDescriptionBlock * block)
{
    unsigned int applicationFirstSector = APPLICATION_FIRST_SECTOR;
    if (checkApplication(block))
    {
    	return ((unsigned char *) (block->startAddress));
    }
    else
    {
    	return ((unsigned char *) applicationFirstSector);
    }
}

unsigned int bootLoaderFirstAddress(void)
{
    return ((unsigned int) (unsigned int*)&_image_start);
}

unsigned int bootLoaderLastAddress(void)
{
    //linker sets this not correctly, so we need the -1
    return ((unsigned int) (unsigned int*)&_image_end - 1);
}

unsigned int bootLoaderSize(void)
{
    // includes .text and .data
    return ((unsigned int) (unsigned int*)&_image_size);
}

unsigned int flashFirstAddress(void)
{
    return ((unsigned int) (unsigned int*)&__base_Flash);
}

unsigned int flashLastAddress(void)
{
    //linker sets this not correctly, so we need the -1
    return ((unsigned int) ((unsigned int*)&__top_Flash) - 1);
}

unsigned int flashSize(void)
{
    // add the -1 from flashLastAddress(void) back to size
    return (flashLastAddress() - flashFirstAddress() + 1);
}

/** @}*/