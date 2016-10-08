/*
 * DrawUtils.h
 */

#ifndef __DRAWUTILS_H__
#define __DRAWUTILS_H__

#include <Hs.h>
#include <HsExt.h>
#include <StringMgr.h>
#include <TimeMgr.h>
#include <Window.h>
#include <Form.h>
#include <Control.h>

#include "Utils.h"
#include "BigText.h"
#include "DateBook.h"
#include "Tasks.h"
#include "Helpers.h"
#include "../../Centroid/src/Bitmaps.h"

#define STRLEN_MISC					20
#define STRLEN_NEXT_APPT			32
#define STRLEN_DAY					4
#define STR_TEMPLATE_DAY			"^1r" // "Wed" // "^1l, "
#define STRLEN_DATE					3
#define STR_TEMPLATE_DATE			"^0r" // "9"
#define STRLEN_MONTH				4
#define STR_TEMPLATE_MONTH			"^2r" // "Jul"
#define STRLEN_YEAR					5
#define STR_TEMPLATE_YEAR			"^4l" // "2008"
#define STRLEN_MONTH_YEAR			8
#define STR_TEMPLATE_MONTH_YEAR		"^2r '^4s" // "Jul '08"

 // 
typedef enum {
	colorDefault		= 0,
	colorBlue 			= 0,
	colorRed			= 1,
	colorGreen 			= 2,
	colorGrey			= 3,
	colorBlack			= 4,
	colorWhite			= 5
} color_e;

typedef enum {
	hAlignLeft			= -1,
	hAlignCenter		= 0,
	hAlignRight			= 1
} hAlign_e;

typedef enum {
	vAlignTop			= -1,
	vAlignMiddle		= 0,
	vAlignBottom		= 1
} vAlign_e;

typedef struct {
	Char 				str[STRLEN_MISC];
	FontID				drawFont;
	color_e				drawColor;	
} drawStr_t;

typedef struct {
	drawStr_t 			alerts;
	drawStr_t 			sounds;
	drawStr_t 			msgs;
	drawStr_t 			mails;
	drawStr_t 			tasks;
	drawStr_t 			appts;
} gadgetsText_t;

/*
enum fontID {
  stdFont = 0x00, 
  boldFont, 
  largeFont, 
  symbolFont, 
  symbol11Font, 
  symbol7Font, 
  ledFont, 
  largeBoldFont, 
  fntAppCustomBase = 0x80
};
*/

// Prototypes
extern void 			DrawDate( FormType* pForm, RectangleType* rectP, DateType* dateP );
extern void 			DrawTime( FormType* pForm, RectangleType* rectP, DateTimeType* dtP );
extern void 			DrawSoundStatus( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP );
extern void 			DrawAlertCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP );
extern void 			DrawMsgsCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP );
extern void 			DrawEmailsCount( FormType* pForm, RectangleType* rectP, drawStr_t* drawStrP );
extern void 			DrawTasksCount( FormType* pForm, RectangleType* rectP, DateType* dateP, drawStr_t* drawStrP );
extern void 			DrawAppointmentsCount( FormType* pForm, RectangleType* rectP, DateType* dateP, TimeType* timeP, drawStr_t* drawStrP );
extern void 			DrawNextAppointment( FormType* pForm, RectangleType* rectP, DateType* dateP, TimeType* timeP );
extern void 			DrawUnlockText( FormType* pForm, RectangleType* rectP );
extern void 			DrawStr( Char* str, RectangleType* rectP, FontID fontID, hAlign_e hAlign, vAlign_e vAlign, color_e color, Boolean draw );
extern void 			ReadGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs );
extern void 			WriteGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs );
extern void 			InitializeGdtTxtPrefs( gadgetsText_t* pGdtTxtPrefs );

#endif /* __DRAWUTILS_H__ */
 
/*
 * DrawUtils.h
 */
 