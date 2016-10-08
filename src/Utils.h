/*
 * Utils.h
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <Hs.h>
#include <HsPhone.h>
#include <palmOneResources.h>
#include <SystemMgr.h>
#include <Form.h>
#include <List.h>
#include <TonesLibTypes.h>
#include <TonesLib.h>
#include <SoundMgr.h>
#include <PmKeyLibCommon.h>
#include <PmKeyLib.h>
#include <SelTime.h>

#include "Prefs.h"

#define STRLEN_TIME			10
#define STRLEN_OPERATOR		MAX_SHORTOPERATORNAME_LENGTH + 1 // 16 + 1

#define MIN_VERSION  		sysMakeROMVersion( 5, 0, 0, sysROMStageRelease, 0 )
#define LAUNCH_FLAGS 		( sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp )

// Prototypes
extern Boolean 				EvtCharEqualsLaunchChar( WChar evt_wChar, UInt16 evt_keyCode, UInt16 evt_modifier, UInt16 keyCharIdx, UInt16 keyModifierIdx );
extern Boolean 				is_display_off( void );
extern Boolean 				OpenKeyLib( UInt16* pmKeyLibRefNumP );
extern void 				CloseKeyLib( UInt16 pmKeyLibRefNum, Boolean pmKeyLibLoaded );
extern UInt16 				GetListIdx( FormType* pForm, UInt16 lstID );
extern Boolean 				SetOneTime( FormType* pForm, UInt16 ctlID, TimeType* timeP, Char* titleStr );
extern Int16 				DateCompare( DateType d1, DateType d2 );
extern Int16 				TimeCompare( TimeType t1, TimeType t2 );
extern ToneVolume 			GetPhoneVolume( Boolean* pSoundOnVibrate, Boolean* pSoundOffVibrate );
extern Boolean 				IsRoaming( void );
extern Err 					GetCurrentOperator( Char* CurrentOperatorNameP, UInt16 strLenCurrentOperator );
extern UInt8 				GetBatteryState( Boolean* bPluggedinP );
extern Err 					RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags );
extern Char* 				ReplaceChar( unsigned char *s, char orgChr, char newChr );
extern Char* 				StripSpaces( unsigned char *s );

#endif /* __UTILS_H__ */

/*
 * Utils.h
 */