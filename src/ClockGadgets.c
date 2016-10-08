/*
 * ClockGadgets.h
 */
 
#include "ClockGadgets.h"
 
// Prototypes
static void 				DrawGdtPressed( RectangleType* rectP, drawStr_t* drawStrP );
static Boolean 				HandleGadgetTap( FormType* pForm, UInt16 ctlID, RectangleType* rectP, gadgetsText_t* gdtTextP );
static void 				DrawBatteryLevel( FormType* pForm, RectangleType* rectP );
static void 				DrawMemBar( FormType* pForm, UInt16 ctlID, RectangleType* rectP );

/*
 * InitializeClockGadgets()
 */
void InitializeClockGadgets( FormType* pForm )
{
	if ( !pForm ) return;
	
	FrmSetActiveForm( pForm );
 	
 	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_ALERTS ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_SOUND ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_MSGS ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_MAILS ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_TASKS ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_APPT ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	//		
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DATE ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_TIME ), (FormGadgetHandlerType *)  ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_BATTERY ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_STORAGE_MEM ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DYNAMIC_HEAP ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DB_CACHE ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_CARD ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_NEXT_APPT ), (FormGadgetHandlerType *) ClockGadgetsHandler );
	//
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_RSSI ), (FormGadgetHandlerType *) SignalLvlGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_ROAM ), (FormGadgetHandlerType *) RoamingGadgetHandler );
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_OPERATOR ), (FormGadgetHandlerType *) CurrentOperatorHandler );
	
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_ALERTS ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_SOUND ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_MSGS ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_MAILS ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_TASKS ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_APPT ), NULL );
	//
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DATE ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_TIME ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_BATTERY ), NULL );	
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_STORAGE_MEM ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DYNAMIC_HEAP ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DB_CACHE ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_CARD ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_NEXT_APPT ), NULL );
	//
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_RSSI ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_ROAM ), NULL );
	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_OPERATOR ), NULL );
	
	
	return;
	
} // InitializeClockGadgets()

/* 
 * ClockGadgetsHandler()
 */
Boolean ClockGadgetsHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	appPrefs_t				prefs;
	usPrefs_t				usPrefs;
	DateTimeType			dt;
	DateType				d;
	TimeType				t;
	RectangleType			rectDate;
	gadgetsText_t			gdtText;		
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:
			break;
			
		case formGadgetEraseCmd:
			break;
			
		case formGadgetDrawCmd:
			
			switch ( gadgetP->id )
			{
				case CLOCK_GDT_ALERTS:	
					
					ReadGdtTxtPrefs( &gdtText );
					
					DrawAlertCount( pForm, &( gadgetP->rect ), &(gdtText.alerts) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
					
				case CLOCK_GDT_SOUND:
					
					ReadGdtTxtPrefs( &gdtText );
					
					DrawSoundStatus( pForm, &( gadgetP->rect ), &(gdtText.sounds) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
					
				case CLOCK_GDT_MSGS:
					
					ReadGdtTxtPrefs( &gdtText );
					
					DrawMsgsCount( pForm, &( gadgetP->rect ), &(gdtText.msgs) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
				
				case CLOCK_GDT_MAILS:
					
					ReadGdtTxtPrefs( &gdtText );
					
					DrawEmailsCount( pForm, &( gadgetP->rect ), &(gdtText.mails) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
				
				case CLOCK_GDT_TASKS:
					
					ReadUsPrefs( &usPrefs );
			
					DateSecondsToDate( usPrefs.clockTime, &d );
				 	
				 	ReadGdtTxtPrefs( &gdtText );
				 	
					DrawTasksCount( pForm, &( gadgetP->rect ), &d, &(gdtText.tasks) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
				
				case CLOCK_GDT_APPT:
					
					ReadUsPrefs( &usPrefs );
					
					TimSecondsToDateTime( usPrefs.clockTime, &dt );
					DateSecondsToDate( usPrefs.clockTime, &d );
					
					t.hours = dt.hour;
					t.minutes = dt.minute;
					
					ReadGdtTxtPrefs( &gdtText );
					
					DrawAppointmentsCount( pForm, &( gadgetP->rect ), &d, &t, &(gdtText.appts) );
					
					WriteGdtTxtPrefs( &gdtText );
					
					break;
				
				case CLOCK_GDT_NEXT_APPT:
					
					ReadUsPrefs( &usPrefs );
					ReadAppPrefs( &prefs );
					
					if ( ( usPrefs.clockState == stateKeyguard ) && ( !prefs.bDoNotDisplayKeyguardText ) )
					{
						DrawUnlockText( pForm, &( gadgetP->rect) );
					}
					else
					{
						TimSecondsToDateTime( usPrefs.clockTime, &dt );
						DateSecondsToDate( usPrefs.clockTime, &d );
						
						t.hours = dt.hour;
						t.minutes = dt.minute;
		
						DrawNextAppointment( pForm, &( gadgetP->rect ), &d, &t );
					}
					
					if ( ( usPrefs.clockState == stateKeyguard ) 
								&& ( prefs.bDoNotDisplayKeyguardText ) 
								&& ( prefs.keyguard.unlockKeyIdx ))
					{
						MemHandle 				bmpH = NULL;
						BitmapType* 			bmpP = NULL;
						Coord					bmpSizeX = 0;
						Coord					bmpSizeY = 0;
						
						bmpP = GetBitmap( CLOCK_BMP_LOCK, &bmpSizeX, &bmpSizeY, &bmpH );
						WinDrawBitmap( bmpP, gadgetP->rect.topLeft.x + 70 /* 109 ( ( gadgetP->rect.extent.x - bmpSizeX ) / 2 ) */, 
												gadgetP->rect.topLeft.y - 136 /* ( ( gadgetP->rect.extent.y - bmpSizeY ) / 2 ) */ );
						ReleaseBitmap( &bmpH );	
						bmpH = NULL;
					}
						
					break;
					
				case CLOCK_GDT_DATE:
					
					ReadUsPrefs( &usPrefs );
					
					DateSecondsToDate( usPrefs.clockTime, &d );
		
					DrawDate( pForm, &( gadgetP->rect ), &d );
					
					break;
					
				case CLOCK_GDT_TIME:
					
					ReadUsPrefs( &usPrefs );
			
					TimSecondsToDateTime( usPrefs.clockTime, &dt );
					
					DrawTime( pForm, &( gadgetP->rect ), &dt );
					
					DateSecondsToDate( usPrefs.clockTime, &d );
					
					FrmGetObjectBounds( pForm, FrmGetObjectIndex( pForm, CLOCK_GDT_DATE ), &rectDate );
					DrawDate( pForm, &( rectDate ), &d );
					
					break;
					
				case CLOCK_GDT_BATTERY:
					
					DrawBatteryLevel( pForm, &( gadgetP->rect ) );
					
					break;
					
				case CLOCK_GDT_STORAGE_MEM:
				case CLOCK_GDT_DYNAMIC_HEAP:
				case CLOCK_GDT_DB_CACHE:
				case CLOCK_GDT_CARD:	
					
					DrawMemBar( pForm, gadgetP->id, &( gadgetP->rect ) );
					
					break;
					
				default:
					break;
			}
			
			handled = true;
			
			break;
			
		case formGadgetHandleEventCmd:	
			
			ReadAppPrefs( &prefs );
		
			if ( prefs.bEnableAppLaunchFromClock ) 
			{	
				ReadUsPrefs( &usPrefs );
				
				if ( usPrefs.clockState == stateKeyguard )
				{
					if ( prefs.keyguard.unlockKeyIdx == 0 )
					{
						ReadGdtTxtPrefs( &gdtText );
					
						handled = HandleGadgetTap( pForm, gadgetP->id, &( gadgetP->rect ), &gdtText );
					}
				}
				else
				{
					ReadGdtTxtPrefs( &gdtText );
					
					handled = HandleGadgetTap( pForm, gadgetP->id, &( gadgetP->rect ), &gdtText );
				}
			}

			break;
			
		default:
			break;		
	}
		
	return ( handled );
	
} // ClockGadgetsHandler()

/*
 * HandleGadgetTap()
 */
static Boolean HandleGadgetTap( FormType* pForm, UInt16 ctlID, RectangleType* rectP, gadgetsText_t*	gdtTextP )
{
	Boolean 				handled = false;
	appPrefs_t				prefs;
	UInt16 					cardNo = 0;
	LocalID 				dbID = NULL;
	Boolean 				newSearch = true;
   	DmSearchStateType 		stateInfo;
   	UInt32 					result;
	
	switch ( ctlID )
	{
		case CLOCK_GDT_ALERTS:
		
			DrawGdtPressed( rectP, &(gdtTextP->alerts) );
			
			AttnListOpen();
			
			handled = true;
			
			break;
				
		case CLOCK_GDT_SOUND:
			
			DrawGdtPressed( rectP, &(gdtTextP->sounds) );
			
			if ( ! DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, sysFileTApplication, 'SMng', true, &cardNo, &dbID ) )
			{
				SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );
			}
			
			handled = true;
			
			break;
			
		case CLOCK_GDT_MSGS:
		
			DrawGdtPressed( rectP, &(gdtTextP->msgs) );
			
			if ( ! DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, sysFileTApplication, 
														GetDefaultHelper( kHelperServiceClassIDSMS ), true, &cardNo, &dbID ) )
			{
				SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );
			}
			
			handled = true;
			
			break;
			
		case CLOCK_GDT_MAILS:
			
			DrawGdtPressed( rectP, &(gdtTextP->mails) );
			
			if ( ! DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, sysFileTApplication, 
														GetDefaultHelper( kHelperServiceClassIDEMail ), true, &cardNo, &dbID ) )
			{
				SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );
			}
			
			handled = true;
			
			break;
		
		case CLOCK_GDT_TASKS:

			DrawGdtPressed( rectP, &(gdtTextP->tasks) );
						
			if ( ! DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, sysFileTApplication, kPalmOneCreatorIDTasks, true, &cardNo, &dbID ) )
			{
				SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );
			}
			
			handled = true;
			
			break;
			
		case CLOCK_GDT_APPT:

			DrawGdtPressed( rectP, &(gdtTextP->appts) );
			
			if ( ! DmGetNextDatabaseByTypeCreator( newSearch, &stateInfo, sysFileTApplication, kPalmOneCreatorIDCalendar, true, &cardNo, &dbID ) )
			{
				SysUIAppSwitch( cardNo, dbID, sysAppLaunchCmdNormalLaunch, NULL );
			}

			handled = true;
			
			break;
						
		case CLOCK_GDT_NEXT_APPT:
		case CLOCK_GDT_DATE:
		case CLOCK_GDT_TIME:
		case CLOCK_GDT_BATTERY:
		case CLOCK_GDT_STORAGE_MEM:
		case CLOCK_GDT_DYNAMIC_HEAP:
		case CLOCK_GDT_DB_CACHE:
		case CLOCK_GDT_CARD:
		default:
			
			ReadAppPrefs( &prefs );
			
			handled = prefs.chime.bAllowKill;
			
			break;	
	}
	
	SetCtlBoolValue( pForm, CLOCK_PBT_EXIT, handled );
	
	return ( true );
	
} // HandleGadgetTap()

/*
 * DrawGdtPressed()
 */
static void DrawGdtPressed( RectangleType* rectP, drawStr_t* drawStrP )
{
	RectangleType			rect = *rectP;
	RGBColorType 			rgbBlack = { 0, 0, 0, 0 };
    IndexedColorType		color;
	
	if ( ( !rectP ) || ( !drawStrP ) ) return;
		
	WinPushDrawState();
	
	color = WinRGBToIndex( &rgbBlack );	
	WinSetForeColor( color );
	WinDrawRectangle( &rect, rect.extent.y / 2 );
	
	WinSetBackColor( color );
		
	rect.extent.x -= rect.extent.y;
	rect.topLeft.x += rect.extent.y/ 2;
	
	DrawStr( drawStrP->str, &rect, drawStrP->drawFont, hAlignCenter, vAlignMiddle, colorWhite, true );
	
	WinPopDrawState();
	
	SysTaskDelay( SysTicksPerSecond() / 2 );
	
	return;	
	
} // DrawGdtPressed()

/* 
 * SignalLvlGadgetHandler()
 */
Boolean SignalLvlGadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	RectangleType			rect = gadgetP->rect;
	UInt16					signalLvl = 0;
	Char					strSignalLvl[maxStrIToALen] = "\0";
    UInt16					bmpID = CLOCK_BMP_RSSI;
	MemHandle 				bmpH = NULL;
	BitmapType* 			bmpP = NULL;
	Coord					bmpSizeX = 0;
	Coord					bmpSizeY = 0;
	// RGBColorType 			rgb = { 0, 221, 221, 221 };
    // IndexedColorType		color;
	
	if ( gadgetP->id != CLOCK_GDT_RSSI ) return ( handled );
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:

			handled = true;

			break;
			
		case formGadgetEraseCmd:

			handled = false;

			break;		
		
		case formGadgetHandleEventCmd:
		case formGadgetDrawCmd:
	
			FrmSetActiveForm( pForm );
			
			gadgetP->attr.visible = true;
							
			if ( !GetSigLvl( &signalLvl, strSignalLvl ) )
			{	
				WinPushDrawState();

				// color = WinRGBToIndex( &rgb );	
	    		// WinSetBackColor( color );
				
				DrawStr( strSignalLvl, &rect, stdFont, hAlignCenter, vAlignMiddle, colorGreen, true );
				
				bmpP = GetBitmap( bmpID, &bmpSizeX, &bmpSizeY, &bmpH );
				WinDrawBitmap( bmpP, rect.topLeft.x - 1, rect.topLeft.y );
				ReleaseBitmap( &bmpH );	
				bmpH = NULL;
				bmpID = 0;
				
				WinPopDrawState();
			}
							
			handled = true;  
					
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // SignalLvlGadgetHandler()

/* 
 * RoamingGadgetHandler()
 */
Boolean RoamingGadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	RectangleType			rect = gadgetP->rect;
	MemHandle 				bmpH = NULL;
	BitmapType* 			bmpP = NULL;
	Coord					bmpSizeX = 0;
	Coord					bmpSizeY = 0;
	
	if ( gadgetP->id != CLOCK_GDT_ROAM ) return ( handled );
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:

			handled = true;

			break;
			
		case formGadgetEraseCmd:

			handled = false;

			break;		
		
		case formGadgetHandleEventCmd:
		case formGadgetDrawCmd:
	
			FrmSetActiveForm( pForm );
			
			gadgetP->attr.visible = true;
			
			WinPushDrawState();
						
			if ( IsRoaming() ) 
			{	
				bmpP = GetBitmap( CLOCK_BMP_ROAM, &bmpSizeX, &bmpSizeY, &bmpH );
				WinDrawBitmap( bmpP, rect.topLeft.x + ( ( rect.extent.x - bmpSizeX ) / 2 ), 
										rect.topLeft.y + ( ( rect.extent.y - bmpSizeY ) / 2 ) );
				ReleaseBitmap( &bmpH );	
				bmpH = NULL;					
			}
			else
			{
				WinEraseRectangle( &rect, 0 );
			}
				
			WinPopDrawState();
						
			handled = true; 
					
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // RoamingGadgetHandler()

/* 
 * CurrentOperatorHandler()
 */
Boolean CurrentOperatorHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	RectangleType			rect = gadgetP->rect;
	Char					CurrentOperator[STRLEN_OPERATOR] = "\0";
	
	if ( gadgetP->id != CLOCK_GDT_OPERATOR ) return ( handled );
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:

			handled = true;

			break;
			
		case formGadgetEraseCmd:

			handled = false;

			break;		
		
		case formGadgetHandleEventCmd:
		case formGadgetDrawCmd:
	
			FrmSetActiveForm( pForm );
			
			gadgetP->attr.visible = true;
			
			if ( !GetCurrentOperator( CurrentOperator, STRLEN_OPERATOR ) )
			{
				DrawStr( CurrentOperator, &rect, boldFont, hAlignLeft, vAlignMiddle, colorGreen, true );	
			}
						
			handled = true; 
					
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // CurrentOperatorHandler()

/*
 * CommonInitGauge
 */
void CommonInitGauge( FormPtr pForm )
{
	UInt32 			hsStatusVersion;
	UInt16 			libRefNum = sysInvalidRefNum;
	Err				error = errNone;
	
	if ( FtrGet( hsFtrCreator, hsFtrIDStatusGadgetRev, &hsStatusVersion ) == 0 )
	{
		HsStatusSetGadgetType( pForm, CLOCK_GDT_BATT, hsStatusGadgetBattery );
		
		HsStatusSetGadgetType( pForm, CLOCK_GDT_SIG, hsStatusGadgetSignal );
		
		error = SysLibFind( kPmSysGadgetLibName, &libRefNum );
		
		if ( error ) SysLibLoad( kPmSysGadgetLibType, kPmSysGadgetLibCreator, &libRefNum );
		
		PmSysGadgetLibOpen( libRefNum );
		PmSysGadgetStatusGadgetTypeSet( libRefNum, pForm, CLOCK_GDT_BT, pmSysGadgetStatusGadgetBt );
		PmSysGadgetLibClose( libRefNum );
		
		if ( error ) SysLibRemove( libRefNum );
	}
	
	return;
	
} // CommonInitGauge

/*
 * DrawBatteryLevel()
 */
void DrawBatteryLevel( FormType* pForm, RectangleType* rectP )
{
	UInt8					batteryPercent = 0;
	RectangleType			rect = *rectP;
	Boolean					bCharging = false;
	Int16					battRectHeight = 0;
	RGBColorType 			rgbGrey = { 0, 221, 221, 221 }; // { 0, 187, 187, 187 };
    RGBColorType 			rgbRed = { 0, 250, 0, 51 };
	IndexedColorType		color;
	RGBColorType 			rgbColorBG;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
		
	WinPushDrawState();
	
	UIColorGetTableEntryRGB( UIDialogFill, &( rgbColorBG ) );
	
	color = WinRGBToIndex( &rgbGrey );	
	WinSetForeColor( color );
	WinDrawRectangle( &rect, 0 );

	batteryPercent = (UInt16) GetBatteryState( &bCharging );

	battRectHeight = rect.extent.y; // save for later
		
	rect.extent.y -= ( ( rect.extent.y * batteryPercent) / 100 );
	
	WinSetBackColor( color );
	color = WinRGBToIndex( &rgbRed );	
	WinSetForeColor( color );
		
	if ( bCharging )
	{
		WinDrawGrayLine ( rect.topLeft.x, rect.topLeft.y, 
							rect.topLeft.x, rect.topLeft.y + rect.extent.y );
	}
	else
	{
		WinDrawRectangle( &rect, 0 );
	}
	
	color = WinRGBToIndex( &rgbColorBG );
	WinSetBackColor( color );	
	
	WinEraseLine( rect.topLeft.x, rect.topLeft.y + ( battRectHeight / 4 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( battRectHeight / 4 ) );

	WinEraseLine( rect.topLeft.x, rect.topLeft.y + ( battRectHeight / 2 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( battRectHeight / 2 ) );

	WinEraseLine( rect.topLeft.x, rectP->topLeft.y + ( 3 * battRectHeight / 4 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( 3 * battRectHeight / 4 ) );
	
	WinPopDrawState();
				
	return;	
	
} // DrawBatteryLevel()

/*
 * DrawMemBar()
 */
void DrawMemBar( FormType* pForm, UInt16 ctlID, RectangleType* rectP )
{
	UInt32					memUsed = 0;
	UInt32					memFree = 0;
	UInt32					memTotal = 0;
	RectangleType			rect = *rectP;
	Int16					barRectHeight = 0;
	RGBColorType 			rgbYellow = { 0, 255, 211, 182 }; // { 0, 255, 225, 109 }; // { 0, 187, 187, 187 };
    RGBColorType 			rgbGreen = { 0, 0, 153, 0 };
    RGBColorType 			rgbBlue = { 0, 0, 51, 200 };
	IndexedColorType		color;
	RGBColorType 			rgbColorBG;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
	
	WinPushDrawState();

	UIColorGetTableEntryRGB( UIDialogFill, &( rgbColorBG ) );

	color = WinRGBToIndex( &rgbYellow );
	WinSetForeColor( color );
	WinDrawRectangle( &rect, 0 );

	switch ( ctlID )
	{			
		case CLOCK_GDT_STORAGE_MEM:
			
			GetCardUsage( true, &memUsed, &memFree, &memTotal, NULL, NULL, NULL, NULL, NULL );
			color = WinRGBToIndex( &rgbGreen );	
			
			break;
			
		case CLOCK_GDT_DYNAMIC_HEAP:

			GetDynamicHeapUsage( &memUsed, &memFree, &memTotal, NULL, NULL, NULL, NULL, NULL );
			color = WinRGBToIndex( &rgbGreen );	
							
			break;
			
		case CLOCK_GDT_DB_CACHE:

			GetDBCacheUsage( &memUsed, &memFree, &memTotal, NULL, NULL, NULL, NULL, NULL );
			color = WinRGBToIndex( &rgbGreen );	
				
			break;
			
		case CLOCK_GDT_CARD:
	
			GetCardUsage( false, &memUsed, &memFree, &memTotal, NULL, NULL, NULL, NULL, NULL );
			color = WinRGBToIndex( &rgbBlue );	
			
			break;
			
		default:			
			break;	
	}
	
	barRectHeight = rect.extent.y; 
	
	if ( memTotal / 1048576 )
	{
		rect.extent.y -= ( ( memUsed / 1048576 ) * rect.extent.y ) / ( memTotal / 1048576 ); // 1048576 = 1024*1024
	}
	
	WinSetForeColor( color );
	WinDrawRectangle( &rect, 0 );
	
	color = WinRGBToIndex( &rgbColorBG );
	WinSetBackColor( color );	
	
	WinEraseLine( rect.topLeft.x, rect.topLeft.y + ( barRectHeight / 4 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( barRectHeight / 4 ) );

	WinEraseLine( rect.topLeft.x, rect.topLeft.y + ( barRectHeight / 2 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( barRectHeight / 2 ) );

	WinEraseLine( rect.topLeft.x, rect.topLeft.y + ( 3 * barRectHeight / 4 ),
					rect.topLeft.x + rect.extent.x, rect.topLeft.y + ( 3 * barRectHeight / 4 ) );
	
	WinPopDrawState();
				
	return;	
	
} // DrawMemBar()

/*
 * ClockGadgets.h
 */