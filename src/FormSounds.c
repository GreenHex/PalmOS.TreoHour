/*
 * FormSounds.c
 */
 
#include "FormSounds.h"

static void 		SoundsForm_Initialize( FormType* pForm, sounds_t* pSnd );
static void 		SoundsForm_SavePrefs( FormType* pForm, sounds_t* pSnd );

/*
 * SoundsForm_HandleEvent()
 */
Boolean SoundsForm_HandleEvent( EventType* pEvent )
{
	Boolean 				handled = false;
	FormType* 				pForm = FrmGetActiveForm();
	ToneIdentifier			toneID = { 0, 0 };
	static appPrefs_t		prefs;
	UInt16					lstIdx = noListSelection;
	ListType*				pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SND_LST_SOUNDS ) );
		
	switch ( pEvent->eType )
	{		
		case frmOpenEvent:
		
			ReadAppPrefs( &prefs );
		
			SoundsForm_Initialize( pForm, &(prefs.sounds) );
			
			FrmDrawForm( pForm );
			
			handled = true;
						
			break;
			
		case popSelectEvent:

			if ( pEvent->data.popSelect.listID == SND_LST_SOUNDS )
			{
				if ( !GetToneID( pEvent->data.popSelect.selection, &toneID ) )
				{	 	
					switch ( pEvent->data.popSelect.controlID )
					{
						case SND_POP_HOUR:
							
							prefs.sounds.toneIdxHour = pEvent->data.popSelect.selection;
							
							break;
						
						case SND_POP_THREEQUARTER:
						
							prefs.sounds.toneIdxThreeQuarters = pEvent->data.popSelect.selection;
						
							break;
						
						case SND_POP_HALFHOUR:
							
							prefs.sounds.toneIdxHalfHouly = pEvent->data.popSelect.selection;
							
							break;
							
						case SND_POP_QUARTER:
							
							prefs.sounds.toneIdxQuarterly = pEvent->data.popSelect.selection;
							
							break;	
						
						default:
							break;
					}
					
					CtlSetLabel( pEvent->data.popSelect.controlP, 
									LstGetSelectionText( pEvent->data.popSelect.listP, 
									pEvent->data.popSelect.selection ) );
					
					PlayTone( toneID, toneVolume3, true );
					
				}
				
				handled = true;
			}
			
			break;
			
		case ctlSelectEvent:
		
			lstIdx = noListSelection;
			
			switch ( pEvent->data.ctlSelect.controlID )
			{
				case SND_POP_HOUR:
					
					lstIdx = prefs.sounds.toneIdxHour;
					
					break;
				
				case SND_POP_THREEQUARTER:
				
					lstIdx = prefs.sounds.toneIdxThreeQuarters;
				
					break;
				
				case SND_POP_HALFHOUR:
					
					lstIdx = prefs.sounds.toneIdxHalfHouly;
					
					break;
					
				case SND_POP_QUARTER:
					
					lstIdx = prefs.sounds.toneIdxQuarterly;
					
					break;
			
				case SND_BTN_DONE:
		
					SoundsForm_SavePrefs( pForm, &(prefs.sounds) );
								
					WriteAppPrefs( &prefs );
					
					FrmReturnToForm( 0 );
					
					handled = true;

					break;

				case SND_BTN_CANCEL:
				
					FrmReturnToForm( 0 );
			
					handled = true;
					
					break;
						
				default:
					break;
			}
			
			if ( ( lstIdx != noListSelection ) && ( pList ) )
			{
				LstSetSelection( pList, ( LstGetNumberOfItems( pList ) > lstIdx ) ? lstIdx : 0 );	
				LstSetTopItem( pList, LstGetSelection( pList ) );				
			}
			
			break;
			
		default:
								
			break;
	}
	
	return ( handled );
	
} // SoundsFormHandleEvent()

/*
 * SoundsForm_Initialize()
 */
static void SoundsForm_Initialize( FormType* pForm, sounds_t* pSnd )
{
	ListType*		pList = NULL;
	
	if ( !pForm ) return;
	
	pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SND_LST_SOUNDS ) );
	
	if ( pList )
	{
		LoadTonesList( pList );
		
		SetPopLabelFromList( pForm, SND_POP_HOUR, SND_LST_SOUNDS, pSnd->toneIdxHour );
		SetPopLabelFromList( pForm, SND_POP_THREEQUARTER, SND_LST_SOUNDS, pSnd->toneIdxThreeQuarters );
		SetPopLabelFromList( pForm, SND_POP_HALFHOUR, SND_LST_SOUNDS, pSnd->toneIdxHalfHouly );
		SetPopLabelFromList( pForm, SND_POP_QUARTER, SND_LST_SOUNDS, pSnd->toneIdxQuarterly );	 	
	}

	return;
	
} // SoundsForm_Initialize()

/*
 * SoundsForm_SavePrefs()
 */
static void SoundsForm_SavePrefs( FormType* pForm, sounds_t* pSnd )
{
	// ListType*		pList = FrmGetObjectPtr( pForm, FrmGetObjectIndex( pForm, SND_LST_SOUNDS ) );
	
	// EMPTY
	
	return;	

} // SoundsForm_SavePrefs()

/*
 * FormSounds.c
 */