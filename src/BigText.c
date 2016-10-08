/*
 * BigText.c
 */

#include "BigText.h"

// Prototypes
static MemHandle 			GetFontResource( DmResType resource, FontPtr *ppFont );
static void 				ReleaseFontResource( MemHandle hFont );

/*
 * DrawLargeTime()
 * 
 * 			Returns drawn screen height
 */
UInt16 DrawLargeTime( FontID fID, Char* strTime, RectangleType* rectP )
{
   	FontPtr					fontP;
   	MemHandle 				fontHandle;
   	FontID 					oldFontID;
   	UInt16					strWidth = 0;
    UInt16					strHeight = 0;
   
	WinPushDrawState();
	
	
   	if ( FntIsAppDefined( fID ) )
   	{
      	fontHandle = GetFontResource( HUGE_NUM_EXT_FONT, &fontP );
      	FntDefineFont( CUSTOM_FONT_ID, fontP );
   	}

   	oldFontID = FntSetFont( CUSTOM_FONT_ID );
   	
	strWidth = FntLineWidth( strTime, StrLen( strTime ) );
	strHeight = FntLineHeight();
   	
   	WinDrawChars( strTime, StrLen( strTime ),
   			rectP->topLeft.x + ( ( rectP->extent.x - strWidth ) / 2 ), 
   			rectP->topLeft.y + ( ( rectP->extent.y - strHeight ) / 2 ) ); 

   	if ( FntIsAppDefined( fID ) )
   	{
      	ReleaseFontResource( fontHandle );
   	}
   	 
   	FntSetFont( oldFontID );
   	
   	WinPopDrawState();
   		 	
    return ( rectP->topLeft.y + strHeight );

} // DrawLargeTime()

/*
 * GetFontResource()
 */
static MemHandle GetFontResource( DmResType resource, FontPtr *ppFont )
{
   	MemHandle 		hFont;
   	FontPtr 		pFont;

   	if ( ( hFont = DmGetResource( fontExtRscType, resource ) ) ) // fontRscType or fontExtRscType
   	{
      	if ( !( pFont = (FontPtr)MemHandleLock( hFont ) ) )
      	{ 
         	DmReleaseResource( hFont ); 
         	hFont = NULL;
      	}
 
      	if ( ppFont )
      	{
         	*ppFont = pFont;
      	}
   	}
   	
   	return ( hFont );

} // GetFontResource()

/*
 * ReleaseFontResource()
 */
static void ReleaseFontResource( MemHandle hFont )
{
	if ( hFont )
   	{
    	MemHandleUnlock( hFont );
		DmReleaseResource( hFont );
   	}
   	
   	return;

} // ReleaseFontResource()

/*
 * BigText.c
 */