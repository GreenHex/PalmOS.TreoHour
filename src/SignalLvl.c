/*
 * SignalLvl.c
 */
 
#include "SignalLvl.h"

/*
 * GetSigLvl()
 */
Err GetSigLvl( UInt16* qualityP, Char* strSignalLvl )
{
	Err				error = errNone;
	UInt16 			phnLibRefNum = sysInvalidRefNum;
	UInt16			quality = 0;
	Char			strSig[maxStrIToALen] = "\0";
	Char			strSignal[maxStrIToALen] = "\0";
	
	if ( ( error = HsGetPhoneLibrary( &phnLibRefNum ) ) )
		return ( error );
					
	if ( ( error = PhnLibOpen( phnLibRefNum ) ) )
		return ( error );
		
	if ( PhnLibModulePowered( phnLibRefNum ) == phnPowerOn ) 	
	{		
		if ( PhnLibRegistered( phnLibRefNum ) )
		{
			if ( ( error = PhnLibSignalQuality( phnLibRefNum, &quality ) ) == errNone )
			{			
				if ( IsPhoneGSM() )
				{
					if ( quality < 32 )
					{
						StrIToA( strSignal, (UInt32) quality ); 
					}
					else if ( quality == 99 )
					{
						StrCopy( strSignal, "??" );
					}
					else
					{
						StrCopy( strSignal, "err" );
					}
				}
				else
				{
					StrIToA( strSig, (UInt32) quality ); 
					StrCopy( strSignal, "-" );
					StrCat( strSignal, strSig );
				}
			}
			else
			{
				quality = 100;
				StrCopy( strSignal, "!sig" );	
			}
		}
		else
		{
			quality = 101;
			StrCopy( strSignal, "!reg" );
		}
	}
	else
	{
		quality = 102;
		StrCopy( strSignal, "off" );
	}

	if ( qualityP ) *qualityP = quality;
	if ( strSignalLvl ) StrCopy( strSignalLvl, strSignal );
							
	PhnLibClose( phnLibRefNum );
	
	return ( error );
	
} // GetSigLvl()

/*
 * IsPhoneGSM()
 */
Boolean IsPhoneGSM( void )
{
  	UInt32 phnType = hsAttrPhoneTypeGSM; 
  
  	HsAttrGet( hsAttrPhoneType, 0, &phnType );
  	
  	return ( phnType == hsAttrPhoneTypeGSM );

} // IsPhoneGSM()

/*
 * SignalLvl.c
 */