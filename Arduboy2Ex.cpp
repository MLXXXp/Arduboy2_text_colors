/*
Copyright (c) 2020, Scott Allen
All rights reserved.

The write() and drawChar() functions are based on those in the
Arduboy2 library version 6.0.0

Arduboy2 library:
Copyright (c) 2016-2020, Scott Allen
All rights reserved.

The Arduboy2 library was forked from the Arduboy library:
https://github.com/Arduboy/Arduboy
Copyright (c) 2016, Kevin "Arduboy" Bates
Copyright (c) 2016, Chris Martinez
Copyright (c) 2016, Josh Goebel
Copyright (c) 2016, Scott Allen
All rights reserved.
which is in turn partially based on the Adafruit_SSD1306 library
https://github.com/adafruit/Adafruit_SSD1306
Copyright (c) 2012, Adafruit Industries
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holders nor the
names of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Arduboy2Ex.h"

size_t Arduboy2Ex::write(uint8_t c)
{
  if ((c == '\r') && !textRaw)
  {
    return 1;
  }

  if (((c == '\n') && !textRaw) ||
      (textWrap && (cursor_x > (WIDTH - (characterWidth * textSize)))))
  {
    cursor_x = 0;
    cursor_y += fullCharacterHeight * textSize;
  }

  if ((c != '\n') || textRaw)
  {
    drawChar(cursor_x, cursor_y, c, textColor, textBackground, textSize);
    cursor_x += fullCharacterWidth * textSize;
  }

  return 1;
}

void Arduboy2Ex::drawChar
  (int16_t x, int16_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size)
{
// It is assumed that rendering characters fully off screen will be rare,
// so let drawPixel() handle off screen checks, to reduce code size at the
// expense of slower off screen character handling.
#if 0
  if ((x >= WIDTH) ||              // Clip right
      (y >= HEIGHT) ||             // Clip bottom
      ((x + characterWidth * size - 1) < 0) ||  // Clip left
      ((y + characterHeight * size - 1) < 0)    // Clip top
     )
  {
    return;
  }
#endif

  const uint8_t* bitmap =
    &font5x7[c * characterWidth * ((characterHeight + 8 - 1) / 8)];

  for (uint8_t i = 0; i < fullCharacterWidth; i++)
  {
    uint8_t column;

    if (characterHeight <= 8)
    {
      column = (i < characterWidth) ? pgm_read_byte(bitmap++) : 0;
    }
    else
    {
      column = 0;
    }

    // draw the character by columns. Top to bottom, left to right
    // including character spacing on the right
    for (uint8_t j = 0; j < characterHeight; j++)
    {
      if (characterHeight > 8)
      {
        // at this point variable "column" will be 0, either from initialization
        // or by having eight 0 bits shifted in by the >>= operation below
        if ((j % 8 == 0) && (i < characterWidth))
        {
          column = pgm_read_byte(bitmap++);
        }
      }

      uint8_t pixelColor = column & 0x01 ? color : bg;

      if (pixelColor != TRANSPARENT)
      {
        for (uint8_t a = 0; a < size; a++)
        {
          for (uint8_t b = 0; b < size; b++)
          {
            drawTextPixel(x + (i * size) + a, y + (j * size) + b, pixelColor);
          }
        }
      }
      column >>= 1;
    }

    // draw the inter-line spacing pixels for this column if required
    if (bg != TRANSPARENT)
    {
      for (uint8_t j = characterHeight; j < fullCharacterHeight; j++)
      {
        for (uint8_t a = 0; a < size; a++)
        {
          for (uint8_t b = 0; b < size; b++)
          {
            drawTextPixel(x + (i * size) + a, y + (j * size) + b, bg);
          }
        }
      }
    }
  }
}

void Arduboy2Ex::drawTextPixel(int16_t x, int16_t y, uint8_t color)
{
  if (color == INVERT)
  {
    color = !getPixel(x, y);
  }

  drawPixel(x, y, color);
}

