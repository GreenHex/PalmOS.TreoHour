/*
 * DefaultHelpers.c
 */

#include "DefaultHelpers.h"
 
/*
 * GetDefaultHelper()
 */
UInt32 GetDefaultHelper( UInt32 serviceID )
{
	UInt32			defaultHelperCreatorID = 0;
	UInt16			defaultHelperLibRefNum = sysInvalidRefNum;
	Boolean			defaultHelperLibLoaded = false;
		
	if ( SysLibFind( defaultHelperLibName, &defaultHelperLibRefNum  ) )
	{
		defaultHelperLibLoaded = ( ( SysLibLoad( defaultHelperLibType, defaultHelperLibCreator, &defaultHelperLibRefNum  ) == errNone )
				&& ( defaultHelperLibRefNum != sysInvalidRefNum ) );
	}
	
	if ( ! DefaultHelperLibOpen( defaultHelperLibRefNum ) )
	{
		DefaultHelperLibGetDefaultHelper( defaultHelperLibRefNum, serviceID, &defaultHelperCreatorID );
									  
		DefaultHelperLibClose( defaultHelperLibRefNum );
	}
	
	if ( defaultHelperLibLoaded ) 
	{
		SysLibRemove( defaultHelperLibRefNum );
	}
	
	return ( defaultHelperCreatorID );
		
} // GetDefaultHelper()

/*
 * DefaultHelpers.c
 */