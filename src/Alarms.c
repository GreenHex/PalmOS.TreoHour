/*
 * Alarms.c
 */
 
#include "Alarms.h"

void SetNextAlarm( void )
{
	UInt16 					cardNo;
	LocalID 				dbID;
	appPrefs_t				prefs;
	UInt32 					secsTimeNow = 0;
	DateTimeType			dt;
	UInt32					minsAlm = 0;
	UInt32					minsStart = 0;
	UInt32					minsEnd = 0;
	UInt32 					secsTimeAlarm = 0;	
	UInt32					secsDayStart = 0;
	UInt32 					secsChimeIntervalSecs = 0;
	UInt32					secsChimeInterval[4] = { CHIME_DISABLE, CHIME_QUARTER_HOUR, CHIME_HALF_HOUR, CHIME_ONE_HOUR };
	
	if ( SysCurAppDatabase( &cardNo, &dbID ) ) return;
	
	AlmSetAlarm( cardNo, dbID, 0, 0, false );
	
	ReadAppPrefs( &prefs );
	
	if ( ( !prefs.bEnabled ) || ( !prefs.chime.chimeIntervalIdx ) ) return;
	
	secsChimeIntervalSecs = secsChimeInterval[prefs.chime.chimeIntervalIdx];	

	secsTimeNow = TimGetSeconds();
	
	secsDayStart = secsTimeNow - ( secsTimeNow % SECONDS_IN_A_DAY );

	secsTimeAlarm = secsTimeNow - ( secsTimeNow % secsChimeIntervalSecs ) + secsChimeIntervalSecs;
	
	TimSecondsToDateTime( secsTimeAlarm, &dt );
	
	minsAlm = ( dt.hour * 60 ) + ( dt.minute );
	minsStart = ( prefs.chime.startTime.hours * 60 ) + prefs.chime.startTime.minutes;
	minsEnd = ( prefs.chime.endTime.hours * 60 ) + prefs.chime.endTime.minutes;
		
	if ( minsEnd < minsStart ) // alarm spans midnight
	{
		if ( ( minsAlm > minsEnd ) && ( minsAlm < minsStart ) ) // this is off period
		{
			secsTimeAlarm = secsDayStart + ( minsStart * 60 );
		}
	}
	else // normal interval
	{
		if ( minsAlm < minsStart ) // yet to start
		{
			secsTimeAlarm = secsDayStart + ( minsStart * 60 );
		}
		else if ( minsAlm > minsEnd ) // ended, go to next day
		{
			secsTimeAlarm = secsDayStart + ( minsStart * 60 ) + SECONDS_IN_A_DAY;
		}
	}
			
	AlmSetAlarm( cardNo, dbID, 0, secsTimeAlarm + 2, false ); /* bias of 2 seconds */
	
	return;
	 	
} // SetAlarm()

/*
 * Alarms.c
 */