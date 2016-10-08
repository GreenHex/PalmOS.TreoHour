/*
 * AppResources.h
 */

#ifndef __APPRESOURCES_H__
#define __APPRESOURCES_H__

// Main Form
#define MAIN_FORM						1000
#define MAIN_BTN_DONE					1010
#define MAIN_CBX_ENABLE					1020
#define MAIN_BTN_CHIME					1030
#define MAIN_BTN_POPUP					1040
#define MAIN_BTN_KEYGUARD				1050
#define MAIN_GDT						1060
#define MAIN_BTN_TEST					1070

#define CHM_LBL							2010 
#define CHM_POP_INTERVAL				2020
#define CHM_LST_INTERVAL				2030
#define CHM_LBL_BETWEEN					2040
#define CHM_TRG_FROM					2050
#define CHM_LBL_AND_BETWEEN				2053
#define CHM_TRG_TO						2055
#define CHM_LBL_DISP_CLK				2060
#define CHM_POP_DISP_CLK				2070
#define CHM_LST_DISP_CLK				2080
#define CHM_CBX_KILL					2090
#define CHM_LBL_BEEP					2100
#define CHM_POP_BEEP					2110
#define CHM_LST_BEEP					2120
#define CHM_BTN_CONFIG_SND				2130
#define CHM_LBL_LED						2190
#define CHM_POP_LED						2200
#define CHM_LST_LED						2210
#define CHM_CBX_VIB						2220

#define POP_CBX_ENABLE					3010
#define POP_LBL_KEY						3020
#define POP_POP_KEY						3030
#define POP_LST_KEY						3040
#define POP_LBL_MOD						3050
#define POP_POP_MOD						3060
#define POP_LST_MOD						3070
#define POP_LBL_TIMEOUT					3080
#define POP_POP_TIMEOUT					3090
#define POP_LST_TIMEOUT					3100
#define POP_CBX_ALLOW_KEY				3110
#define POP_LBL_L1						3120
#define POP_LBL_L2						3130

#define KG_CBX_ENABLE					4010
#define KG_LBL_NOTE						4020
#define KG_LBL_KEY						4030
#define KG_POP_KEY						4040
#define KG_LST_KEY						4050
#define KG_LBL_MOD						4060
#define KG_POP_MOD						4070
#define KG_LST_MOD						4080
#define KG_LBL_TIMEOUT					4090
#define KG_POP_TIMEOUT					4100
#define KG_LST_TIMEOUT					4110
#define KG_CBX_NO_CLK_ON_SMS			4120
#define KG_LBL_L1						4130

// Bitmaps
#define MAIN_BMP_TAB_BG					1500
#define MAIN_BMP_TAB					1600
#define CLOCK_BMP_RSSI					1900
#define CLOCK_BMP_ROAM					2000
#define CLOCK_BMP_DOW					2100
#define CLOCK_BMP_LOCK					2200

// Main Menu
#define MAIN_MENU						1000
#define MAIN_MENU_EXIT					1110
#define MAIN_MENU_APPPREFS				1210
#define MAIN_MENU_HELP					1220
#define MAIN_MENU_ABOUT					1230

// Strings
#define STRING_MAIN_HELP				1000
	
// About Form
#define ABOUT_FORM						5000
#define ABOUT_BTN_DONE					5010
#define ABOUT_LBL_VERSION				5020

// Clock Form
#define CLOCK_FORM						6000
#define CLOCK_GDT_ALERTS				6010
#define CLOCK_GDT_SOUND					6020
#define CLOCK_GDT_MSGS					6030
#define CLOCK_GDT_MAILS					6040
#define CLOCK_GDT_TASKS					6050
#define CLOCK_GDT_APPT					6060
#define CLOCK_GDT_NEXT_APPT				6070
#define CLOCK_GDT_DATE					6080
#define CLOCK_GDT_TIME					6090
#define CLOCK_GDT_BATTERY				6100
#define CLOCK_PBT_EXIT					6110
#define CLOCK_GDT_BATT					6120
#define CLOCK_GDT_SIG					6130
#define CLOCK_GDT_BT					6140
#define CLOCK_GDT_RSSI					6150
#define CLOCK_GDT_ROAM					6160
#define CLOCK_GDT_OPERATOR				6170
#define CLOCK_GDT_STORAGE_MEM			6180
#define CLOCK_GDT_DYNAMIC_HEAP			6190
#define CLOCK_GDT_DB_CACHE				6200
#define CLOCK_GDT_CARD					6210

// Sounds Form
#define SND_FORM						7000
#define SND_BTN_DONE					7010
#define SND_BTN_CANCEL					7020
#define SND_POP_HOUR					7030
#define SND_POP_THREEQUARTER			7040
#define SND_POP_HALFHOUR				7050
#define SND_POP_QUARTER					7060
#define SND_LST_SOUNDS					7070

// App Preferences
#define APPPREFS_FORM					8000
#define APPPREFS_BTN_DOME				8010
#define APPPREFS_BTN_CANCEL				8020
#define APPPREFS_CBX_ALLOW_APP_LAUNCH	8030
#define APPPREFS_CBX_DONT_DISP_KG_TXT	8040

#endif /* __APPRESOURCES_H__ */

/*
 * <Form.h>
 * 
 * FrmCustomAlert ( InfoOKAlert, "", "", "" );
 * UInt16 FrmCustomAlert ( UInt16 alertId, const Char *s1, const Char *s2, const Char *s3 ) 
 * 
#define ErrOKAlert							10021		// Error Alert with just an OK button
#define ErrOKCancelAlert					10022		// Error Alert with an OK & Cancel button
#define ErrCancelAlert						10023		// Error Alert with just Cancel button.  Special case for antenna down alert.
#define InfoOKAlert							10024		// Info alert with just an OK button
#define InfoOKCancelAlert					10025		// Info alert with an OK & Cancel button
#define InfoCancelAlert						10026		// Info alert with just a Cancel button
#define PrivacyWarningAlert					10027    	// Privacy warning for weblib
#define ConfirmationOKAlert					10028		// Confirmation alert with just an OK button
#define ConfirmationOKCancelAlert			10029		// Confirmation alert with an OK & Cancel button
#define ConfirmationCancelAlert				10030		// Confirmation alert with just a Cancel button
#define WarningOKAlert						10031		// Warning Alert with just an OK button
#define WarningOKCancelAlert				10032		// Warning Alert with an OK & Cancel button
#define WarningCancelAlert					10033		// Warning Alert with just Cancel button.  Special case for antenna down alert.
*/

/*
 * <SoundMgr.h>
 * 
 * SndPlaySystemSound()
 * 
 * typedef enum SndSysBeepTag {
 * sndInfo = 1,
 * sndWarning,
 * sndError,
 * sndStartUp,
 * sndAlarm,
 * sndConfirmation,
 * sndClick
 * }
 */


/*
 * AppResources.h
 */
