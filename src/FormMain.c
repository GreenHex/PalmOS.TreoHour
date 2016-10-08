/*
 * FormMain.c
 */
 
#include "FormMain.h"

// Prototypes
static void 			MainForm_Initialize( FormType* pForm );
static Boolean 			MainForm_SavePrefs( FormType* pForm );
static Boolean 			MainForm_HandleMenu( UInt16 menuID );

/*
 * MainForm_HandleEvent()
 */
Boolean MainForm_HandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	appPrefs_t				prefs;
	EventType				newEvent;
	static btn_e			btn = MAIN_BTN_CHIME;
		
	switch ( pEvent->eType )
	{	
		case frmOpenEvent:
		
			MainForm_Initialize( pForm );
			
			FrmDrawForm( pForm );

			InitializeMainFormGadgets( pForm, &btn );
			UpdateGadgets( pForm, &btn );
		
			FrmNavObjectTakeFocus( pForm, MAIN_BTN_DONE );
								
			handled = true;
						
			break;
			
		case frmCloseEvent:
		
			MainForm_SavePrefs( pForm );
		
			handled = false;
			
			break;
			
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case MAIN_BTN_TEST:
				
					MainForm_SavePrefs( pForm );
					
					ShowClock( stateChime, TimGetSeconds() );
					
					handled = true;
					
					break;
					
				case MAIN_BTN_DONE:		
											
					newEvent.eType = appStopEvent;	
					EvtAddEventToQueue( &newEvent );
					
					handled = true;

					break;
					
				case MAIN_CBX_ENABLE:
					
					ReadAppPrefs( &prefs );
					
					prefs.bEnabled = GetCtlBoolValue( pForm, pEvent->data.ctlSelect.controlID );

					WriteAppPrefs( &prefs );
					
					handled = true;

					break;
					
				case MAIN_BTN_CHIME:
				case MAIN_BTN_POPUP:				
				case MAIN_BTN_KEYGUARD:

					btn = pEvent->data.ctlSelect.controlID;
					UpdateGadgets( pForm, &btn );
									
					handled = true;
					
					break;
				
				case CHM_BTN_CONFIG_SND:
					
					FrmPopupForm( SND_FORM );
					
					handled = true;
					
					break;
					
				case CHM_TRG_FROM:
					
					ReadAppPrefs( &prefs );
	
					if ( SetOneTime( pForm, pEvent->data.ctlSelect.controlID, (TimeType *) &(prefs.chime.startTime), "Chime Start Time" ) )
					{
						WriteAppPrefs( &prefs );
					}
								
					handled = false;
					
					break;
					
				case CHM_TRG_TO:
					
					ReadAppPrefs( &prefs );
	
					if ( SetOneTime( pForm, pEvent->data.ctlSelect.controlID, (TimeType *) &(prefs.chime.endTime), "Chime End Time" ) )
					{
						WriteAppPrefs( &prefs );
					}
								
					handled = false;
					
					break;
									
				default:
					break;
			}
					
		case menuEvent:
										
			handled = MainForm_HandleMenu( pEvent->data.menu.itemID );
			
			break;
			
		default:
								
			break;
	}
	
	return ( handled );
	
} // MainForm_HandleEvent()

/*
 * MainForm_Initialize()
 */
static void MainForm_Initialize( FormType* pForm )
{	
	ListType* 				pList = NULL;
	appPrefs_t				prefs;
	Char					strTime[STRLEN_TIME] = "\0";

	ReadAppPrefs( &prefs );
	
	SetCtlBoolValue( pForm, MAIN_CBX_ENABLE, prefs.bEnabled );
	// Chime
	SetCtlBoolValue( pForm, CHM_CBX_KILL, prefs.chime.bAllowKill );
	SetCtlBoolValue( pForm, CHM_CBX_VIB, prefs.chime.bVibrate );
	TimeToAscii( prefs.chime.startTime.hours, prefs.chime.startTime.minutes, PrefGetPreference( prefTimeFormat ), strTime );
	SetCtlLabelStr( pForm, CHM_TRG_FROM, strTime );
	TimeToAscii( prefs.chime.endTime.hours, prefs.chime.endTime.minutes, PrefGetPreference( prefTimeFormat ), strTime );
	SetCtlLabelStr( pForm, CHM_TRG_TO, strTime );
	SetPopLabelFromList( pForm, CHM_POP_INTERVAL, CHM_LST_INTERVAL, prefs.chime.chimeIntervalIdx );
	SetPopLabelFromList( pForm, CHM_POP_DISP_CLK, CHM_LST_DISP_CLK, prefs.chime.timeoutIdx );	
	SetPopLabelFromList( pForm, CHM_POP_BEEP, CHM_LST_BEEP, prefs.sounds.toneVolume );
	SetPopLabelFromList( pForm, CHM_POP_LED, CHM_LST_LED, prefs.chime.flashLEDIdx );
	// Popup
	SetCtlBoolValue( pForm, POP_CBX_ENABLE, prefs.popup.bEnabled );
	SetCtlBoolValue( pForm, POP_CBX_ALLOW_KEY, prefs.popup.bKeyPassthrough );
	SetPopLabelFromList( pForm, POP_POP_TIMEOUT, POP_LST_TIMEOUT, prefs.popup.timeoutIdx );
	SetPopLabelFromList( pForm, POP_POP_KEY, POP_LST_KEY, prefs.popup.popupkeyIdx );
	SetPopLabelFromList( pForm, POP_POP_MOD, POP_LST_MOD, prefs.popup.popupModifierKeyIdx );
	// Keyguard
	SetCtlBoolValue( pForm, KG_CBX_ENABLE, prefs.keyguard.bEnabled );
	SetCtlBoolValue( pForm, KG_CBX_NO_CLK_ON_SMS, prefs.keyguard.bNoKeyguardOnAttnAlert );
	SetPopLabelFromList( pForm, KG_POP_TIMEOUT, KG_LST_TIMEOUT, prefs.keyguard.timeoutIdx );
	SetPopLabelFromList( pForm, KG_POP_KEY, KG_LST_KEY, prefs.keyguard.unlockKeyIdx );
	SetPopLabelFromList( pForm, KG_POP_MOD, KG_LST_MOD, prefs.keyguard.unlockModifierKeyIdx );
	
	return;
	
} // MainForm_Initialize()

/*
 * MainForm_SavePrefs()
 */
static Boolean MainForm_SavePrefs( FormType* pForm )
{
	Boolean					retVal = false;
	appPrefs_t				prefs;
	usPrefs_t				usPrefs;
	
	if ( !pForm ) return ( retVal );

	ReadAppPrefs( &prefs );
	
	prefs.bEnabled = GetCtlBoolValue( pForm, MAIN_CBX_ENABLE );
	// Chime
	prefs.chime.bAllowKill = GetCtlBoolValue( pForm, CHM_CBX_KILL );
	prefs.chime.bVibrate = GetCtlBoolValue( pForm, CHM_CBX_VIB );
	prefs.chime.chimeIntervalIdx = GetListIdx( pForm, CHM_LST_INTERVAL );
	prefs.chime.timeoutIdx = GetListIdx( pForm, CHM_LST_DISP_CLK );
	prefs.sounds.toneVolume = GetListIdx( pForm, CHM_LST_BEEP );
	prefs.chime.flashLEDIdx = GetListIdx( pForm, CHM_LST_LED );	
	// Popup
	prefs.popup.bEnabled = GetCtlBoolValue( pForm, POP_CBX_ENABLE );
	prefs.popup.bKeyPassthrough = GetCtlBoolValue( pForm, POP_CBX_ALLOW_KEY );
	prefs.popup.timeoutIdx = GetListIdx( pForm, POP_LST_TIMEOUT );
	prefs.popup.popupkeyIdx = GetListIdx( pForm, POP_LST_KEY );
	prefs.popup.popupModifierKeyIdx = GetListIdx( pForm, POP_LST_MOD );
	// Keyguard
	prefs.keyguard.bEnabled = GetCtlBoolValue( pForm, KG_CBX_ENABLE );
	prefs.keyguard.bNoKeyguardOnAttnAlert = GetCtlBoolValue( pForm, KG_CBX_NO_CLK_ON_SMS );
	prefs.keyguard.timeoutIdx = GetListIdx( pForm, KG_LST_TIMEOUT );
	prefs.keyguard.unlockKeyIdx = GetListIdx( pForm, KG_LST_KEY );
	prefs.keyguard.unlockModifierKeyIdx = GetListIdx( pForm, KG_LST_MOD );
	
	prefs.keyguard.unlockKeyName[0] = chrNull;
	if ( prefs.keyguard.unlockKeyIdx )
	{
		StrCopy( prefs.keyguard.unlockKeyName, LstGetSelectionText( 
								FrmGetObjectPtr(pForm, FrmGetObjectIndex( pForm, KG_LST_KEY ) ), 
								prefs.keyguard.unlockKeyIdx ) );
	}
	
	prefs.keyguard.unlockModifierKeyName[0] = chrNull;
	if ( prefs.keyguard.unlockModifierKeyIdx )
	{
		StrCopy( prefs.keyguard.unlockModifierKeyName, LstGetSelectionText( 
							FrmGetObjectPtr(pForm, FrmGetObjectIndex( pForm, KG_LST_MOD ) ), 
							prefs.keyguard.unlockModifierKeyIdx ) );
		StrCat( prefs.keyguard.unlockModifierKeyName, " + " );
	}
	
	WriteAppPrefs( &prefs );
	
	RegisterForNotifications( &prefs );
	SetNextAlarm();	
	
	InitializeUsPrefs( &usPrefs );
	WriteUsPrefs( &usPrefs );
	
	return ( retVal );	

} // MainForm_SavePrefs()

/*
 * MainForm_HandleMenu()
 */
static Boolean MainForm_HandleMenu( UInt16 menuID )
{
	Boolean 			handled = false;
	EventType			newEvent;

	switch ( menuID )
	{
		case MAIN_MENU_EXIT:
		
			newEvent.eType = appStopEvent;
									
			EvtAddEventToQueue( &newEvent );	
					
			handled = true;
		
			break;

		case MAIN_MENU_APPPREFS:
		
			FrmPopupForm( APPPREFS_FORM );
			
			handled = true;
			
			break;
			
		case MAIN_MENU_HELP:
		
			FrmHelp( STRING_MAIN_HELP );
					
			handled = true;
			
			break;			
			
		case MAIN_MENU_ABOUT:
		
			FrmPopupForm( ABOUT_FORM );
			
			handled = true;
			
			break;
						
		default:
			break;
	}

	return ( handled );

} // MainForm_HandleMenu()

/*
 * FormMain.c
 */