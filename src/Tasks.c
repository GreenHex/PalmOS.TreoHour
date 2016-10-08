/*
 * Tasks.c
 */

#include "Tasks.h"

#define TODO_SORT_OTHER_MAKE( sort, filter, subfilter )	\
	((((sort) & 0x0F) << 8) |	\
	 (((filter) & 0x0F) << 4) |	\
	 (((subfilter) & 0x0F))		\
	)
#define TODO_SORT_OTHER_GET_SORT_ORDER( other )	(((other) >> 8) & 0x0F)
#define TODO_SORT_OTHER_GET_FILTER( other )		(((other) >> 4) & 0x0F)
#define TODO_SORT_OTHER_GET_SUBFILTER( other )	(((other) & 0x0F))
	
static Char *ToDoDBRecordGetFieldPointer( ToDoDBRecordPtr recordP,
		ToDoRecordFieldType field );

#define maxDescLen	256
#define maxNoteLen	maxFieldTextLen             //was 4096, now is 32,767

/******************************************************************************
 *
 * FUNCTION:	ToDoDBRecordGetFieldPointer
 *
 * DESCRIPTION:	Get a pointer to the data for a specified field
 *
 * PARAMETERS:	recordP -		record pointer
 *				field -			which field
 *
 * RETURNED:	pointer to data if present, NULL otherwise
 *
 * REVISION HISTORY:
 *	Name	Date		Description
 *	----	----		-----------
 *	CS2		02/06/03	Initial Revision
 *
 *****************************************************************************/
static Char *ToDoDBRecordGetFieldPointer( ToDoDBRecordPtr recordP, ToDoRecordFieldType field )
{
	Char	*ret;

	if ( toDoRecordFieldDataFlags == field )
	{
		return ( Char * ) &recordP->dataFlags;
	}
	
	if ( toDoRecordFieldRecordFlags == field )
	{
		return ( Char * ) &recordP->recordFlags;
	}

	if ( toDoRecordFieldPriority == field )
	{
		return ( Char * ) &recordP->priority;
	}
	
	ret = ( Char * ) &recordP->optionalData;
	
	if ( toDoRecordFieldDueDate == field )
	{
		if ( recordP->dataFlags.dueDate )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}
	
	if ( recordP->dataFlags.dueDate )
	{
		ret += sizeof( DateType );
	}
	
	if ( toDoRecordFieldCompletionDate == field )
	{
		if ( recordP->dataFlags.completionDate )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.completionDate )
	{
		ret += sizeof( DateType );
	}

	if ( toDoRecordFieldAlarm == field )
	{
		if ( recordP->dataFlags.alarm )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.alarm )
	{
		ret += sizeof( ToDoAlarmInfoType );
	}

	if ( toDoRecordFieldRepeat == field )
	{
		if ( recordP->dataFlags.repeat )
		{
			return ret;
		}
		else
		{
			return NULL;
		}
	}

	if ( recordP->dataFlags.repeat )
	{
		ret += sizeof( ToDoRepeatInfoType );
	}

	if ( toDoRecordFieldDescription == field )
	{
		return ret;
	}

	ret += (StrLen( ret ) + 1);
	
	if ( toDoRecordFieldNote == field )
	{
		return ret;
	}
	
	ret += (StrLen( ret ) + 1);
	
	if ( toDoRecordFieldEndOfRecord == field )
	{
		return ret;
	}

	return NULL;
}

/*
 * GetNumTasks()
 */
UInt16 GetNumTasks(DateType* pDate)
{
	UInt16					count = 0;
	DmOpenRef				dbP = NULL;
	UInt16					categoryIdx = dmAllCategories;
	UInt16					numRecordsInCategory = 0;
	UInt16					recordIdx = 0;
	MemHandle				recordH = NULL;
	ToDoDBRecordPtr			recordP = NULL;
	DateType				DueDate;

	if (!pDate) return (count);
	
	dbP = DmOpenDatabaseByTypeCreator('DATA', kPalmOneCreatorIDTasks, dmModeReadOnly);
	
	if (dbP)
	{
		UInt16			i = 0;
		
		numRecordsInCategory = DmNumRecordsInCategory(dbP, categoryIdx);
		for ( i = 0 ; i < numRecordsInCategory ; i++ )
		{
			recordH = DmQueryNextInCategory(dbP, &recordIdx, categoryIdx);
				
			if (recordH)
			{				
				recordP = (ToDoDBRecordPtr) MemHandleLock(recordH);

/*
 * 1. Does not have due date
 * 2. Past due date
 * 2.a Completed
 * 2.b Not completed
 * 3. Today
 * 3.a Completed
 * 3.b Not Completed
 * 4. Future
 * 4.a Completed
 * 4.b Not completed
 */
				if (recordP)
				{				
					if ( !( recordP->recordFlags & TODO_RECORD_FLAG_COMPLETE ) )
					{
						if (recordP->dataFlags.dueDate)
						{
							DueDate = *( DateType* )ToDoDBRecordGetFieldPointer( recordP, toDoRecordFieldDueDate );
							
							if ( DateCompare( DueDate, *pDate ) <= 0)
							{
								count++;
							}
						}
						else
						{
							count++;
						}
					}	
				}

				MemHandleUnlock(recordH);
				recordH = NULL;
				recordP = NULL;
			}
			++recordIdx;
		}
		// ToDoDBNumRecordsInDateSubFilters(dbP, *pDate, false, &count, NULL, NULL, NULL);
		DmCloseDatabase(dbP);
	}
	
	return (count);
	
} // GetNumTasks()

/*
 * Tasks.c
 */