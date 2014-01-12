/*
 *  bits.h - Bit and byte manipulation functions.
 *
 *  Copyright (c) 2014 Stefan Taferner <stefan.taferner@gmx.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3 as
 *  published by the Free Software Foundation.
 */
#ifndef sblib_bits_h
#define sblib_bits_h

#include <sblib/types.h>

/**
 * Compute the value of the specified bit.
 *
 * @param bitno - the number of the bit (0, 1, 2... 31)
 */
#define bit(bitno) (1UL << (bitno))

/**
 * Clear the bit of a number. The number can be
 * any integer (byte, short, int, long).
 *
 * @param val - the number from which to clear the bit
 * @param bitno - the number of the bit (0, 1, 2... 31)
 */
#define bitClear(val, bitno) ((val) &= ~(1UL << (bitno)))

/**
 * Set the bit of a number. The number can be
 * any integer (byte, short, int, long).
 *
 * @param val - the number from which to set the bit
 * @param bitno - the number of the bit (0, 1, 2... 31)
 */
#define bitSet(val, bitno) ((val) |= 1UL << (bitno))

/**
 * Write the value of a bit of a number.
 *
 * @param val - the number from which to write the bit
 * @param bitno - the number of the bit (0, 1, 2... 31)
 * @param b - the bit value (0 or 1)
 */
#define bitWrite(val, bitno, b) ((b) ? bitSet(val, bitNo) : bitClear(val, bitNo))

/**
 * Read the value of a bit of a number. The number can be
 * any integer (byte, short, int, long).
 *
 * @param val - the number from which to get the bit
 * @param bitno - the number of the bit (0, 1, 2... 31)
 * @return The value of the bit (0 or 1).
 */
#define bitRead(val, bitno) (((val) >> (bitno)) & 1)

/**
 * Extract the lowest (rightmost) byte of a number. The number can be
 * any integer (byte, short, int, long).
 *
 * @param val - the value to extract the lowest byte.
 * @return The extracted byte (0..255)
 */
#define lowByte(val) ((val) & 255)

/**
 * Extract the highest (leftmost) byte of a number. The number can be
 * any integer (byte, short, int, long).
 *
 * @param val - the value to extract the highest byte.
 * @return The extracted byte (0..255)
 */
#define highByte(val) (((val) >> ((sizeof(val) - 1) << 3)) & 255)


#endif /*sblib_bits_h*/
