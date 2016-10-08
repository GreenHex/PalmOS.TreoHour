/*
 * Clock.c
 */

#include "Clock.h" 

// Prototypes
static void 				ClockForm_Initialize( FormType* pForm, UInt32 secondsNow );
static Boolean 				ClockForm_HandleEvent( EventType* pEvent, clockState_e clockState, appPrefs_t* pPrefs, UInt16* pmKeyLibRefNumP );
static LoopExitType_e 		ClockEventLoop( UInt32 secondsNow, UInt32 timeoutSecs, clockState_e clockState, appPrefs_t* pPrefs );

/*
 * ShowClock()
 */
LoopExitType_e ShowClock( clockState_e newState, UInt32 secondsNow )
{
	LoopExitType_e				retVal = exitTimedout;
	FormType*					pForm = NULL;
	FormActiveStateType			activeState;
	appPrefs_t					prefs;
	UInt32						timeoutSecs1[6] = { 0, 2, 3, 5, 8, 10 };
	UInt32						timeoutSecs2[8] = { 2, 3, 5, 8, 10, 15, 20, 32500 }; // nine hours is for "infinite"
	UInt32						timeoutSecs = 0;
	usPrefs_t					usPrefs;
	UInt16						ledOldState = kIndicatorStateNull;
	UInt16						ledOldCount = kIndicatorCountGetState;
	UInt16						ledNewState;
	UInt16						ledNewCount = kIndicatorCountForever;
	
	ReadUsPrefs( &usPrefs );
	
	if ( usPrefs.clockState != stateSleep ) return ( retVal );

	usPrefs.clockState = newState;
	usPrefs.clockTime = secondsNow;
	WriteUsPrefs( &usPrefs );
	
	ReadAppPrefs( &prefs );

	switch ( newState )
	{
		case stateChime:
		
			if ( !( prefs.chime.chimeIntervalIdx ) )
			{
				break;
			}
			
			timeoutSecs = timeoutSecs1[prefs.chime.timeoutIdx];
			
			break;
			
		case statePopup:
		
			if ( !( prefs.popup.bEnabled ) )
			{
				break;
			}
		
			timeoutSecs = timeoutSecs2[prefs.popup.timeoutIdx];
			
			break;
			
		case stateKeyguard:

			if ( !( prefs.keyguard.bEnabled ) )
			{
				break;
			}
			
			timeoutSecs = timeoutSecs2[prefs.keyguard.timeoutIdx];
			
			break;
			
		default:
			break;
	}	

	// Make Clock
	if ( timeoutSecs )
	{
		FrmSaveActiveState( &activeState );
	
		pForm = FrmInitForm( CLOCK_FORM );
	
		if ( pForm )
		{
			ClockForm_Initialize( pForm, secondsNow );
		}	
	}
		
	if ( newState == stateChime )
	{
		if ( prefs.chime.flashLEDIdx )
		{
			LEDState( false, &ledOldCount, &ledOldState );
			ledNewState = ledOldState & 0xFF00;
			LEDState( true, &ledNewCount, &ledNewState );
		
			FlashLED( true, prefs.chime.flashLEDIdx - 1 );
		}
		
		if ( prefs.chime.bVibrate )
		{
			Vibrate();
		}
		
		if ( prefs.sounds.toneVolume )
		{
			PlayChime( secondsNow, &( prefs.sounds ), true );
		}
	}
	
	// Delete Clock
	if ( ( pForm ) && ( timeoutSecs ) )
	{
		retVal = ClockEventLoop( secondsNow, timeoutSecs, newState, &prefs );
	
		FrmEraseForm( pForm );
		FrmDeleteForm( pForm );
				
		FrmRestoreActiveState( &activeState );
	}
	
	// Cleanup
	if ( newState == stateKeyguard )
	{
		if ( ( retVal == exitTimedout ) && ( prefs.keyguard.unlockKeyIdx ) )
		{
			EvtEnqueueKey( (WChar) vchrPowerOff, 0, commandKeyMask );
		}
	}	
	else if ( newState == stateChime )
	{
		if ( prefs.chime.flashLEDIdx )
		{
			FlashLED( false, prefs.chime.flashLEDIdx - 1 );
			
			ledOldCount = kIndicatorCountForever;
			LEDState( false, &ledOldCount, &ledOldState );
		}
	
		if ( usPrefs.bSleepAfterChime ) 
		{
			EvtEnqueueKey( (WChar) vchrPowerOff, 0, commandKeyMask );
		}
	}
	
	InitializeUsPrefs( &usPrefs );
	WriteUsPrefs( &usPrefs );
	
	return ( retVal );	
	
} // ShowClock()

/*
 * ClockForm_Initialize()
 */
static void ClockForm_Initialize( FormType* pForm, UInt32 secondsNow )
{	
	FrmSetActiveForm( pForm );
	
	InitializeClockGadgets( pForm );
	CommonInitGauge( pForm );
	
	FrmDrawForm( pForm );
		
	return;	

} // ClockForm_Initialize()

/*
 * ClockForm_HandleEvent()
 */
static Boolean ClockForm_HandleEvent( EventType* pEvent, clockState_e clockState, appPrefs_t* pPrefs, UInt16* pmKeyLibRefNumP )
{
	Boolean			handled = false;
	FormType*		pForm = FrmGetActiveForm();
	WChar			wChar = 0;
	UInt16			keyCode = 0;
	UInt16			modifier = 0; 
	Boolean			bExitLoopFlag = false;
	// UInt16 			keyCodes[1];
	// Boolean 		pressed[1];
		
	switch ( pEvent->eType )
	{
		case keyDownEvent:
			
			wChar = pEvent->data.keyDown.chr;
			keyCode = pEvent->data.keyDown.keyCode;
			modifier = pEvent->data.keyDown.modifiers;
			
			switch ( clockState )
			{
				case stateChime:
				case statePopup:
					
					switch ( wChar )
					{
						case vchrHardPower:		 	// 0x0208
						case vchrAutoOff:			// 0x0114
 						case vchrPowerOff:			// 0x011E
						// case vchrResumeSleep:		// 0x011F
							
							bExitLoopFlag = true;
							
							break;
							
						case hsChrKeyboardLock: 	// 0x1618
						case vchrHardRockerCenter: 	//0x013D
						
							break;
							
						default:
							
							if (  pPrefs->chime.bAllowKill )
							{					
								if ( ( wChar > vchrHardKeyMin /* 0x0200 */ ) && ( wChar < vchrHardKeyMax /* 0x02FF */ ) )
								{
									bExitLoopFlag = true;
								}
								else if ( wChar == vchrRockerCenter	/* 0x0136 */ )
								{
									bExitLoopFlag = true;
								}
								else if ( ( pmKeyLibRefNumP ) && ( PmKeyEventIsFromKeyboard( *pmKeyLibRefNumP, pEvent ) ) )
								{
									bExitLoopFlag = true;
								}
							}
							
							break;
					}
					
					SetCtlBoolValue( pForm, CLOCK_PBT_EXIT, bExitLoopFlag );
					
					break;
					
				case stateKeyguard:
					
					switch ( wChar )
					{
						case vchrHardPower:		 	// 0x0208
						case vchrAutoOff:			// 0x0114
 						case vchrPowerOff:			// 0x011E
 						// case vchrResumeSleep:		// 0x011F
 						
							bExitLoopFlag = true;
							
							break;
							
						// case hsChrKeyboardLock: 	// 0x1618
						case vchrHardRockerCenter: 	//0x013D
						
							break;
							
						default:
					
							if ( pPrefs->keyguard.unlockKeyIdx )
							{
								if ( EvtCharEqualsLaunchChar( wChar, keyCode, modifier, pPrefs->keyguard.unlockKeyIdx, pPrefs->keyguard.unlockModifierKeyIdx ) ) 
								{
									bExitLoopFlag = true;
									handled = true;
								}
							}
							else
							{
								if ( ( wChar > vchrHardKeyMin /* 0x0200 */ ) && ( wChar < vchrHardKeyMax /* 0x02FF */ ) )
								{
									bExitLoopFlag = true;
									handled = true;
								}
								else if ( wChar == vchrRockerCenter	/* 0x0136 */ )
								{
									bExitLoopFlag = true;
									handled = true;
								}
								else if ( ( pmKeyLibRefNumP ) && ( PmKeyEventIsFromKeyboard( *pmKeyLibRefNumP, pEvent ) ) )
								{
									bExitLoopFlag = true;
									handled = true;
								}
							}

							break;
					}
					
					SetCtlBoolValue( pForm, CLOCK_PBT_EXIT, bExitLoopFlag );
										
					break;
					
				default:
					
					SetCtlBoolValue( pForm, CLOCK_PBT_EXIT, true );
				
					break;
			}
			
			break;
			
		case nilEvent:
		
			break;
		
		default:
			break;
			
	}		
						
	return ( handled );	
	
} // ClockForm_HandleEvent()

/*
 * ClockEventLoop()
 */
static LoopExitType_e ClockEventLoop( UInt32 secondsNow, UInt32 timeoutSecs, clockState_e clockState, appPrefs_t* pPrefs )
{
	LoopExitType_e		retVal = exitTimedout;
	EventType			event;
	UInt32				timeExit = timeoutSecs + secondsNow;
	Int32				timeoutTics = (Int32) timeoutSecs * (Int32) SysTicksPerSecond();
	UInt16				pmKeyLibRefNum = sysInvalidRefNum;
	Boolean				pmKeyLibLoaded = OpenKeyLib( &pmKeyLibRefNum );
	
	do {

		EvtGetEvent( &event, (Int32) SysTicksPerSecond() /* timeoutTics */ );
		
		FrmDispatchEvent( &event );
		
		ClockForm_HandleEvent( &event, clockState, pPrefs, ( pmKeyLibRefNum != sysInvalidRefNum ) ? &pmKeyLibRefNum : NULL );
		
		if ( SysHandleEvent( &event ) )
			continue;
					
	} while ( ( TimGetSeconds() < timeExit )
				&& ( event.eType != appStopEvent )
				&& ( !GetCtlBoolValue( FrmGetActiveForm(), CLOCK_PBT_EXIT ) ) );

	retVal = ( GetCtlBoolValue( FrmGetActiveForm(), CLOCK_PBT_EXIT ) ) ? exitUnlocked : exitTimedout;
	
	CloseKeyLib( pmKeyLibRefNum, pmKeyLibLoaded );
	
	if ( clockState != stateChime )
	{
		SndPlaySystemSound( sndClick );
	}
	
	return ( retVal );
	
} // ClockEventLoop()

/*
 * Clock.c
 */