/*
 * Gadgets.h
 */
 
#ifndef __GADGETS_H__
#define __GADGETS_H__

#include <Hs.h>
#include <Form.h>
#include <Control.h>
#include <Window.h>

#include "Global.h"
#include "AppResources.h"
#include "../../Centroid/src/Bitmaps.h"

typedef enum {
	btnInvalid			= 0,
	btnChime			= MAIN_BTN_CHIME,
	btnPopup			= MAIN_BTN_POPUP,
	btnKeyguard			= MAIN_BTN_KEYGUARD
} btn_e;

#define X_POS_CHIME			0
#define X_POS_POPUP			53
#define X_POS_KEYGUARD		106

// Prototypes
extern void 				InitializeMainFormGadgets( FormType* pForm, btn_e* btnP );
extern Boolean				MainForm_GadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP );
extern void 				UpdateGadgets( FormType* pForm, btn_e* btnP );

#endif /* __GADGETS_H__ */
  
/*
 * Gadgets.h
 */