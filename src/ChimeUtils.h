/*
 * ChimeUtils.h
 */

#ifndef __CHIMEUTILS_H__
#define __CHIMEUTILS_H__

#include <Hs.h>
#include <palmOneResources.h>
#include <SystemMgr.h>
#include <TonesLibTypes.h>
#include <TonesLib.h>
#include <SoundMgr.h>

#include "Global.h"
#include "Prefs.h"
#include "Sounds.h"

#define PAUSE_FREQUENCY			1 // 0 doesn't play any sound // we don't use this anyway
#define PAUSE_DURATION			40 // 750

typedef enum {
	Invalid						= 0,
	QuarterHour 				= 1,
	HalfHour 					= 2,
	ThreeQuarterHour 			= 3,
	OneHour 					= 4
} chimeInterval_e;

// Prototypes
extern void 				PlayChime( UInt32 timeSeconds, sounds_t* pSnd, Boolean blocking );
extern void 				LEDState( Boolean set, UInt16* countP, UInt16* stateP );
extern void 				FlashLED( Boolean blink, UInt8 BlinkType );
extern void 				Vibrate( void );

#endif /* __CHIMEUTILS_H__ */
 
/*
 * ChimeUtils.h
 */