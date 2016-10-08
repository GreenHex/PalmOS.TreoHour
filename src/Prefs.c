/*
 * Prefs.c
 */

#include "Prefs.h"

// Prototypes
static void 			InitializeAppPrefs( appPrefs_t* pPrefs );
static void 			InitializeChimePrefs( chimePrefs_t* pChime );
static void 			InitializePopupPrefs( popupPrefs_t* pPopup );
static void 			InitializeKeyguardPrefs( keyguardPrefs_t* pKG );
static void 			InitializeSoundPrefs( sounds_t* pSnd );
static void 			InitializeColorPrefs( colors_t* pColors );

/*
 * WriteAppPrefs()
 */
void WriteAppPrefs( appPrefs_t* pPrefs )
{
	if ( !pPrefs ) return;
	
	PrefSetAppPreferences( appPrefCreator, appPrefID, appPrefVersionNum, pPrefs, sizeof( appPrefs_t ), appPrefType );
	
	return;
	
} // WriteAppPrefs()

/*
 * ReadAppPrefs()
 */
void ReadAppPrefs( appPrefs_t* pPrefs )
{
	UInt16 			prefSize = sizeof( appPrefs_t );
	UInt16			prefsVersion = 0;

	if ( !pPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( appPrefCreator, appPrefID, pPrefs, &prefSize, appPrefType );
	
	if ( prefsVersion != appPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( appPrefCreator, appPrefID, prefsVersion, NULL, 0, appPrefType );				
	
		InitializeAppPrefs( pPrefs );
		
		WriteAppPrefs( pPrefs );
	}
	
	return;
	
} // ReadAppPrefs()

/*
 * InitializeAppPrefs()
 */
static void InitializeAppPrefs( appPrefs_t* pPrefs )
{
	if ( !pPrefs ) return;
	
	MemSet( pPrefs, sizeof( appPrefs_t ), 0 );
	
	pPrefs->bEnabled = true;
	pPrefs->bEnableAppLaunchFromClock = true;
	
	InitializeChimePrefs( &(pPrefs->chime) );
	InitializePopupPrefs( &(pPrefs->popup) );
	InitializeKeyguardPrefs( &(pPrefs->keyguard) );
	InitializeSoundPrefs( &(pPrefs->sounds) );
	InitializeColorPrefs( &(pPrefs->colors) );
	
	return;
	
} // InitializeAppPrefs

/*
 * InitializeChimePrefs()
 */
static void InitializeChimePrefs( chimePrefs_t* pChime )
{
	if ( !pChime ) return;
	
	pChime->chimeIntervalIdx = 1;
	pChime->startTime.hours = 0;
	pChime->startTime.minutes = 0;
	pChime->endTime.hours = 23;
	pChime->endTime.minutes = 59;
	pChime->timeoutIdx = 3;	
	pChime->bAllowKill = true;
	pChime->flashLEDIdx = 2;
	pChime->bVibrate = false;
	pChime->font = stdFont;

	return;	
	
} // InitializeChimePrefs()

/*
 * InitializePopupPrefs()
 */
static void InitializePopupPrefs( popupPrefs_t* pPopup )
{
	if ( !pPopup ) return;
	
	pPopup->bEnabled = false;
	pPopup->timeoutIdx = 0;
	pPopup->bKeyPassthrough = false;
	pPopup->popupkeyIdx = 0;
	pPopup->popupModifierKeyIdx = 0;
		
	return;
	
} // InitializePopupPrefs()

/*
 * InitializeKeyguardPrefs()
 */
static void InitializeKeyguardPrefs( keyguardPrefs_t* pKG )
{
	if ( !pKG ) return;
	
	pKG->bEnabled = false;
	pKG->timeoutIdx = 0;
	pKG->bNoKeyguardOnAttnAlert = false; // "true" to NOT lock
	pKG->unlockKeyIdx = 0;
	pKG->unlockModifierKeyIdx = 0;
	pKG->unlockKeyName[STRLEN_KEYNAMES] = chrNull;
	pKG->unlockModifierKeyName[STRLEN_KEYNAMES] = chrNull;

	return;
	
} // InitializeKeyguardPrefs()

/*
 * InitializeSoundPrefs()
 */
static void InitializeSoundPrefs( sounds_t* pSnd )
{	
	if ( !pSnd ) return;
	
	pSnd->toneVolume = toneVolume4;
	pSnd->toneIdxHour = 0;
	pSnd->toneIdxThreeQuarters = 0;
	pSnd->toneIdxHalfHouly = 0;
	pSnd->toneIdxQuarterly = 0;
		
	return;
	
} // InitializeSoundPrefs()

/*
 * InitializeColorPrefs()
 */
static void InitializeColorPrefs( colors_t* pColors )
{
	if ( !pColors ) return;

	pColors->background.r = 255 ; pColors->background.g = 255 ; pColors->background.b = 255 ; // white
	pColors->foreground.r = 0 ; pColors->foreground.g = 0 ; pColors->foreground.b = 0 ; // // black
	pColors->timeText.r = 0 ; pColors->timeText.g = 51 ; pColors->timeText.b = 204 ; // blue
	pColors->noAlertText.r = 200 ; pColors->noAlertText.g = 200 ; pColors->noAlertText.b = 200 ; // grey
	pColors->alertText.r = 255 ; pColors->alertText.g = 51 ; pColors->alertText.b = 51 ; // red
	
	return;
	
} // InitializeColorPrefs()

/*
 * WriteUsPrefs()
 */
void WriteUsPrefs( usPrefs_t* pUsPrefs )
{
	if ( !pUsPrefs ) return;
	
	PrefSetAppPreferences( usPrefCreator, usPrefID, usPrefVersionNum, pUsPrefs, sizeof( usPrefs_t ), usPrefType );
	
	return;
	
} // WriteUsPrefs()

/*
 * ReadUsPrefs()
 */
void ReadUsPrefs( usPrefs_t* pUsPrefs )
{
	UInt16 			prefSize = sizeof( usPrefs_t );
	UInt16			prefsVersion = 0;

	if ( !pUsPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( usPrefCreator, usPrefID, pUsPrefs, &prefSize, usPrefType );
	
	if ( prefsVersion != usPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( usPrefCreator, usPrefID, prefsVersion, NULL, 0, usPrefType );				
	
		InitializeUsPrefs( pUsPrefs );
		
		WriteUsPrefs( pUsPrefs );
	}
	
	return;
	
} // ReadUsPrefs()

/*
 * InitializeUsPrefs()
 */
void InitializeUsPrefs( usPrefs_t* pUsPrefs )
{
	if ( !pUsPrefs ) return;
	
	MemSet( pUsPrefs, sizeof( usPrefs_t ), 0 );
	
	return;

} // InitializeUsPrefs()

/*
 * WriteModifierCharPrefs()
 */
void WriteModifierCharPrefs( prevModPref_t* pModPrefs )
{
	if ( !pModPrefs ) return;
	
	PrefSetAppPreferences( modifierPrefCreator, modifierPrefID, modifierPrefVersionNum, pModPrefs, sizeof( prevModPref_t ), modifierPrefType );
	
	return;
	
} // WriteModifierCharPrefs()

/*
 * ReadModifierCharPrefs()
 */
void ReadModifierCharPrefs( prevModPref_t* pModPrefs )
{
	UInt16 			prefSize = sizeof( prevModPref_t );
	UInt16			prefsVersion = 0;

	if ( !pModPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( modifierPrefCreator, modifierPrefID, pModPrefs, &prefSize, modifierPrefType );
	
	if ( prefsVersion != modifierPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( modifierPrefCreator, modifierPrefID, prefsVersion, NULL, 0, modifierPrefType );				
	
		InitializeModifierCharPrefs( pModPrefs );
		
		WriteModifierCharPrefs( pModPrefs );
	}
	
	return;
	
} // ReadModifierCharPrefs()

/*
 * InitializeModifierCharPrefs()
 */
void InitializeModifierCharPrefs( prevModPref_t* pModPrefs )
{
	if ( !pModPrefs ) return;
	
	MemSet( pModPrefs, sizeof( prevModPref_t ), 0 );
	
	return;

} // InitializeModifierCharPrefs()

/*
 * Prefs.c
 */