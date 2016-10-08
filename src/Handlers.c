/*
 * Handlers.c
 */
 
#include "Handlers.h"

/*
 * HandleNotification()
 */
Err HandleNotification( SysNotifyParamType* notifyParamP )
{
	Err				error = errNone;
	appPrefs_t		prefs;
	usPrefs_t		usPrefs;
	WChar			wChar = 0;
	UInt16			keyCode = 0;
	UInt16			modifier = 0;
	
	switch ( notifyParamP->notifyType )
	{
		case pmNotifyBroadcastNewMsgCount:
		
			HandleHelperNotifications( (PmBroadcastNewMsgCountNotifyParamType *) notifyParamP->notifyDetailsP );
			
			notifyParamP->handled = true;
			
			break;
			
		case sysNotifyEarlyWakeupEvent:
		
			SendMsgCountNotifications( false );
			
			notifyParamP->handled = true;
			
			break;
			
		case sysNotifyVirtualCharHandlingEvent:

			ReadAppPrefs( &prefs );
			
			if ( prefs.bEnabled ) 
			{
				wChar = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.chr;
				keyCode = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.keyCode;
				modifier = ( ( SysNotifyVirtualCharHandlingType *) notifyParamP->notifyDetailsP )->keyDown.modifiers;
				
				if ( ( wChar == hsChrKeyboardLock ) && ( modifier == commandKeyMask )  ) // manual enable
				{
					if ( prefs.keyguard.bEnabled )
					{
						ShowClock( stateKeyguard, TimGetSeconds() );
						
						notifyParamP->handled = ( wChar == hsChrKeyboardLock );
					}
				}
				else if ( ( wChar == hsChrKeyboardLock ) && ( modifier == ( poweredOnKeyMask | commandKeyMask ) )  ) 
				{
					if ( prefs.keyguard.bEnabled )
					{
						ReadUsPrefs( &usPrefs );
						
						if ( !usPrefs.bSleepAfterChime /* Not woken by clock */ ) 
						{
							if ( usPrefs.bNoKeyguardOnAttnAlert ) // reset and exit
							{
								usPrefs.bNoKeyguardOnAttnAlert = false;
							
								WriteUsPrefs( &usPrefs );
							}
							else // do keyguard
							{ 
								ShowClock( stateKeyguard, TimGetSeconds() );								
							}
							
							notifyParamP->handled = ( wChar == hsChrKeyboardLock );
						}
					}
					
					// DO NOT lock if device is woken by a chime...
					if ( (  prefs.chime.chimeIntervalIdx ) && ( usPrefs.bSleepAfterChime ) )
					{
						notifyParamP->handled = ( wChar == hsChrKeyboardLock );
					}	
				}
				else if ( EvtCharEqualsLaunchChar( wChar, keyCode, modifier, prefs.popup.popupkeyIdx, prefs.popup.popupModifierKeyIdx ) )
				{
					if ( prefs.popup.bEnabled )
					{	
						SendMsgCountNotifications( false );
								
						notifyParamP->handled = !( prefs.popup.bKeyPassthrough );

						ShowClock( statePopup, TimGetSeconds() );
					}					
				}
			}
			
			break;
					
		case sysNotifyTimeChangeEvent:
			
			notifyParamP->handled = true;
			
			break;
			
		case sysNotifyGotUsersAttention:
			
			SendMsgCountNotifications( false );
			
			ReadAppPrefs( &prefs );
			
			if ( prefs.keyguard.bNoKeyguardOnAttnAlert )
			{
				ReadUsPrefs( &usPrefs );
				usPrefs.bNoKeyguardOnAttnAlert = true;
				WriteUsPrefs( &usPrefs );
			}
			
			notifyParamP->handled = true;
			
			break;
			
		default: 
			break;
	}
	
	return ( error );	
	
} // HandleNotification()


/*
 * HandleReset()
 */
void HandleReset( void )
{
	appPrefs_t				prefs;
	usPrefs_t				usPrefs;
	prevModPref_t			modPrefs;
	
	ReadAppPrefs( &prefs );
	RegisterForNotifications( &prefs );
	SetNextAlarm( );
	
	InitializeUsPrefs( &usPrefs );
	WriteUsPrefs( &usPrefs );
	InitializeModifierCharPrefs( &modPrefs );
	WriteModifierCharPrefs( &modPrefs );
	
	return;	
	
} // HandleReset()

/*
 * HandleAlarmTriggered()
 */
void HandleAlarmTriggered( void )
{
	usPrefs_t				usPrefs;
	
	ReadUsPrefs( &usPrefs );
	
	usPrefs.bSleepAfterChime = is_display_off();
	
	WriteUsPrefs( &usPrefs );

	if ( !usPrefs.bSleepAfterChime )
	{
		SendMsgCountNotifications( false );
	}
	
	SetNextAlarm( );
			
	return;	
	
} // HandleAlarmTriggered()
 
/*
 * HandleAlarmDisplay()
 */
void HandleAlarmDisplay( void )
{	
	ShowClock( stateChime, TimGetSeconds() );

	return;	

} // HandleAlarmDisplay()

/*
 * Handlers.c
 */