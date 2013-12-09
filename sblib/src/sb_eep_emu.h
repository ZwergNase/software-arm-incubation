/*
 *  Copyright (c) 2013 Martin Glück <martin@mangari.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

/**
 * @file  sb_eep_emu.h
 * @brief EEPROM emulation
 *
 * This module contains a simple EEPROM emulation with support for multiple independent
 * regions.
 *
 * The last 32bit of the FLAsh page are used book-keeping. The first 24bits are a write
 * counter. Every time this EEPROM region will be written this counter will be increased.
 * If the whole sector will be erased outside of this module, the counter information is
 * lost.
 * In the last 8bit a version identification is stored. If this ID does not match the ID
 * specified for this region during construction, the content of the FLASH will not be
 * copied into the RAM mirror.
 *
 * Pre-Requests:
 *   At least one sector of the FLASH per region must be reserved in the linker command file
 *   (this should be done via the Project Settings -> C/C++ Build ->MCU settings dialog since
 *   the linker files will be regenerated by the IDE at some point. Or you can change the
 *   files manually and then disable the "Manage linker script" option in the
 *   Project Settings -> C/C++ Build -> Settings -> MCU Linker -> Target and select a hand
 *   modified linker script):
 *
 *   - reduce the size of the MFlash32 region
 *   - add a new region called EEPROM (the name is important)
 *
 *   If you want to prevent that the EEPROM data will be downloaded as part when
 *   downloading the axf file, the generated linker scripts need to be modified (and therefore
 *   the Managed linker script needs to be disables):
 *   - add the (NOLOAD) option to the section handling the EEPROM region, e.g.:
 *         .text_Flash2 (NOLOAD) : ALIGN(4)
 *
 *   In the sblib/linker-scripts folder you can find a example linker script for the LPC11xx
 *   platform with 32kB total FLASH memory and the correct settings.
 *
 * Known limitations:
 * - The size of one EEPROM page must be a multiple of 256 - 4
 *   (4 bytes are required for management and internal information=
 * - The whole area which is used by a region must cover whole sectors
 *   (size + 4) * no of pages == n * Sector size.
 *
 *   It is possible to use more than one page per region
 *
 */
#ifndef SB_EEP_EMU_H_
#define SB_EEP_EMU_H_

/**
 * Define a EEPROM emulation region with the specified size.
 * @param name      name of the EEPROM region
 * @param size      the size of ONE page (size + 4 must be a multiple of 256)
 * @param copies    how many pages should be allocated of this region
 * @param version   an unique ID use to identify the layout version
 *                  of the data section. If the store layout version
 *                  does not match the current version, the data will
 *                  not be copied from the FLASH to the RAM mirror
 */
#define SB_DEFINE_REGION(name,size,copies,version)                        \
  typedef int name##_size_check[((size + 4) % 256) == 0 ? 1 : -1];        \
        unsigned char  __attribute__ ((aligned (4))) name##_m [size + 4]; \
  __attribute__ ((section(".text_EEPROM")))                               \
  const unsigned char name##_f [(size + 4) * copies];                     \
        SB_Epp_Region name = {name##_m, name##_f, 4 + size, copies, version};

/**
 * The organizational data for an emulated EEPROM region
 */
typedef struct {
    unsigned char        * ram;         //!< the base address of the RAM mirror
    const unsigned char  * rom_base;    //!< the base address of the pages in the FLASH
    unsigned short         size;        //!< the size of one page
    unsigned char          rom_pages;   //!< how many pages should be stored in the FLASH
    unsigned char          version;     //!< a layout version ID
    unsigned char          state;       //!< internal state
} SB_Epp_Region;

#define SB_EEP_NO_VALID_PAGE_FOUND   -1
#define SB_EEP_VERSION_MISSMATCH     -2

/**
 * Initializes the EEPROM region.
 *
 * If a valid page can be found inside the FLASH the data will be copied into
 * the RAM mirror.
 *
 * @param region        handle to the EEPROM region
 * @param clear         specifies if the all sectors belonging to this EEPROM region
 *                      should be erased.
 * @return              Returns the status:
 *                      - 0                          if a valid ROM page has been copied
 *                                                   to the RAM mirror
 *                      - SB_EEP_NO_VALID_PAGE_FOUND if no valid FLASH page could be found
 *                      - SB_EEP_VERSION_MISSMATCH   if the version ID did not match
 */
int sb_eep_init   (SB_Epp_Region * region, unsigned int  clear);

/**
 * Write the current content of the RAM mirror of the EEPROM region into
 * a new page insinde the FLASH.
 * @param region        handle to the EEPROM reagion
 * @return              status of the operation (0 == OK)
 */
int sb_eep_update (SB_Epp_Region * region);

#endif /* SB_EEP_EMU_H_ */