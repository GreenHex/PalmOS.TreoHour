/*
 * AppMain.c
 */

#include <Hs.h>

#include "Global.h"
#include "AppResources.h"
#include "Utils.h"
#include "Prefs.h"
#include "NotifyReg.h"
#include "Handlers.h"
#include "FormAbout.h"
#include "FormSounds.h"
#include "FormMain.h"
#include "FormAppPrefs.h"

// Prototypes
static Err 					AppStart( void );
static void 				AppStop( void );
static Boolean 				AppHandleEvent( EventType* pEvent );
static void 				AppEventLoop( void );
UInt32 						PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags );

/*
 * AppStart()
 */
static Err AppStart( void )
{
	gTonesH = NULL;
	
	FrmGotoForm( MAIN_FORM );
	
	return ( errNone );
	
} // AppStart()

/*
 * AppStop()
 */
static void AppStop( void )
{
	FrmCloseAllForms();	
	
	if ( gTonesH )
    {
    	MemHandleUnlock( gTonesH );
    	MemHandleFree( gTonesH );
    	gTonesH = NULL;
    }
    
	return;
	
} // AppStop()

/*
 * AppHandleEvent()
 */
static Boolean AppHandleEvent( EventType* pEvent )
{
	Boolean			handled = false;
	FormType* 		pForm = NULL;

	if ( pEvent->eType == frmLoadEvent )
	{
		pForm = FrmInitForm( pEvent->data.frmLoad.formID );
		
		if ( !pForm ) return ( handled );
		
		FrmSetActiveForm( pForm );
		
		switch ( pEvent->data.frmLoad.formID )
		{
			case MAIN_FORM:
				
				FrmSetEventHandler( pForm, MainForm_HandleEvent );
						
				handled = true;
				
				break;
				
			case SND_FORM:
				
				FrmSetEventHandler( pForm, SoundsForm_HandleEvent );
						
				handled = true;
				
				break;
				
			case APPPREFS_FORM:
				
				FrmSetEventHandler( pForm, AppPrefsForm_HandleEvent );
						
				handled = true;
				
				break;
					
			case ABOUT_FORM:
				
				FrmSetEventHandler( pForm, AboutForm_HandleEvent );
						
				handled = true;
				
				break;
			
			default:
				break;
		}
	}
	
	return ( handled );

} // AppHandleEvent()

/*
 * AppEventLoop()
 */
static void AppEventLoop( void )
{
	Err			error;
	EventType	event;

	do {
		
		EvtGetEvent( &event, evtWaitForever );

		if ( SysHandleEvent( &event ) )
			continue;
			
		if ( MenuHandleEvent( 0, &event, &error ) )
			continue;
			
		if ( AppHandleEvent( &event ) )
			continue;

		FrmDispatchEvent( &event );

	} while ( event.eType != appStopEvent );

} // AppEventLoop()

/*
 * PilotMain()
 */
UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
{
	Err 						error = errNone;
	
	switch ( cmd )
	{
		case sysAppLaunchCmdNotify:
						
			error = HandleNotification( (SysNotifyParamType *) cmdPBP );
			
			break;
			
		case sysAppLaunchCmdAlarmTriggered:
		
			((SysAlarmTriggeredParamType *) cmdPBP)->purgeAlarm = ( ((SysAlarmTriggeredParamType *)cmdPBP)->ref != 0 );
			
			HandleAlarmTriggered();
						
			break;
			
		case sysAppLaunchCmdDisplayAlarm:
		
			if ( ((SysDisplayAlarmParamType *)cmdPBP)->ref != 0 ) break;
			
			++( ((SysDisplayAlarmParamType *)cmdPBP)->ref );
			
			HandleAlarmDisplay();
			
			break;

		case sysAppLaunchCmdSystemReset:

			if ( ( error = RomVersionCompatible( MIN_VERSION, launchFlags ) ) == errNone )
			{
				HandleReset();
			}
				
			break;
				
		case sysAppLaunchCmdNormalLaunch:
	    case sysAppLaunchCmdPanelCalledFromApp:
	    case sysAppLaunchCmdReturnFromPanel:
			
			if ( ( error = RomVersionCompatible( MIN_VERSION, launchFlags ) ) )
			{
				FrmCustomAlert( ErrOKAlert, "Incompatible Device", "", "" );
				return ( error );
			} 
			
			if ( ( error = AppStart() ) == 0 )
			{
				AppEventLoop();
				AppStop();
			}
			
			break;
			
		default:
			break;
	}

	return ( error );
	
} // PilotMain()

/*
 * AppMain.c
 */
