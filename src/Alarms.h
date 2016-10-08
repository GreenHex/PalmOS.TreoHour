/*
 * Alarms.h
 */

#ifndef __ALARMS_H__
#define __ALARMS_H__

#include <Hs.h>
#include <TimeMgr.h>
#include <AlarmMgr.h>

#include "Utils.h"
#include "Prefs.h"

#define SECONDS_IN_A_DAY			86400 // 24 * 60 * 60
#define CHIME_DISABLE				0
#define CHIME_QUARTER_HOUR			900 // 60 * 15
#define CHIME_HALF_HOUR				1800
#define CHIME_ONE_HOUR				3600

// Prototypes
extern void 				SetNextAlarm( void );

#endif /* __ALARMS_H__ */
 
/*
 * Alarms.h
 */