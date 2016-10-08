/*
 * DrawUtils.c
 */

#include "DrawUtils.h"

// Prototypes

/*
 * DrawDate()
 */
void DrawDate( FormType* pForm, RectangleType* rectP, DateType* dateP )
{
	Char					strDay[STRLEN_DAY] = "\0";
	Char					strDate[STRLEN_DATE] = "\0";
	Char					strMonth[STRLEN_MONTH] = "\0";
	Char					strYear[STRLEN_YEAR] = "\0";
	Char					strMonthYear[STRLEN_MONTH_YEAR] = "\0";
	Int16					dayOfWeek = DayOfWeek( (Int16) dateP->month, (Int16) dateP->day, (Int16) dateP->year + 1904 );
	Int16					strHeight = 0;
	Int16					strWidth = 0;
	RGBColorType 			rgbLightYellow = { 0, 255, 255, 204 };
	RGBColorType 			rgbRed = { 0, 250, 0, 51 };	
	RGBColorType 			rgbGreen = { 0, 0, 153, 0 };
    RGBColorType 			rgbBlue = { 0, 0, 51, 200 };
    RGBColorType 			rgbGrey = { 0, 85, 85, 85 };
    RGBColorType 			rgbBlack = { 0, 0, 0, 0 };
    RGBColorType 			rgbWhite = { 0, 255, 255, 255 };
    IndexedColorType		color;
    MemHandle 				bmpH = NULL;
	BitmapType* 			bmpP = NULL;
	Coord					bmpSizeX = 0;
	Coord					bmpSizeY = 0;
	
	if ( ( !pForm ) || ( !rectP ) || ( !dateP ) ) return;
	
	DateTemplateToAscii( STR_TEMPLATE_DAY, dateP->month, dateP->day, dateP->year + 1904, strDay, STRLEN_DAY );
    DateTemplateToAscii( STR_TEMPLATE_DATE, dateP->month, dateP->day, dateP->year + 1904, strDate, STRLEN_DATE );
    DateTemplateToAscii( STR_TEMPLATE_MONTH, dateP->month, dateP->day, dateP->year + 1904, strMonth, STRLEN_MONTH );
    DateTemplateToAscii( STR_TEMPLATE_YEAR, dateP->month, dateP->day, dateP->year + 1904, strYear, STRLEN_YEAR );
    DateTemplateToAscii( STR_TEMPLATE_MONTH_YEAR, dateP->month, dateP->day, dateP->year + 1904, strMonthYear, STRLEN_MONTH_YEAR );
    
    WinPushDrawState();
    
    color = WinRGBToIndex( &rgbGrey );	
    WinSetTextColor( color );
	FntSetFont( boldFont );
	strHeight = FntLineHeight();
	strWidth = FntLineWidth( strDay, StrLen( strDay ) );
 	WinDrawChars( strDay, StrLen( strDay ), rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 ), rectP->topLeft.y + 7 );
 
 	color = WinRGBToIndex( &rgbBlue );	
    WinSetTextColor( color );
	FntSetFont( ledFont );
 	strHeight = FntLineHeight();
	strWidth = FntLineWidth( strDate, StrLen( strDate ) );
 	WinDrawChars( strDate, StrLen( strDate ), rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 ), rectP->topLeft.y + 17 );

	color = WinRGBToIndex( &rgbGrey );	
    WinSetTextColor( color );
 	FntSetFont( boldFont );
 	strHeight = FntLineHeight();
	strWidth = FntLineWidth( strMonthYear, StrLen( strMonthYear ) );
 	WinDrawChars( strMonthYear, StrLen( strMonthYear ), rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 ), rectP->topLeft.y + 34 );

	bmpP = GetBitmap( CLOCK_BMP_DOW, &bmpSizeX, &bmpSizeY, &bmpH );
	WinDrawBitmap( bmpP, rectP->topLeft.x - 4, rectP->topLeft.y + 2 );
	ReleaseBitmap( &bmpH );	
	bmpH = NULL;
	
	color = WinRGBToIndex( &rgbRed );
	WinSetForeColor( color );
	WinDrawLine( rectP->topLeft.x - 4, rectP->topLeft.y + 2 + ( 7 * dayOfWeek ) ,
						rectP->topLeft.x - 4, rectP->topLeft.y + 7 + ( 7 * dayOfWeek ) );	
		
/*
 *  	
 	FntSetFont( stdFont );
 	strHeight = FntLineHeight();
	strWidth = FntLineWidth( strYear, StrLen( strYear ) );
 	WinDrawChars( strYear, StrLen( strYear ), rectP->topLeft.x + ( rectP->extent.x - strWidth ), rectP->topLeft.y + 34 );
 */   
    WinPopDrawState();
    
	return;
	
} // DrawDate()

/*
 * DrawTime()
 */
void DrawTime( FormType* pForm, RectangleType* rectP, DateTimeType* dtP )
{
	Char				strTime[timeStringLength] = "\0";
	
	if ( ( !pForm ) || ( !rectP ) || ( !dtP ) ) return;
	
	TimeToAscii( dtP->hour, dtP->minute, PrefGetPreference( prefTimeFormat ), strTime );
	ReplaceChar( strTime, 'm', chrNull );
	StripSpaces( strTime );
	DrawLargeTime( fntAppFontCustomBase, strTime, rectP );
	
	return;
	
} // DrawTime()

/*
 * DrawAlertCount()
 */
void DrawAlertCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP )
{
	UInt16				count = 0;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
	
	count = AttnGetCounts( 0, 0, NULL, NULL );
					
    if ( count )
    {
    	StrPrintF( drawStrP->str, ( ( count == 1 ) ? "%u alert" : "%u alerts" ), count );
    	drawStrP->drawFont = boldFont;
    	drawStrP->drawColor = colorRed;
    }
    else
    {
    	StrCopy( drawStrP->str, "no alerts" );
    	drawStrP->drawFont = stdFont;
    	drawStrP->drawColor = colorGrey;
    }
	
	DrawStr( drawStrP->str, rectP,drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );
	
	return;
	
} // DrawAlertCount()

/*
 * DrawSoundStatus()
 */
void DrawSoundStatus( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP )
{
	UInt16				muteSwitchPosition = hsAttrRingSwitchMute;
	UInt16				volume = 0;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
					
   	if ( HsAttrGet( hsAttr68KRingSwitch, 0, (UInt32 *) &muteSwitchPosition ) == errNone )
	{
		if ( muteSwitchPosition == hsAttrRingSwitchMute )
		{
			StrCopy( drawStrP->str, "ringer OFF" );
			drawStrP->drawFont = boldFont;
    		drawStrP->drawColor = colorRed;	
		}
		else // hsAttrRingSwitchLoud
		{
			volume = GetPhoneVolume( NULL, NULL );
			
			StrPrintF( drawStrP->str, "volume %u", volume );
			
			if ( volume < toneVolume2 ) // toneVolumeOff
			{
				drawStrP->drawFont = boldFont;
    			drawStrP->drawColor = colorRed;
			}
			else
			{	
				drawStrP->drawFont = stdFont;
    			drawStrP->drawColor = colorGrey;
			}
		}
		
		DrawStr( drawStrP->str, rectP, drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );	
	}
	
	return;
	
} // DrawSoundStatus()

/*
 * DrawTasksCount()
 */
void DrawTasksCount( FormType* pForm, RectangleType* rectP, DateType* dateP, drawStr_t* drawStrP )
{
	UInt16				count = 0;
	
	if ( ( !pForm ) || ( !rectP ) || ( !dateP ) ) return;
	
	count = GetNumTasks( dateP );
					
    if ( count )
    {
    	StrPrintF( drawStrP->str, ( (count == 1 ) ? "%u task" : "%u tasks" ), count );
    	drawStrP->drawFont = boldFont;
    	drawStrP->drawColor = colorRed;
    }
    else
    {
    	StrCopy( drawStrP->str, "no tasks" );
    	drawStrP->drawFont = stdFont;
    	drawStrP->drawColor = colorGrey;
    }
	
	DrawStr( drawStrP->str, rectP, drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );
	
	return;
	
} // DrawTasksCount()


/*
 * DrawAppointmentsCount()
 */
void DrawAppointmentsCount( FormType* pForm, RectangleType* rectP, DateType* dateP, TimeType* timeP, drawStr_t* drawStrP )
{
	UInt16				count = 0;
	
	if ( ( !pForm ) || ( !rectP ) || ( !dateP ) || ( !timeP ) ) return;
	
	count = GetNumAppointments( dateP, timeP, NULL, 0, NULL );
					
    if ( count )
    {
    	StrPrintF( drawStrP->str, ( ( count == 1 ) ? "%u appt" : "%u appts" ), count );
    	drawStrP->drawFont = boldFont;
    	drawStrP->drawColor = colorRed;
    }
    else
    {
    	StrCopy( drawStrP->str, "no appts" );
    	drawStrP->drawFont = stdFont;
    	drawStrP->drawColor = colorGrey;
    }
	
	DrawStr( drawStrP->str, rectP, drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );
	
	return;
	
} // DrawAppointmentsCount()

/*
 * DrawNextAppointment()
 */
void DrawNextAppointment( FormType* pForm, RectangleType* rectP, DateType* dateP, TimeType* timeP )
{
	UInt16				count = 0;
	Char				str[STRLEN_MISC] = "\0";
	FontID				drawFont = stdFont;
	color_e				drawColor = colorDefault;
	Char				strNextApptDesc[STRLEN_NEXT_APPT] = "\0";
	Char				strNextApptTime[STRLEN_MISC] = "\0";
	Char				strNextAppt[STRLEN_NEXT_APPT + STRLEN_MISC] = "\0";
	
	if ( ( !pForm ) || ( !rectP ) || ( !dateP ) || ( !timeP ) ) return;
	
	count = GetNumAppointments( dateP, timeP, strNextApptDesc, STRLEN_NEXT_APPT, strNextApptTime );
					
	if ( StrLen( strNextApptDesc ) )
	{
		StrCopy( strNextAppt, strNextApptTime );
		StrCat( strNextAppt, strNextApptDesc );
		drawFont = stdFont;
    	drawColor = colorBlue;
	}
	else
	{
		StrCopy( strNextAppt, "no further appointments" );
		drawFont = stdFont;
    	drawColor = colorGrey;
	}
	
	DrawStr( strNextAppt, rectP, drawFont, hAlignCenter, vAlignMiddle, drawColor, true );
	
	return;
	
} // DrawNextAppointment()

/*
 * DrawMsgsCount()
 */
void DrawMsgsCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP )
{
	newMsgCount_t		newMsgCounts;
	UInt16				countMsgs = 0;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
	
	ReadNewMsgPrefs( &newMsgCounts );
	
	countMsgs = ( newMsgCounts.SMS == COUNT_INVALID ) ? 0 : newMsgCounts.SMS;
	countMsgs += ( newMsgCounts.MMS == COUNT_INVALID ) ? 0 : newMsgCounts.MMS;
					
    if ( countMsgs )
    {
    	StrPrintF( drawStrP->str, ( ( countMsgs == 1 ) ? "%u msg" : "%u msgs" ), countMsgs );
    	drawStrP->drawFont = boldFont;
    	drawStrP->drawColor = colorRed;
    }
    else
    {
    	StrCopy( drawStrP->str, "no msgs" );
    	drawStrP->drawFont = stdFont;
    	drawStrP->drawColor = colorGrey;
    }
	
	DrawStr( drawStrP->str, rectP, drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );
	
	return;
	
} // DrawMsgsCount()

/*
 * DrawEmailsCount()
 */
void DrawEmailsCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP )
{
	newMsgCount_t		newMsgCounts;
	UInt16				countEmails = 0;
	
	if ( ( !pForm ) || ( !rectP ) ) return;
	
	ReadNewMsgPrefs( &newMsgCounts );
	
	countEmails = ( newMsgCounts.EMail == COUNT_INVALID ) ? 0 : newMsgCounts.EMail;
					
	if ( countEmails )
    {
    	StrPrintF( drawStrP->str, ( ( countEmails == 1 ) ? "%u email" : "%u emails" ), countEmails );
    	drawStrP->drawFont = boldFont;
    	drawStrP->drawColor = colorRed;
    }
    else
    {
    	StrCopy( drawStrP->str, "no emails" );
    	drawStrP->drawFont = stdFont;
    	drawStrP->drawColor = colorGrey;
    }
	
	DrawStr( drawStrP->str, rectP, drawStrP->drawFont, hAlignCenter, vAlignMiddle, drawStrP->drawColor, true );
	
	return;
	
} // DrawEmailsCount()

/* 
 * DrawUnlockText()
 */
void DrawUnlockText( FormType* pForm, RectangleType* rectP )
{
	RectangleType			rect = *rectP;
	appPrefs_t				prefs;
	Char					str[STRLEN_NEXT_APPT] = "\0";
	RGBColorType 			rgbRed = { 0, 250, 0, 51 };
    RGBColorType 			rgbWhite = { 0, 255, 255, 255 };
	IndexedColorType		color;
	 
	if ( ( !pForm ) || ( !rectP ) ) return;
	
	ReadAppPrefs( &prefs );
		
	if ( prefs.keyguard.unlockKeyIdx )
	{	
		if ( StrLen( prefs.keyguard.unlockModifierKeyName ) )
		{
			StrCopy(str, prefs.keyguard.unlockModifierKeyName );
		}				
		if ( StrLen( prefs.keyguard.unlockKeyName ) )
		{
			StrCat( str, prefs.keyguard.unlockKeyName );
		}
	}
	else
	{
		StrCopy( str, "Tap to dismiss" );
	}
	
	WinPushDrawState();
	
	color = WinRGBToIndex( &rgbRed );	
	WinSetForeColor( color );

	rect.extent.x -= rect.extent.y / 2 ;
	rect.topLeft.x += rect.extent.y / 4;
	
	WinDrawRectangle( &rect, rect.extent.y / 2 );

	WinSetBackColor( color );
	
	rect.extent.x -= rect.extent.y;
	rect.topLeft.x += rect.extent.y / 2;
	
	DrawStr( str, &rect, stdFont, hAlignCenter, vAlignMiddle, colorWhite, true );
	
	WinPopDrawState();
	
	return;
	
} // DrawUnlockText()
 
/*
 * DrawStr()
 */
void DrawStr( Char* str, RectangleType* rectP, FontID fontID, hAlign_e hAlign, vAlign_e vAlign, color_e color, Boolean draw )
{
	Coord 					posX = 0;
	Coord 					posY = 0;
	Int16					strHeight = 0;
	Int16					strWidth = 0;	
    RGBColorType 			rgb;
    RGBColorType 			rgbRed = { 0, 250, 0, 51 };
    RGBColorType 			rgbGreen = { 0, 0, 153, 0 };
    RGBColorType 			rgbBlue = { 0, 0, 51, 200 };
    RGBColorType 			rgbGrey = { 0, 187, 187, 187 };
    RGBColorType 			rgbBlack = { 0, 0, 0, 0 };
    RGBColorType 			rgbWhite = { 0, 255, 255, 255 };
    IndexedColorType		strColor;
	
	if ( ( !str ) || ( !rectP ) ) return;
		
	WinPushDrawState();
	
	WinEraseRectangle( rectP, 0 ); 
	
	if ( draw )
	{
		switch ( color )
		{
			case colorRed:
			
				rgb = rgbRed;
	
				break;
				
			case colorGreen:
	
				rgb = rgbGreen;
			
				break;
 
			case colorGrey:
			
				rgb = rgbGrey;
	
				break;
				
			case colorBlack:
			
				rgb = rgbBlack;
	
				break;
			
			case colorWhite:
			
				rgb = rgbWhite;
	
				break;
				
			default:
			
				rgb = rgbBlue;
				
				break;		
		}
		
		strColor = WinRGBToIndex( &rgb );	
	    WinSetTextColor( strColor );
	    
		FntSetFont( fontID );
		strHeight = FntLineHeight();
		// strWidth = FntLineWidth( str, StrLen( str ) );
		FntWidthToOffset( str, StrLen( str ), rectP->extent.x, NULL, &strWidth );
		
		switch ( hAlign )
		{
			case hAlignLeft:
				posX = rectP->topLeft.x;		
				break;
			case hAlignRight:
				posX = rectP->topLeft.x + ( rectP->extent.x - strWidth );
				break;
			default:
				posX = rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 );
				break;
		}
		
		switch ( vAlign )
		{
			case vAlignTop:
				posY = rectP->topLeft.y;
				break;
			case vAlignBottom:
				posY = rectP->topLeft.y + ( rectP->extent.y - strHeight );
				break;
			default:
				posY = rectP->topLeft.y + ( ( rectP->extent.y - strHeight ) / 2 );
				break;
		}
		
		// WinDrawChars( str, StrLen( str ), posX, posY );
		WinDrawTruncChars( str, StrLen( str ), posX, posY, rectP->extent.x );
	}
		    
	WinPopDrawState();
	
	return;
	
} // DrawStr()

/*
 * WriteGdtTxtPrefs()
 */
void WriteGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs )
{
	if ( !pGdtTxtPrefs ) return;
	
	PrefSetAppPreferences( gdtTxtPrefCreator, gdtTxtPrefID, gdtTxtPrefVersionNum, pGdtTxtPrefs, sizeof( gadgetsText_t ), gdtTxtPrefType );
	
	return;
	
} // WriteGdtTxtPrefs()

/*
 * ReadGdtTxtPrefs()
 */
void ReadGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs )
{
	UInt16 			prefSize = sizeof( gadgetsText_t );
	UInt16			prefsVersion = 0;

	if ( !pGdtTxtPrefs ) return;
		
	prefsVersion = PrefGetAppPreferences( gdtTxtPrefCreator, gdtTxtPrefID, pGdtTxtPrefs, &prefSize, gdtTxtPrefType );
	
	if ( prefsVersion != gdtTxtPrefVersionNum ) // remove old prefs...
	{
		PrefSetAppPreferences( gdtTxtPrefCreator, gdtTxtPrefID, prefsVersion, NULL, 0, gdtTxtPrefType );				
	
		InitializeGdtTxtPrefs( pGdtTxtPrefs );
		
		WriteGdtTxtPrefs( pGdtTxtPrefs );
	}
	
	return;
	
} // ReadGdtTxtPrefs()

/*
 * InitializeGdtTxtPrefs()
 */
void InitializeGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs )
{
	if ( !pGdtTxtPrefs ) return;
	
	MemSet( pGdtTxtPrefs, sizeof( gadgetsText_t ), 0 );
	
	return;

} // InitializeGdtTxtPrefs()
 
/*
 * DrawUtils.c
 */