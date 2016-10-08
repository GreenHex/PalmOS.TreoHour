/*
 * ChimeUtils.c
 */

#include "ChimeUtils.h"

// Prototypes
static chimeInterval_e 		GetChimeInterval( UInt32 timeSeconds );

/*
 * PlayChime()
 */
void PlayChime( UInt32 timeSeconds, sounds_t* pSnd, Boolean blocking )
{
	UInt16						toneIdx = 0;
	ToneIdentifier				toneID = { 0, 0 };
	ToneVolume					toneVol = toneVolume4;
	
	if ( pSnd->toneVolume == toneVolumeOff ) return;
	
	toneVol = ( pSnd->toneVolume > toneVolume7 ) ? GetSoundVolume() : pSnd->toneVolume;
	
	switch ( GetChimeInterval( timeSeconds ) )
	{
		case OneHour:
			
			toneIdx = pSnd->toneIdxHour;
			
			break;
			
		case ThreeQuarterHour:
		
			toneIdx = pSnd->toneIdxThreeQuarters;
			
			break;

		case HalfHour:
		
			toneIdx = pSnd->toneIdxHalfHouly;
			
			break;
			
		case QuarterHour:
		
			toneIdx = pSnd->toneIdxQuarterly;
			
			break;
			
		default:
			break;	
	}
	if ( !GetToneID( toneIdx, &toneID ) )
	{
		PlayTone( toneID, toneVol, blocking );
	}
	
	return;

} // PlayChime()

/*
 * LEDState()
 * set = true SETS the state
 * set = false GETS the state
 */
void LEDState( Boolean set, UInt16* countP, UInt16* stateP )
{
	if ( !set )
	{
		*countP = kIndicatorCountGetState;
		*stateP = kIndicatorStateNull;
	}
	
	HsIndicatorState( *countP, kIndicatorTypeLed, stateP );
	
	return;
		
} // LEDState()

/*
 * FlashLED()
 */
void FlashLED( Boolean blink, UInt8 BlinkType )
{
	UInt16 		count = kIndicatorCountForever;
	UInt16		onState[4] = { kIndicatorAlertAlert , kIndicatorForceRedGreen , kIndicatorForceRed , kIndicatorForceGreen } ;
	UInt16		offState[4] = { kIndicatorAlertNone , kIndicatorForceNone , kIndicatorForceNone , kIndicatorForceNone } ;
	UInt16		state = ( blink ) ? onState[BlinkType] : offState[BlinkType];
		
	HsIndicatorState( count, kIndicatorTypeLed, &state );
	
	return;
	
} // FlashLED()

/*
 * GetChimeInterval()
 */
static chimeInterval_e GetChimeInterval( UInt32 timeSeconds )
{
	
	DateTimeType		dt;
	chimeInterval_e		interval = Invalid;
	
	TimSecondsToDateTime( timeSeconds, &dt );
	
	switch ( dt.minute )
	{
		case 59:
		case 0:
		case 1:
		
			interval = OneHour;
			break;
			
		case 14:	
		case 15:
		case 16:
		
			interval = QuarterHour;
			break;
			
		case 29:
		case 30:
		case 31:
		
			interval = HalfHour;			
			break;
		
		case 44:	
		case 45:
		case 46:
		
			interval = ThreeQuarterHour;		
			break;
			
		default:
			interval = QuarterHour; // ExtraOptional
			break;	
	}

	return ( interval );
		
} // GetChimeInterval()

/*
 * Vibrate()
 */
void Vibrate( void )
{
	UInt16 				IndicatorState = kIndicatorAlertAlert;
	
	HsIndicatorState( 1, kIndicatorTypeVibrator, &IndicatorState );

	return;
	
} // Vibrate()

/*
 * ChimeUtils.c
 */