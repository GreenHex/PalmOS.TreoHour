/*
 * BigText.h
 */

#ifndef __FONTS_H__
#define __FONTS_H__

#include <Hs.h>
#include <Font.h>

#include "AppResources.h"

#define HUGE_NUM_FONT				1234 // unused
#define HUGE_NUM_EXT_FONT			5678

// Should not be used by any other application!!!
#define CUSTOM_FONT_ID				fntAppFontCustomBase + 18 // random number

// Prototypes
extern UInt16 						DrawLargeTime( FontID fID, Char* strTime, RectangleType* rectP );

#endif /* __FONTS_H__ */
 
/*
 * BigText.h
 */