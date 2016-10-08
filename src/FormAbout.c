/*
 * FormAbout.c
 */

#include "FormAbout.h"

/*
 * AboutForm_HandleEvent()
 */
Boolean AboutForm_HandleEvent( EventType* pEvent )
{
	Boolean 			handled = false;
	FormType*			pForm = FrmGetActiveForm();
	MemHandle			strVerH = NULL;
	Char*				strVerP = NULL;
	Char				strVer[20];
		
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
		
			strVerH = DmGet1Resource('tver', 1000);
	
			if ( strVerH )
			{
				strVerP = MemHandleLock( strVerH );
		
				if ( strVerP )
				{
						StrCopy( strVer, "v" );
						StrCat( strVer, strVerP );
						StrCat( strVer, " [Freeware]" );
						strVer[StrLen( (Char *)FrmGetLabel( pForm, ABOUT_LBL_VERSION ) )] = chrNull;
						
						FrmCopyLabel( pForm, ABOUT_LBL_VERSION, strVer );
			
						MemHandleUnlock( strVerH );
				}
	   	
			   	DmReleaseResource( strVerH );
			   	strVerH = NULL;
			}
		
			FrmDrawForm( pForm );
			
			handled = true;
						
			break;
		
		case ctlSelectEvent:
		
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case ABOUT_BTN_DONE:
				
					FrmReturnToForm( 0 );
					
					handled = true;

					break;
					
				default:
					break;
			}
		
			break;
			
		default:
							
			break;
	}
	
	return ( handled );
	
} // AboutForm_HandleEvent()

/*
 * FormAbout.c
 */