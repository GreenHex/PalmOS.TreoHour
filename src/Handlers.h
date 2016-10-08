/*
 * Handlers.h
 */
 
#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <Hs.h>

#include "Global.h"
#include "Prefs.h"
#include "Utils.h"
#include "NotifyReg.h"
#include "Alarms.h"
#include "Clock.h"
#include "Helpers.h"
#include "ChimeUtils.h"

// Prototypes
extern Err				HandleNotifications( SysNotifyParamType* notifyParamP );
extern void 			HandleAlarmTriggered( void );
extern void 			HandleAlarmDisplay( void );
extern void 			HandleReset( void );

#endif /* __HANDLERS_H__ */

/*
 * Handlers.h
 */