/*
 * ClockGadgets.h
 */
 
#ifndef __CLOCKGADGETS_H__
#define __CLOCKGADGETS_H__

#include <Hs.h>
#include <Form.h>
#include <Window.h>
#include <AttentionMgr.h>
#include <SystemResources.h>
#include <palmOneCreators.h>
#include <PmSysGadgetLib.h>
#include <PmSysGadgetLibCommon.h>
#include <SoundMgr.h>

#include "Global.h"
#include "AppResources.h"
#include "Prefs.h"
#include "Utils.h"
#include "DrawUtils.h"
#include "DefaultHelpers.h"
#include "SignalLvl.h"
#include "MemUtils.h"
#include "../../Centroid/src/Bitmaps.h"
#include "../../TreoFlex2/src/FormUtils.h"

// Prototypes
extern void 				InitializeClockGadgets( FormType* pForm );
extern Boolean 				ClockGadgetsHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );
extern Boolean 				RoamingGadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );
extern Boolean 				SignalLvlGadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );
extern Boolean 				CurrentOperatorHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );
extern void 				CommonInitGauge( FormPtr pForm );

#endif /* __CLOCKGADGETS_H__ */
  
/*
 * ClockGadgets.h
 */