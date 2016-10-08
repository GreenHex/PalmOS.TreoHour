/*
 * Helpers.c
 */
 
#include "Helpers.h"

// Prototypes
static void 					WriteNewMsgPrefs( newMsgCount_t* pNewMsgPrefs );
static void 					SetInvalidCount( newMsgCount_t* pNewMsgPrefs );

/*
 * SendMsgCountNotifications()
 */
void SendMsgCountNotifications( Boolean resetMsgCounts )
{
	newMsgCount_t			newMsgCounts;
	
	if ( resetMsgCounts )
	{					
		SetInvalidCount( &newMsgCounts );
		WriteNewMsgPrefs( &newMsgCounts );
	}
	
	NewMsgHelperNotify();
	
	return;

} // SendMsgCountNotifications()


/*
 * HandleHelperNotifications()
 */
Boolean HandleHelperNotifications( PmBroadcastNewMsgCountNotifyParamType* notifyDetailsP )
{
	Boolean 				retVal = true;
	UInt32					helperServiceClass = (UInt32)(notifyDetailsP->helperServiceClass);
	UInt16					msgCount = (UInt16)(notifyDetailsP->msgCount);							
	newMsgCount_t			newMsgCounts;
	
	if ( !notifyDetailsP ) return ( false );
	
	ReadNewMsgPrefs( &newMsgCounts );

	switch ( helperServiceClass )
	{
		case kHelperServiceClassIDEMail:
		
			newMsgCounts.EMail = msgCount;
			
			break;
			
		case kHelperServiceClassIDFax:
	
			newMsgCounts.Fax = msgCount;
			
			break;
		
		case kHelperServiceClassIDSMS:
	
			newMsgCounts.SMS = msgCount;
			
			break;
			
		case kHelperServiceClassIDMMS:
	
			newMsgCounts.MMS = msgCount;
			
			break;
		
		case kHelperServiceClassIDVoiceDial:
	
			newMsgCounts.VoiceDial = msgCount;
			
			break;
			
		default:
		
			retVal = false;
		
			break;
	}
	
	WriteNewMsgPrefs( &newMsgCounts );
	
	return ( retVal );		
	
} //HandleHelperNotifications()

/*
 * NewMsgHelperNotify()
 */
Boolean NewMsgHelperNotify()
{
	Boolean									handled = false;
	SysNotifyParamType 						param;
	PmGetNewMsgCountNotifyParamType			msgCountNotify;
	
	param.notifyType = pmNotifyGetNewMsgCountFromHelper; 
   	param.broadcaster = appFileCreator;  
	param.notifyDetailsP = &msgCountNotify;
	
	param.handled = false; 
	msgCountNotify.helperServiceClass = kHelperServiceClassIDEMail;
	SysNotifyBroadcast( &param );
	handled |= param.handled;
	
	param.handled = false;
	msgCountNotify.helperServiceClass = kHelperServiceClassIDSMS;
	SysNotifyBroadcast( &param );
	handled |= param.handled;
	
	param.handled = false;
	msgCountNotify.helperServiceClass = kHelperServiceClassIDMMS;
	SysNotifyBroadcast( &param );
	handled |= param.handled;
	
    return ( handled );
    
} // NewMsgHelperNotify()

/*
 * WriteNewMsgPrefs
 */
static void WriteNewMsgPrefs( newMsgCount_t* pNewMsgPrefs )
{
	if ( !pNewMsgPrefs ) return;
	
	PrefSetAppPreferences( newMsgCountPrefCreator, newMsgCountPrefID, newMsgCountPrefVersionNum, pNewMsgPrefs, sizeof( newMsgCount_t ), newMsgCountPrefType );

	return;
	
} // WriteNewMsgPrefs

/*
 * readNewMsgPrefs
 */
void ReadNewMsgPrefs( newMsgCount_t* pNewMsgPrefs )
{	
	UInt16 			prefSize = sizeof( newMsgCount_t );
	UInt16			prefsVersion = 0;
	
	if ( !pNewMsgPrefs ) return;
	
	prefsVersion = PrefGetAppPreferences( newMsgCountPrefCreator, newMsgCountPrefID, pNewMsgPrefs, &prefSize, newMsgCountPrefType );
	
	if ( prefsVersion != newMsgCountPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( newMsgCountPrefCreator, newMsgCountPrefID, prefsVersion, NULL, 0, newMsgCountPrefType );				
	
		SetInvalidCount( pNewMsgPrefs );
		WriteNewMsgPrefs( pNewMsgPrefs );
	}
	
	return;
		
}// ReadNewMsgPrefs

/*
 * SetInvalidCount()
 */
static void SetInvalidCount( newMsgCount_t* pNewMsgPrefs )
{
	if ( !pNewMsgPrefs ) return;

	pNewMsgPrefs->EMail = COUNT_INVALID;
	pNewMsgPrefs->Fax = COUNT_INVALID;
	pNewMsgPrefs->SMS = COUNT_INVALID;
	pNewMsgPrefs->MMS = COUNT_INVALID;
	pNewMsgPrefs->VoiceDial = COUNT_INVALID;
	
	return;
	
} // SetInvalidCount

/*
 * Helpers.c
 */  