# Arduboy2_text_colors

Provides the _Arduboy2Ex_ class, which inherits the _Arduboy2_ class of the
[Arduboy2 library](https://github.com/MLXXXp/Arduboy2).

It overrides the _write()_ and _drawChar()_ functions to add "inverted" color
support and more control of transparency.

- Color value INVERT can be used for both text and background.
- New color value TRANSPARENT can be used for both text and background in place
of setting text and background the same to get a transparent background.

## Usage

Copy files _Arduboy2Ex.h_ and _Arduboy2Ex.cpp_ to the folder containing
the sketch.

Use the local _Arduboy2Ex_ class in place of _Arduboy2_

```cpp
//#include <Arduboy2.h>
#include "Arduboy2Ex.h"

//Arduboy2 arduboy;
Arduboy2Ex arduboy;
```

The _setTextColor()_ and _setTextBackground()_ functions can now use the
following values:

    WHITE
    BLACK
    INVERT
    TRANSPARENT

`INVERT` will invert the pixels in the screen buffer that coincide with the
text or background pixels.

`TRANSPARENT` will leave unchanged the pixels in the screen buffer that coincide
with the text or background pixels.

Note that setting the text and background colors to the same value will work
as expected but won't be of much use.

