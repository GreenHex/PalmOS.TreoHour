/*
 * Helpers.h
 */

#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <Hs.h>
#include <HsCreators.h>
#include <NotifyMgr.h>
#include <HsHelper.h>
#include <SysEvtMgr.h>
#include <Helper.h>
#include <HelperServiceClass.h>

#include "Global.h"
#include "Prefs.h"

#define COUNT_INVALID			-1

typedef struct {
	UInt16						EMail;
	UInt16						Fax;
	UInt16						SMS;
	UInt16						MMS;
	UInt16						VoiceDial;
} newMsgCount_t;

// Prototypes
extern void 					SendMsgCountNotifications( Boolean resetMsgCounts );
extern Boolean 					HandleHelperNotifications( PmBroadcastNewMsgCountNotifyParamType* notifyDetailsP );
extern Boolean 					NewMsgHelperNotify( void );
extern void 					ReadNewMsgPrefs( newMsgCount_t* pNewMsgPrefs );

#endif /* __HELPERS_H__ */

/*
 * Helpers.h
 */