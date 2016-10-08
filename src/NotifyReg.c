/*
 * NotifyReg.c
 */

#include "NotifyReg.h"

/*
 * RegisterForNotifications()
 */
void RegisterForNotifications( appPrefs_t* pPrefs )
{
	Err				error = errNone;	
	UInt16 			cardNo; 
	LocalID  		dbID;
	
	SysCurAppDatabase( &cardNo, &dbID );			

	if ( pPrefs->bEnabled )
	{
		SysNotifyRegister( cardNo, dbID, sysNotifyTimeChangeEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, pmNotifyBroadcastNewMsgCount, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyEarlyWakeupEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyVirtualCharHandlingEvent, NULL, sysNotifyNormalPriority, NULL );
		SysNotifyRegister( cardNo, dbID, sysNotifyGotUsersAttention, NULL, sysNotifyNormalPriority, NULL );
	}
	else
	{	
		SysNotifyUnregister( cardNo, dbID, sysNotifyTimeChangeEvent, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, pmNotifyBroadcastNewMsgCount, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyEarlyWakeupEvent, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyVirtualCharHandlingEvent, sysNotifyNormalPriority );
		SysNotifyUnregister( cardNo, dbID, sysNotifyGotUsersAttention, sysNotifyNormalPriority );
	}
	
	return;
	
} // RegisterForNotifications()

/*
 * NotifyReg.c
 */