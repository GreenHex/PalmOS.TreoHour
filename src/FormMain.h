/*
 * FormMain.h
 */
 
#ifndef __FORMMAIN_H__
#define __FORMMAIN_H__

#include <Hs.h>
#include <Form.h>
#include <Control.h>
#include <List.h>

#include "Global.h"
#include "AppResources.h"
#include "Gadgets.h"
#include "Prefs.h"
#include "Utils.h"
#include "NotifyReg.h"
#include "Alarms.h"
#include "Clock.h"
#include "FormSounds.h"
#include "FormAppPrefs.h"
#include "../../TreoFlex2/src/FormUtils.h"

#ifndef STRLEN_TIME
# define STRLEN_TIME 	10
#endif

// Proptypes
extern Boolean 					MainForm_HandleEvent( EventType* pEvent );

#endif /* __FORMMAIN_H__ */

/*
 * FormMain.h
 */