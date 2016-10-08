/*
 * Utils.c
 */
 
#include "Utils.h"

/*
 * EvtCharEqualsLaunchChar()
 *
 * <HsChars.h>
 * 
 * // enqueued after the display is turned on
 * // (the display is usually still off for Palm's late wakeup)
 * #define hsChrWakeup			0x1618
 * 
 * // causes the keyguard dialog to be displayed
 * // for internal use only... use hsAttrKeyboardLocked to programmatically enable/disable it
 * #define hsChrKeyboardLock	0x1619
 * #define hsChrModifierKey		0x160D // was hsChrUpdateGrfIndicator
 * 
 * #define vchrHardRockerCenter	0x013D
 * 
 * <Chars.h>
 * 
 * #define	vchrAutoOff			0x0114		// power off due to inactivity timer
 * #define	vchrPowerOff		0x011E		// Posted after autoOffChr or hardPowerChr
 * #define	vchrResumeSleep		0x011F		// Posted by NotifyMgr clients after they
 * 
 * vchrRockerUp 				0x0132 
 * vchrRockerDown 				0x0133
 * vchrLaunch 					0x0108
 * vchrHard4 					0x0207
 *
 * #define shiftKeyMask       	0x0001
 * #define capsLockMask       	0x0002
 * #define numLockMask        	0x0004
 * #define commandKeyMask     	0x0008
 * #define optionKeyMask      	0x0010
 * #define controlKeyMask     	0x0020
 * #define autoRepeatKeyMask  	0x0040      // True if generated due to auto-repeat
 * #define doubleTapKeyMask   	0x0080      // True if this is a double-tap event
 * #define poweredOnKeyMask   	0x0100      // True if this is a double-tap event
 * #define appEvtHookKeyMask  	0x0200      // True if this is an app hook key
 * #define libEvtHookKeyMask  	0x0400      // True if this is a library hook key
 * #define willSendUpKeyMask  	0x0800      // True if a keyUp event will be sent later
 * #define softwareKeyMask    	0x1000 
 *
 */
Boolean EvtCharEqualsLaunchChar( WChar evt_wChar, UInt16 evt_keyCode, UInt16 evt_modifier, UInt16 keyCharIdx, UInt16 keyModifierIdx )
{

	UInt16 			wChars[13] = { 0, vchrPageUp /* 0x000B */, vchrPageDown /* 0x000C */, 
										vchrRockerLeft /* 0x0134 */, vchrRockerRight /* 0x0135 */,
										vchrRockerCenter /* 0x0136 */, 
										vchrHard4 /* 0x0207 */, vchrMenu /* 0x0105 */, 
										vchrHard2 /* 0x0205 */, vchrHard3 /* 0x0206 */,
										hsChrSide /* 0x161F */, hsChrVolumeUp /* 0x161B */, hsChrVolumeDown /* 0x161C */ };
	UInt16			modifiers[5] = { willSendUpKeyMask | commandKeyMask, willSendUpKeyMask | shiftKeyMask | commandKeyMask,
			  							willSendUpKeyMask | optionKeyMask | commandKeyMask, willSendUpKeyMask | doubleTapKeyMask | commandKeyMask, 
			  							willSendUpKeyMask | autoRepeatKeyMask | commandKeyMask };		  					
	Boolean 		retVal = false;
	WChar			sel_wChar = wChars[keyCharIdx];
	UInt16			sel_modifier = modifiers[keyModifierIdx];
	prevModPref_t	modPrefs;
	
	ReadModifierCharPrefs( &modPrefs );		
			
	switch ( sel_wChar )
	{	
		case vchrPageUp: 			/* 0x000B */
		case vchrPageDown:			/* 0x000C */
		case vchrRockerLeft:		/* 0x0134 */
		case vchrRockerRight:		/* 0x0135 */
		case hsChrSide:				/* 0x161F */
		case hsChrVolumeUp:			/* 0x161B */
		case hsChrVolumeDown:		/* 0x161C */
		
			if ( evt_wChar == sel_wChar )
			{			
				retVal = ( ( evt_modifier == sel_modifier ) || ( modPrefs.prevModifier == sel_modifier ) );
			}
			
			break;
		
		case vchrHard2:				/* 0x0205 */
		case vchrHard3:				/* 0x0206 */
		case vchrHard4:				/* 0x0207 */
		
			if ( evt_keyCode == sel_wChar )
			{
				retVal = ( ( evt_modifier == sel_modifier ) || ( modPrefs.prevModifier == sel_modifier ) );
			}
			
			break;
			
		case vchrRockerCenter:		/* 0x0136 */
		
			if ( evt_wChar == sel_wChar )
			{	
				if ( modPrefs.prevModifier )
				{
					retVal = ( modPrefs.prevModifier == sel_modifier );
				}
				else
				{
					retVal = ( evt_modifier == ( sel_modifier & 0x00FF ) ) ;
				}
			}
				
			break;
			
		case vchrMenu: 				/* 0x0105 */
		
			if ( evt_wChar == sel_wChar )
			{
				retVal = ( evt_modifier == sel_modifier );
			}
			else if ( evt_keyCode == sel_wChar )
			{
				retVal = ( evt_modifier == ( sel_modifier | controlKeyMask ) );
			}
			
			break;
			
		default:
			break;	
	}
	
	if ( hsChrModifierKey == evt_wChar )
	{	
		modPrefs.prevModifier = evt_modifier;
		WriteModifierCharPrefs( &modPrefs );
	}
	else if ( ( modPrefs.prevModifier != 0 ) 
					/* && ( willSendUpKeyMask == ( evt_modifier & 0xFF00 ) ) */
					&& ( vchrHardRockerCenter != evt_wChar ) )
	{
		modPrefs.prevModifier = 0;
		WriteModifierCharPrefs( &modPrefs );
	}
	
	if ( ( retVal ) && ( keyModifierIdx == 4 /* autoRepeatKeyMask */ ) )
	{
		HsKeyStop( evt_keyCode );
	}
	
	return ( retVal );
	
} // EvtCharEqualsLaunchChar()

/*
 * is_display_off()
 */
Boolean is_display_off( void )
{
    UInt32 tui16;

    return (!(!HsAttrGet(hsAttrDisplayOn, 0, (UInt32 *) &tui16) && tui16)); // weird!!! not thinking here!

} // is_display_off()

/*
 * OpenKeyLib()
 */
Boolean OpenKeyLib( UInt16* pmKeyLibRefNumP )
{
	Boolean pmKeyLibLoaded = false;
	
	if ( SysLibFind( kPmKeyLibName, pmKeyLibRefNumP ) )
	{
		pmKeyLibLoaded = ( ( ( SysLibLoad( kPmKeyLibType, kPmKeyLibCreator, pmKeyLibRefNumP ) ) == errNone )
								&& ( *pmKeyLibRefNumP != sysInvalidRefNum ) );
	}
	
	if ( *pmKeyLibRefNumP )
	{
		PmKeyLibOpen( *pmKeyLibRefNumP );
	}
	
	return ( pmKeyLibLoaded );
	
} // OpenKeyLib(

/*
 * CloseKeyLib()
 */
void CloseKeyLib( UInt16 pmKeyLibRefNum, Boolean pmKeyLibLoaded )
{
	if ( pmKeyLibRefNum )
	{
		PmKeyLibClose( pmKeyLibRefNum );
		
		if ( pmKeyLibLoaded )
		{
			SysLibRemove( pmKeyLibRefNum );	
		}	
	}
	
	return;
	
} // CloseKeyLib()

/*
 * GetListIdx()
 */
UInt16 GetListIdx( FormType* pForm, UInt16 lstID )
{
	UInt16		retVal = noListSelection;
	ListType*	pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, lstID ) );
	
	if ( pList )
	{
		retVal = LstGetSelection( pList );
	}
	
	return ( retVal );
		
} // GetListIdx()

/*
 * SetOneTime()
 */
Boolean SetOneTime( FormType* pForm, UInt16 ctlID, TimeType* timeP, Char* titleStr )
{	
	Boolean			retVal = true;
	UInt16			objIdx = 0;
	Int16			trgHour = 0;
	Int16			trgMin = 0;
	Char			strTime[STRLEN_TIME] = "\0";
	
	if ( ( !pForm ) || ( !timeP ) ) return ( retVal );
	
	objIdx = FrmGetObjectIndex( pForm, ctlID );
	
	if ( objIdx == frmInvalidObjectId ) return ( retVal );
	
	trgHour = (Int16) ( timeP->hours );
	trgMin = (Int16) ( timeP->minutes );
	
	if ( ( retVal = SelectOneTime( &trgHour, &trgMin, titleStr ) ) )
	{
		timeP->hours  = (UInt8) trgHour;
		timeP->minutes = (UInt8) trgMin;
		
		TimeToAscii( timeP->hours, timeP->minutes, PrefGetPreference( prefTimeFormat ), strTime );
		
		SetCtlLabelStr( pForm, ctlID, strTime );
		
		FrmShowObject( pForm, objIdx );
	}
	
	return ( retVal );
	
} // SetOneTime()

/*
 * DateCompare()
 * 
 * 				if d1 > d2  returns a positive int
 *              if d1 < d2  returns a negative int
 *              if d1 = d2  returns zero
 */
Int16 DateCompare( DateType d1, DateType d2 )
{
	UInt16 		int1 = 0;
	UInt16		int2 = 0;
	
	int1 = DateToInt( d1 );
	int2 = DateToInt( d2 );
	
	return ( int1 - int2 );

} // DateCompare()

/* 
 * TimeCompare()
 * 
 * 			if t1 > t2  returns a positive int
 *          if t1 < t2  returns a negative int
 *          if t1 = t2  returns zero
 */
Int16 TimeCompare( TimeType t1, TimeType t2 )
{
	Int16 		int1 = 0;
	Int16 		int2 = 0;
	
	int1 = TimeToInt( t1 );
	int2 = TimeToInt( t2 );
	
	return ( int1 - int2 );

} // TimeCompare()

/*
 * GetPhoneVolume()
 * typedef UInt16  ToneVolume;
 */
ToneVolume GetPhoneVolume( Boolean* pSoundOnVibrate, Boolean* pSoundOffVibrate )
{
	ToneVolume			volume = toneVolumeOff;
	Err					error = errNone;
	UInt16				tonesLibRefNum = 0;
	Boolean				tonesLibLoaded = false;
	SoundPreference		phnSndPref;
	
	if ( SysLibFind( tonesLibName, &tonesLibRefNum ) )
	{
		tonesLibLoaded = ( ( SysLibLoad( tonesLibType, tonesLibCreator, &tonesLibRefNum ) == errNone )
				&& ( tonesLibRefNum != sysInvalidRefNum ) );
	}
	
	error = TonesLibOpen( tonesLibRefNum );
					
	if (!error)
	{
		TonesLibGetSoundPrefs( tonesLibRefNum, soundPrefTypePhone, &phnSndPref );
		volume = phnSndPref.soundOnVolume;
	
		if ( pSoundOnVibrate )
		{
			*pSoundOnVibrate = phnSndPref.soundOnVibrate;
		}
		
		if ( pSoundOffVibrate )
		{
			*pSoundOffVibrate = phnSndPref.soundOffVibrate;
		}
		
		TonesLibClose( tonesLibRefNum );
	}
	
	if ( tonesLibLoaded )
	{
		SysLibRemove( tonesLibRefNum );	
	}
	
	return ( volume );
	
} // GetPhoneVolume()

/* 
 * IsRoaming()
 */
Boolean IsRoaming( void )
{
	Boolean					retVal = false;
	UInt16 					phnLibRefNum = sysInvalidRefNum;
	PhnRoamStatusEnum		roamingStatus = PhnRoamStatusBlink;
	
	if ( HsGetPhoneLibrary( &phnLibRefNum ) )
		return ( retVal );
					
	if ( PhnLibOpen( phnLibRefNum ) )
		return ( retVal );
		
	if ( ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOn )
			&& ( PhnLibRegistered( phnLibRefNum ) ) )
	{
		if ( !PhnLibRoaming( phnLibRefNum, &roamingStatus ) )
		{
			retVal = ( roamingStatus == PhnRoamStatusOn );
		}
	}
		
	PhnLibClose( phnLibRefNum );
	
	return ( retVal );
	
} // IsRoaming()

/*
 * GetCurrentOperator()
 */
Err GetCurrentOperator( Char* CurrentOperatorName, UInt16 strLenCurrentOperator )
{
	Err						error = errNone;
	UInt16 					phnLibRefNum = sysInvalidRefNum;
	PhnOperatorID			uCurrentOperatorID = 0;
  	Char*					PrvCurrentOperatorName = NULL;
  	
  	if ( !CurrentOperatorName ) 
  		return ( memErrNotEnoughSpace );
  	
  	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) != errNone )
  		return ( error );

	if ( ( error = PhnLibOpen( phnLibRefNum ) ) != errNone ) 
		return ( error );
		
	if ( PhnLibModulePowered( phnLibRefNum ) != phnPowerOn )
		return ( 1 );
		
	if ( !PhnLibRegistered( phnLibRefNum ) )
		return ( 2 );
		
	error = PhnLibCurrentOperator( phnLibRefNum, &uCurrentOperatorID, &PrvCurrentOperatorName, NULL );
	StrNCopy( CurrentOperatorName, PrvCurrentOperatorName, strLenCurrentOperator - 1 );
	CurrentOperatorName[strLenCurrentOperator - 1] = chrNull;
	
	if ( PrvCurrentOperatorName ) MemPtrFree( PrvCurrentOperatorName );

	error = errNone;
		
	return ( error );

} // GetCurrentOperator()

/*
 * GetBatteryState()
 */
UInt8 GetBatteryState( Boolean* bPluggedinP )
{
   	UInt8 	percent = 0;
   
	SysBatteryInfo ( false, NULL, NULL, NULL, NULL, bPluggedinP, &percent );

	return ( percent );
	
} // GetBatteryState()

/*
 * RomVersionCompatible()
 */
Err RomVersionCompatible( UInt32 requiredVersion, UInt16 launchFlags )
{	
	UInt32 	rom = 0;
	UInt32 	hal = 0;
	UInt32	company = 0;
	UInt32	device = 0;
	UInt32	hsFtrVersion;

	if( FtrGet( hsFtrCreator, hsFtrIDVersion, &hsFtrVersion ) != 0 )
	{
		return ( 1 );
	}
	
    FtrGet( sysFtrCreator, sysFtrNumOEMHALID, &hal );
    FtrGet( sysFtrCreator, sysFtrNumOEMCompanyID, &company );
    FtrGet( sysFtrCreator, sysFtrNumOEMDeviceID, &device );
	FtrGet( sysFtrCreator, sysFtrNumROMVersion, &rom );
		
	if (rom >= requiredVersion
		&&	(company == kPalmCompanyIDPalm
			|| company == kPalmCompanyIDOldPalm
			|| kPalmCompanyIDHandspring)

		&&	(device == kPalmOneDeviceIDTreo650
			|| device == kPalmOneDeviceIDTreo650Sim

			|| device == kPalmOneDeviceIDTreo600
			|| device == kPalmOneDeviceIDTreo600Sim

			|| device == 'D053' // Treo 680
			|| device == 'H104'

			|| device == 'D052' // Treo 700p
			|| device == 'H103'

			|| device == 'D060' // Treo 755p
			|| device == 'H104'
			
			|| device == 'D061' // Centro ???
			|| device == 'D062' // Centro
			)
			
		&&	(hal == hsHALIDHandspringOs5Rev2 // Treo 650
			|| hal == hsHALIDHandspringOs5Rev2Sim

			|| hal == hsHALIDHandspringOs5Rev1 // Treo 600
			|| hal == hsHALIDHandspringOs5Rev1Sim

			|| hal == hsHALIDHandspringOs5Rev4 // Treo 680, 755p
			|| hal == hsHALIDHandspringOs5Rev4Sim

			|| hal == hsHALIDHandspringOs5Rev3 // Treo 700p
			|| hal == hsHALIDHandspringOs5Rev3Sim
			
			|| hal == 'P052' // Centro ???
			|| hal == 'P053' // Centro
		) )
	{
		return 0;
	}

	if ((launchFlags & LAUNCH_FLAGS) == LAUNCH_FLAGS)
	{
		// Pilot 1.0 will continuously relaunch this app unless we switch to 
		// another safe one.
		if ( rom < sysMakeROMVersion( 2, 0, 0, sysROMStageRelease, 0 ) )
			AppLaunchWithCommand( sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL );
	}

	return ( sysErrRomIncompatible );

} // RomVersionCompatible()

/*
 * ReplaceChar
 */
Char* ReplaceChar( unsigned char *s, char orgChr, char newChr )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
	{
		if ( c == orgChr )
			*d++ = newChr;
		else
			*d++ = c;
	}
					
	*d = 0;
	
	return ( s );
	
} // ReplaceChar

/*
 * StripSpace
 */
Char* StripSpaces( unsigned char *s )
{
	unsigned char c, *d = s;
	
	while ( ( c = *s++ ) )
	{
		if ( c != chrSpace )
		{
			*d++ = c;
		}
	}
					
	*d = 0;
	
	return ( s );
	
} // StripSpaces

/*
 * Utils.c
 */