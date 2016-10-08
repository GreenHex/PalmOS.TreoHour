/*
 * MemUtils.c
 */
 
#include "MemUtils.h"

// Prototypes
static Err 		GetMemUsage( UInt16 heapID, UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent );
static void 	MakeMemUsageStrings( UInt32 memUsed, UInt32 memFree, UInt32 memTotal, UInt32 memPercent, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent );

/*
 * GetDBCacheUsage()
 */
Err GetDBCacheUsage( UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent )
{
	return ( GetMemUsage( ( STORAGE_HEAP_ID | 0x8000 ), memUsedP, memFreeP, memTotalP, memPercentP, strMemUsed, strMemFree, strMemTotal, strMemPercent ) );

} // GetDBCacheUsage()

/*
 * GetDynamicHeapUsage()
 */
Err GetDynamicHeapUsage( UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent )
{
	return ( GetMemUsage( DYNAMIC_HEAP_ID, memUsedP, memFreeP, memTotalP, memPercentP, strMemUsed, strMemFree, strMemTotal, strMemPercent ) );
	
} // GetDynamicHeapUsage()

/*
 * GetMemUsage()
 */ 
static Err GetMemUsage( UInt16 heapID, UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent )
{
	Err						error = errNone;
	UInt32 					memUsed = 0;
	UInt32					memFree = 0;
   	UInt32 					memTotal = 0;
   	UInt32					memPercent = 0;
   	UInt32					memMaxFreeChunkSize = 0;

	memTotal = MemHeapSize( heapID );
	error = MemHeapFreeBytes( heapID, &memFree, &memMaxFreeChunkSize );
		
	if ( !error )
	{

		memUsed = memTotal - memFree;

		if ( memTotal)
		{
			memPercent = (UInt32) ( memUsed * 100 ) / memTotal;
		}
		
		if ( memUsedP )
		{
			*memUsedP = memUsed;	
		}
		
		if ( memFreeP )
		{
			*memFreeP = memFree;
		}
			
		if ( memTotalP )
		{
			*memTotalP = memTotal;	
		}
		
		if ( memPercentP )
		{
			*memPercentP = memPercent;	
		}

		MakeMemUsageStrings( memUsed, memFree, memTotal, memPercent, strMemUsed, strMemFree, strMemTotal, strMemPercent );

	}

	return ( error );
	
} // GetMemUsage()

/*
 * GetCardUsage()
 */
Err GetCardUsage( Boolean bInternalCard, UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent )
{
	Err						error = errNone;
	UInt16 					volRefNum = sysInvalidRefNum;
	UInt32 					volIterator = ( bInternalCard ) ? ( vfsIteratorStart | 0x80000000 ) : vfsIteratorStart;
	VolumeInfoType			volInfo;
	UInt32 					memUsed = 0;
	UInt32					memFree = 0;
   	UInt32 					memTotal = 0;
   	UInt32					memPercent = 0;

	while ( volIterator != vfsIteratorStop )
	{ 
		error = VFSVolumeEnumerate( &volRefNum, &volIterator );  
		
		if ( error == expErrEnumerationEmpty ) break;

		error = VFSVolumeInfo( volRefNum, &volInfo );
		
		if ( !error )
		{
			error = vfsErrNoFileSystem;

			if ( bInternalCard )
			{
				if ( ( volInfo.attributes & vfsVolumeAttrHidden )
			    		&& ( volInfo.attributes & vfsVolumeAttrNonRemovable ) )	
			    {
					error = errNone;
			    	break;	
			    }
			}
			else
			{
				if ( !( volInfo.attributes & vfsVolumeAttrNonRemovable ) )
				{
					error = errNone;
					break;
				}
			}
		}
	}

	if ( !error ) 
	{ 
		error = VFSVolumeSize( volRefNum, &memUsed, &memTotal );   								
	}

	if ( !error )
	{
		memFree = memTotal - memUsed;
		
		if ( memTotal )
		{
			memPercent = (UInt32) ( memUsed * 100 ) / memTotal;
		}
		
		if ( memUsedP )
		{
			*memUsedP = memUsed;	
		}
		
		if ( memFreeP )
		{
			*memFreeP = memFree;	
		}
		
		if ( memTotalP )
		{
			*memTotalP = memTotal;	
		}
		
		if ( memPercentP )
		{
			*memPercentP = memPercent;	
		}
			
		MakeMemUsageStrings( memUsed, memFree, memTotal, memPercent, strMemUsed, strMemFree, strMemTotal, strMemPercent );
	}

	return ( error );
	
} // GetCardUsage()

/*
 * MakeMemUsageStrings()
 */
static void MakeMemUsageStrings( UInt32 memUsed, UInt32 memFree, UInt32 memTotal, UInt32 memPercent, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent )
{

	if ( strMemUsed )
	{
		StrIToA( strMemUsed, (Int32) memUsed );
	}

	if ( strMemFree )
	{
		StrIToA( strMemFree, (Int32) memFree );
	}

	if ( strMemTotal )
	{
		StrIToA( strMemTotal, (Int32) memTotal );
	}

	if ( strMemPercent )
	{
		StrIToA( strMemPercent, (Int32) memPercent );
		StrCat( strMemPercent, "%" );
	}

	return;	

} // MakeMemUsageStrings()

/*
 * MemUtils.c
 */