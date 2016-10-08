/*
 * MemUtils.h
 */

#ifndef __MEMUTILS_H__
#define __MEMUTILS_H__
 
#include <Hs.h>
#include <StringMgr.h>
#include <VfsMgr.h>
#include <PmPalmOSNVFS.h>
#include <MemoryMgr.h>

#define DYNAMIC_HEAP_ID					0
#define STORAGE_HEAP_ID					1

// Prototypes
extern Err 			GetDBCacheUsage( UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent );
extern Err 			GetDynamicHeapUsage( UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent );
extern Err 			GetCardUsage( Boolean bInternalCard, UInt32* memUsedP, UInt32* memFreeP, UInt32* memTotalP, UInt32* memPercentP, Char* strMemUsed, Char* strMemFree, Char* strMemTotal, Char* strMemPercent );

#endif /* __MEMUTILS_H__ */

/*
 * MemUtils.h
 */