/*
 * FormAppPrefs.c
 */
  
#include "FormAppPrefs.h"

// Prototypes
static void 			AppPrefsForm_Initialize( FormType* pForm );
static void 			AppPrefsForm_SavePrefs( FormType* pForm );

/*
 * AppPrefsForm_HandleEvent()
 */
Boolean AppPrefsForm_HandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
				
			AppPrefsForm_Initialize( pForm );
			
			FrmDrawForm( pForm );
			
			handled = true;
						
			break;
			
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case APPPREFS_BTN_DOME:
		
					AppPrefsForm_SavePrefs( pForm );
								
					FrmReturnToForm( 0 );
					
					handled = true;

					break;

				case APPPREFS_BTN_CANCEL:
				
					FrmReturnToForm( 0 );
			
					handled = true;
					
					break;
						
				default:
					break;
			}
			
			break;
			
		default:
								
			break;
	}
	
	return ( handled );
	
} // AppPrefsForm_HandleEvent()

/*
 * AppPrefsForm_Initialize()
 */
static void AppPrefsForm_Initialize( FormType* pForm )
{
	appPrefs_t		prefs;
	
	if ( !pForm ) return;

	ReadAppPrefs( &prefs );

	SetCtlBoolValue( pForm, APPPREFS_CBX_ALLOW_APP_LAUNCH, prefs.bEnableAppLaunchFromClock );
	SetCtlBoolValue( pForm, APPPREFS_CBX_DONT_DISP_KG_TXT, prefs.bDoNotDisplayKeyguardText );
	
	return;
	
} // AppPrefsForm_Initialize()

/*
 * AppPrefsForm_SavePrefs()
 */
static void AppPrefsForm_SavePrefs( FormType* pForm )
{
	appPrefs_t		prefs;
	
	if ( !pForm ) return;
	
	ReadAppPrefs( &prefs );
	
	prefs.bEnableAppLaunchFromClock = GetCtlBoolValue( pForm, APPPREFS_CBX_ALLOW_APP_LAUNCH );
	prefs.bDoNotDisplayKeyguardText = GetCtlBoolValue( pForm, APPPREFS_CBX_DONT_DISP_KG_TXT );
	
	WriteAppPrefs( &prefs );

	return;	

} // AppPrefsForm_SavePrefs()

/*
 * FormAppPrefs.c
 */