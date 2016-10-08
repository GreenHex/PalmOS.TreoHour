/*
 * Prefs.h
 */
 
#ifndef __PREFS_H__
#define __PREFS_H__

#include <Hs.h>
#include <Preferences.h>
#include <TonesLibTypes.h>
#include <TonesLib.h>

#define STRLEN_KEYNAMES			16

#include "Global.h"

typedef enum {
	prefUnsaved					= false,
	prefSaved					= true
} prefType_e;

#define appPrefCreator					appFileCreator
#define appPrefID						0x00
#define appPrefVersionNum				0x02
#define appPrefType						prefSaved						
//
#define usPrefCreator					appFileCreator
#define usPrefID						0x00
#define usPrefVersionNum				0x01
#define usPrefType						prefUnsaved
//
#define newMsgCountPrefCreator			appFileCreator
#define newMsgCountPrefID				0x01
#define newMsgCountPrefVersionNum		0x01
#define newMsgCountPrefType				prefUnsaved
//
#define gdtTxtPrefCreator				appFileCreator
#define gdtTxtPrefID					0x02
#define gdtTxtPrefVersionNum			0x01
#define gdtTxtPrefType					prefUnsaved
//
#define modifierPrefCreator				appFileCreator
#define modifierPrefID					0x03
#define modifierPrefVersionNum			0x01
#define modifierPrefType				prefUnsaved

// Saved Prefs
typedef struct {
	RGBColorType 				background;
	RGBColorType 				foreground;
	RGBColorType 				timeText;
	RGBColorType 				noAlertText;
	RGBColorType				alertText;
} colors_t;

typedef struct {
	ToneVolume					toneVolume;
	UInt16						toneIdxHour;
	UInt16						toneIdxThreeQuarters;
	UInt16						toneIdxHalfHouly;
	UInt16						toneIdxQuarterly;
} sounds_t;

typedef struct {
	UInt16						chimeIntervalIdx;
	TimeType					startTime;
	TimeType					endTime;
	UInt16						timeoutIdx;
	Boolean						bAllowKill;
	Boolean						flashLEDIdx;
	Boolean						bVibrate;
	FontID						font;
} chimePrefs_t;

typedef struct {
	Boolean						bEnabled;
	UInt8						timeoutIdx;
	Boolean						bKeyPassthrough;
	UInt8						popupkeyIdx;
	UInt8						popupModifierKeyIdx;
} popupPrefs_t;

typedef struct {
	Boolean						bEnabled;
	UInt8						timeoutIdx;
	Boolean						bNoKeyguardOnAttnAlert;
	UInt8						unlockKeyIdx;
	UInt8						unlockModifierKeyIdx;
	Char						unlockKeyName[STRLEN_KEYNAMES];
	Char						unlockModifierKeyName[STRLEN_KEYNAMES];
} keyguardPrefs_t;

typedef struct {
	Boolean						bEnabled;
	chimePrefs_t				chime;
	popupPrefs_t				popup;
	keyguardPrefs_t				keyguard;
	sounds_t					sounds;
	colors_t					colors;
	Boolean						bEnableAppLaunchFromClock;
	Boolean						bDoNotDisplayKeyguardText;
} appPrefs_t;

// Unsaved Prefs

typedef enum {
	stateNoshow			= -1,
	stateSleep			= 0,
	stateChime			= 1,
	statePopup			= 2,
	stateKeyguard		= 3
} clockState_e;

typedef struct {
	UInt32						clockTime;
	Boolean 					bSleepAfterChime;
	clockState_e				clockState;
	Boolean						bNoKeyguardOnAttnAlert;
} usPrefs_t;

typedef struct {
	UInt16						prevModifier;
} prevModPref_t;

// Prototypes
extern void 					WriteAppPrefs( appPrefs_t* pPrefs );
extern void 					ReadAppPrefs( appPrefs_t* pPrefs );
extern void						WriteUsPrefs( usPrefs_t* pUsPrefs );
extern void						ReadUsPrefs( usPrefs_t* pUsPrefs );
extern void 					InitializeUsPrefs( usPrefs_t* pUsPrefs );
extern void 					WriteModifierCharPrefs( prevModPref_t* pModPrefs );
extern void 					ReadModifierCharPrefs( prevModPref_t* pModPrefs );
extern void 					InitializeModifierCharPrefs( prevModPref_t* pModPrefs );

#endif /* __PREFS_H__ */
 
/*
 * Prefs.h
 */