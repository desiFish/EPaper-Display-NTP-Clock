/**
 ******************************************************************************
 * @file    fonts.h
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-February-2014
 * @brief   Header for fonts.c file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

/* Max size of bitmap based on Font48 (48x48) */
#define MAX_HEIGHT_FONT 48
#define MAX_WIDTH_FONT 24
#define OFFSET_BITMAP 54

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/**
 * Font structure containing bitmap data and dimensions
 * @table: Pointer to font bitmap data
 * @Width: Width of each character in pixels
 * @Height: Height of each character in pixels
 */
struct sFONT
{
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
};

/**
 * Font definitions for the E-Paper Display Clock
 * Font48: Large font for weekday and time display
 * Font24: Medium font for temperature symbol
 * Font12: Small font for battery percentage
 * Font8: Tiny font for special symbols
 */
extern sFONT Font48; // Primary display font (weekday, time, date)
extern sFONT Font24; // Temperature degree symbol
extern sFONT Font12; // Battery status information
extern sFONT Font8;  // Special indicators and symbols

#endif /* __FONTS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
