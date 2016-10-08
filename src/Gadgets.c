/*
 * Gadgets.c
 */
 
#include "Gadgets.h"

#define NUM_CTL_TAB_1 		15
#define NUM_CTL_TAB_2 		10
#define NUM_CTL_TAB_3 		10

static void 				SetCtlVisibility( FormType* pForm, UInt16 ctlID, Boolean bVisible );
static void 				SetTabVisibility( FormType* pForm, btn_e btn );

/*
 * SetCtlVisibility()
 */
static void SetCtlVisibility( FormType* pForm, UInt16 ctlID, Boolean bVisible )
{
	UInt16				objIdx = FrmGetObjectIndex( pForm, ctlID );

	if ( objIdx == frmInvalidObjectId ) return;
		
	if ( bVisible )
	{
		FrmShowObject( pForm, objIdx );
	}
	else
	{
		FrmHideObject( pForm, objIdx );
	}
			
	return;
	
} // SetCtlVisibility()

/*
 * SetTabVisibility()
 */
static void SetTabVisibility( FormType* pForm, btn_e btn )
{
	static btn_e	oldBtn = btnInvalid;
	
	UInt16			Tab1Ctl[NUM_CTL_TAB_1] = { CHM_LBL, CHM_POP_INTERVAL,
												CHM_LBL_BETWEEN, CHM_TRG_FROM,
												CHM_LBL_AND_BETWEEN, CHM_TRG_TO,
												CHM_LBL_DISP_CLK, CHM_POP_DISP_CLK,
												CHM_CBX_KILL,
												CHM_LBL_BEEP, CHM_POP_BEEP, CHM_BTN_CONFIG_SND,
												CHM_LBL_LED, CHM_POP_LED, CHM_CBX_VIB };
	UInt16			Tab2Ctl[NUM_CTL_TAB_2] = { POP_CBX_ENABLE, POP_LBL_KEY, POP_POP_KEY,
												POP_LBL_MOD, POP_POP_MOD,
												POP_LBL_TIMEOUT, POP_POP_TIMEOUT,
												POP_CBX_ALLOW_KEY, POP_LBL_L1, POP_LBL_L2 };
	UInt16			Tab3Ctl[NUM_CTL_TAB_3] = { KG_CBX_ENABLE, KG_LBL_NOTE, 
												KG_LBL_KEY, KG_POP_KEY, 
												KG_LBL_MOD, KG_POP_MOD,
												KG_LBL_TIMEOUT, KG_POP_TIMEOUT,
												KG_CBX_NO_CLK_ON_SMS, KG_LBL_L1 };
	UInt16*			hideCtlList1 = NULL;
	UInt16*			hideCtlList2 = NULL;
	UInt16*			showCtlList = NULL;
	UInt16			hideCtlCount1 = 0;
	UInt16			hideCtlCount2 = 0;
	UInt16			showCtlCount = 0;
	UInt16			idx = 0;
	
	if ( oldBtn != btn )
	{
		switch ( btn )
		{
			case btnPopup:

				showCtlList = Tab2Ctl;
				showCtlCount = NUM_CTL_TAB_2;

				hideCtlList1 = Tab1Ctl;
				hideCtlList2 = Tab3Ctl;
				hideCtlCount1 = NUM_CTL_TAB_1;
				hideCtlCount2 = NUM_CTL_TAB_3;

				break;
				
			case btnKeyguard:
				
				showCtlList = Tab3Ctl;
				showCtlCount = NUM_CTL_TAB_3;
				
				hideCtlList1 = Tab1Ctl;
				hideCtlList2 = Tab2Ctl;
				hideCtlCount1 = NUM_CTL_TAB_1;
				hideCtlCount2 = NUM_CTL_TAB_2;
				break;
			
			default:
			
				showCtlList = Tab1Ctl;
				showCtlCount = NUM_CTL_TAB_1;
				
				hideCtlList1 = Tab2Ctl;
				hideCtlList2 = Tab3Ctl;
				hideCtlCount1 = NUM_CTL_TAB_2;
				hideCtlCount2 = NUM_CTL_TAB_3;
				
				break;	
		}
		
		oldBtn = btn;
	}
	
	for ( idx = 0 ; idx < hideCtlCount1 ; idx++ )
	{
		SetCtlVisibility( pForm, hideCtlList1[idx], false );
	}

	for ( idx = 0 ; idx < hideCtlCount2 ; idx++ )
	{
		SetCtlVisibility( pForm, hideCtlList2[idx], false );
	}
	
	for ( idx = 0 ; idx < showCtlCount ; idx++ )
	{
		SetCtlVisibility( pForm, showCtlList[idx], true );	
	}
	
	return;	
	
} // SetTabVisibility()

/*
 * InitializeMainFormGadgets()
 */
void InitializeMainFormGadgets( FormType* pForm, btn_e* btnP )
{
	if ( !pForm ) return;
			
	FrmSetGadgetHandler( pForm, FrmGetObjectIndex( pForm, MAIN_GDT ), (FormGadgetHandlerType *) MainForm_GadgetHandler );

	FrmSetGadgetData( pForm, FrmGetObjectIndex( pForm, MAIN_GDT ), btnP );
	
	return;
	
} // InitializeMainFormGadgets()

/* 
 * MainForm_GadgetHandler()
 */
Boolean MainForm_GadgetHandler( FormGadgetTypeInCallback *gadgetP, UInt16 cmd, void *paramP )
{
	Boolean 				handled = false;
	FormType*				pForm = FrmGetActiveForm();
	RectangleType			rect = gadgetP->rect;
	UInt16					bmpID = MAIN_BMP_TAB;
	MemHandle 				bmpH = NULL;
	BitmapType* 			bmpP = NULL;
	Coord					bmpSizeX = 0;
	Coord					bmpSizeY = 0;
	Coord					xLoc = X_POS_CHIME;
	btn_e					btn;
	static btn_e			lastBtn;
	
	if ( gadgetP->id != MAIN_GDT ) return ( handled );
	
	switch ( cmd )
	{
		case formGadgetDeleteCmd:

			handled = true;

			break;
			
		case formGadgetEraseCmd:

			handled = false;

			break;		

		case formGadgetHandleEventCmd:		
		case formGadgetDrawCmd:
	
			FrmSetActiveForm( pForm );
			
			gadgetP->attr.visible = true;
			
			switch ( *( (btn_e *) gadgetP->data ) )
			{
				case MAIN_BTN_POPUP:
				
					xLoc = X_POS_POPUP;
					
					break;
					
				case MAIN_BTN_KEYGUARD:
				
					xLoc = X_POS_KEYGUARD;
					
					break;
					
				default: // MAIN_BTN_CHIME
				
					xLoc = X_POS_CHIME;
					
					break;	
			}
			
			bmpID = MAIN_BMP_TAB;
			bmpP = GetBitmap( MAIN_BMP_TAB_BG, &bmpSizeX, &bmpSizeY, &bmpH );
			WinDrawBitmap( bmpP, rect.topLeft.x, rect.topLeft.y );
			ReleaseBitmap( &bmpH );	
			
			bmpID = MAIN_BMP_TAB;
			bmpP = GetBitmap( bmpID, &bmpSizeX, &bmpSizeY, &bmpH );
			WinDrawBitmap( bmpP, rect.topLeft.x + xLoc, rect.topLeft.y );
			ReleaseBitmap( &bmpH );	
			bmpH = NULL;
					
			handled = true; 
					
			break;
			
		default:
			break;
		
	}
	
	return ( handled );
	
} // MainForm_GadgetHandler()

/*
 * UpdateGadgets()
 */
void UpdateGadgets( FormType* pForm, btn_e* btnP )
{	
	EventType			event;
	static btn_e		oldBtn = MAIN_BTN_KEYGUARD;
	
	event.eType = frmGadgetMiscEvent;
	event.data.gadgetMisc.dataP = btnP;
	event.data.gadgetMisc.gadgetID = MAIN_GDT;
	EvtAddEventToQueue( &event );

	FrmNavRemoveFocusRing( pForm );
	
	CtlSetEnabled( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, oldBtn ) ), true );
	CtlSetEnabled( FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, *btnP ) ), false );
	
	SetTabVisibility( pForm, *btnP );
	
	oldBtn = *btnP;

	return;	
	
} // UpdateGadgets()


/*
 * Gadgets.c
 */