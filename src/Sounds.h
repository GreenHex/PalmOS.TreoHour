/*
 * Sounds.h
 */

#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <Hs.h>
#include <TonesLibTypes.h>
#include <TonesLib.h>
#include <HsExtCommon.h>

#include "Global.h"
#include "Prefs.h"

// Prototypes
extern Err 			LoadTonesList( ListType* pList );
extern Err 			GetToneID( UInt16 idx, ToneIdentifier* pToneID );
extern void 		PlayTone( ToneIdentifier ToneID, UInt16 volume, Boolean blocking );
extern void 		StopTone( UInt16 TonesLibRefNum );
extern ToneVolume	GetSoundVolume( void );

#endif /* __SOUNDS_H__ */
 
/*
 * Sounds.h
 */