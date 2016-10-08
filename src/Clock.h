/*
 * Clock.h
 */
 
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include <Hs.h>
#include <Form.h>
#include <Control.h>
#include <List.h>
#include <PmKeyLibCommon.h>
#include <PmKeyLib.h>
#include <SoundMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "Prefs.h"
#include "Utils.h"
#include "DrawUtils.h"
#include "ChimeUtils.h"
#include "../../TreoFlex2/src/FormUtils.h"

typedef enum {
	exitTimedout			= 0,	
	exitUnlocked			= 1
} LoopExitType_e;

// Proptypes
extern LoopExitType_e 		ShowClock( clockState_e newState, UInt32 secondsNow );

#endif /* __CLOCK_H__ */

/*
 * Clock.h
 */