/*
 * DateBook.c
 */

#include "DateBook.h"

typedef struct {
	ApptDateTimeType 	when;
	ApptDBRecordFlags	flags;	// A flag set for each  datum present
	char				firstField;
	UInt8				reserved;
} ApptPackedDBRecordType;

typedef ApptPackedDBRecordType * ApptPackedDBRecordPtr;

/*
typedef struct {
	DmOpenRef					dbP;
	UInt16						cardNo;
	LocalID						dbID;
	DatebookPreferenceType		prefs;
} AlarmPostingData;
*/
// typedef AlarmPostingData * AlarmPostingDataPtr;

static Int16 				ApptListCompare(ApptInfoPtr a1, ApptInfoPtr  a2, Int32 extra);
static Boolean 				ApptFindFirst(DmOpenRef dbP, DateType date, UInt16* indexP);
static void 				ApptUnpack(ApptPackedDBRecordPtr src, ApptDBRecordPtr dest);
static Boolean 				ApptRepeatsOnDate(ApptDBRecordPtr apptRec, DateType date);
static Boolean 				IsException(ApptDBRecordPtr apptRec, DateType date);
static Boolean 				ApptNextRepeat(ApptDBRecordPtr apptRec, DatePtr dateP, Boolean searchForward);
static Boolean 				FindNextRepeat(ApptDBRecordPtr apptRec, DatePtr dateP, Boolean searchForward);
static VoidHand 			ApptGetAppointments (DmOpenRef dbP, DateType date, UInt16* countP);
static Err 					ApptGetRecord(DmOpenRef dbP, UInt16 index, ApptDBRecordPtr r, MemHandle * handleP);

/************************************************************
 *
 *  FUNCTION: ApptGetRepeatInfo
 *
 *  DESCRIPTION: Fills in the ApptDBRecord structure
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the record unpacked
 *
 *  CREATED: 4/28/03
 *
 *  BY: Peter Epstein
 *
 *************************************************************/
 RepeatInfoPtr ApptGetRepeatInfo(ApptPackedDBRecordPtr src)
{
	ApptDBRecordFlags	flags;
	char *p;

	
	flags = src->flags;
	p = &src->firstField;

	if (flags.alarm) 
		p += sizeof (AlarmInfoType);

	if (flags.repeat)
		return (RepeatInfoType *) p;
	else
		return NULL;
}

/***********************************************************************
 *
 * FUNCTION:    ApptListCompare
 *
 * DESCRIPTION: This routine compares two entries in the appointment list, 
 *              it's called by ApptGetAppointments via the quick sort 
 *              routine.
 *
 * PARAMETERS:  a     - a pointer to an entry in the appointment list
 *					 b     - a pointer to an entry in the appointment list
 *              extra - extra data passed to quick sort - not used
 *
 * RETURNED:    if a1 > a2  returns a positive int
 *              if a1 < a2  returns a negative int
 *              if a1 = a2  returns zero
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/95		Initial Revision
 *
 ***********************************************************************/
static Int16 ApptListCompare(ApptInfoPtr a1, ApptInfoPtr  a2, Int32 extra)
{
	Int16 result;
	
	result = TimeCompare (a1->startTime, a2->startTime);
	if (result == 0)
		{
		result = TimeCompare (a1->endTime, a2->endTime);
		}
	return result;
}

/***********************************************************************
 *
 * FUNCTION:    ApptFindFirst
 *
 * DESCRIPTION: This routine finds the first appointment on the specified
 *              day.
 *
 * PARAMETERS:  dbP    - pointer to the database
 *              date   - date to search for
 *              indexP - pointer to the index of the first record on the 
 *                       specified day (returned value)
 *
 * RETURNED:    true if a record has found
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/15/95		Initial Revision
 *
 ***********************************************************************/
Boolean ApptFindFirst (DmOpenRef dbP, DateType date, UInt16* indexP)
{
	Err err;
	Int16 numOfRecords;
	Int16 kmin, probe, i;		// all positions in the database.
	Int16 result = 0;			// result of comparing two records
	UInt16 index;
	MemHandle recordH;
	Boolean found = false;
	ApptPackedDBRecordPtr r;
 

	kmin = probe = 0;
	numOfRecords = DmNumRecords(dbP);
	
	
	while (numOfRecords > 0)
		{
		i = numOfRecords >> 1;
		probe = kmin + i;
		
		index = probe;
		recordH = DmQueryNextInCategory (dbP, &index, dmAllCategories);
		if (recordH)
			{
			r = (ApptPackedDBRecordPtr) MemHandleLock (recordH);
			if (r->flags.repeat)
				result = 1;
			else
				result = DateCompare (date, r->when.date);
			MemHandleUnlock (recordH);
			}

		// If no handle, assume the record is deleted, deleted records
		// are greater.
		else
			result = -1;
			

		// If the date passed is less than the probe's date, keep searching.
		if (result < 0)
			numOfRecords = i;

		// If the date passed is greater than the probe's date, keep searching.
		else if (result > 0)
			{
			kmin = probe + 1;
			numOfRecords = numOfRecords - i - 1;
			}

		// If the records are equal find the first record on the day.
		else
			{
			found = true;
			*indexP = index;
			while (true)
				{
				err = DmSeekRecordInCategory (dbP, &index, 1, dmSeekBackward, 
					dmAllCategories);
				if (err == dmErrSeekFailed) break;
				
				recordH = DmQueryRecord(dbP, index);
				r = (ApptPackedDBRecordPtr) MemHandleLock (recordH);
				if (r->flags.repeat)
					result = 1;
				else
					result = DateCompare (date, r->when.date);
				MemHandleUnlock (recordH);
				if (result != 0) break;
				*indexP = index;
				}

			break;
			}
		}

	
	// If that were no appointments on the specified day, return the 
	// index of the next appointment (on a future day).
	if (! found)
		{
		if (result < 0) 
			*indexP = probe;
		else if (DmNumRecords(dbP) == 0)
			*indexP = 0;
		else
			*indexP = probe + 1;
		}

	return (found);
}

/************************************************************
 *
 *  FUNCTION: ApptUnpack
 *
 *  DESCRIPTION: Fills in the ApptDBRecord structure
 *
 *  PARAMETERS: database record
 *
 *  RETURNS: the record unpacked
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static void ApptUnpack(ApptPackedDBRecordPtr src, ApptDBRecordPtr dest)
{
	ApptDBRecordFlags	flags;
	Char *p;
	Char *blobStart;
	Char *blobEnd;
	UInt16 recordSize;
	UInt32 blobId;
	UInt16 blobSize;
	UInt16 index;
	flags = src->flags;
	p = &src->firstField;

	recordSize = MemPtrSize(src);
	dest->when = (ApptDateTimeType *) src;
	ErrNonFatalDisplayIf(dest->when->endTime.hours == hoursPerDay, "Hours = 24 found in database");
	
	if (flags.alarm) 
		{
		dest->alarm = (AlarmInfoType *) p;
		p += sizeof (AlarmInfoType);
		}
	else
		dest->alarm = NULL;

	
	if (flags.repeat)
		{
		dest->repeat = (RepeatInfoType *) p;
		p += sizeof (RepeatInfoType);
		}
	else
		dest->repeat = NULL;
	
	
	if (flags.exceptions)
		{
		dest->exceptions = (ExceptionsListType *) p;
		p += sizeof (UInt16) + 
			(((ExceptionsListType *) p)->numExceptions * sizeof (DateType));
		}
	else
		dest->exceptions = NULL;
		
	
	if (flags.description)
		{
		dest->description = p;
		p += StrLen(p) + 1;
		}
	else
		dest->description = NULL;
		
	
	if (flags.note)
		{
		dest->note = p;
		p += StrLen(p) + 1;
		}
	else
		dest->note = NULL;
		
	
	if (flags.location)
		{
		dest->location = p;
		p += StrLen(p) + 1;
		}
	else
		dest->location = NULL;

	// There may also be blob data on the end of the record.
	// First set everything as if there were no blobs.
	dest->timeZone.name[0] = chrNull;			// This indicates that there is no time zone info for this meeting.
	dest->meetingInfo.meetingStatus = unansweredMeeting;
	dest->meetingInfo.apptStatus = showAsBusy;
	dest->meetingInfo.numAttendees = 0;			// This indicates that it is not a meeting.
//	dest->serverID = NULL;							// This indicates that there is no known server ID.
	dest->numBlobs = 0;								// Start by assuming no blobs we don't understand.
	
	
	// Then iterate through the blobs, ignoring any we don't understand.
	blobStart = p;			// First blob starts where last non-blob data ends.
	while (blobStart < (Char *)src + recordSize)
	{
		p = blobStart;
		// If a field is using edit in place to directly edit a database record at the time this routine
		// is called, or if the device was reset while an edit in place was in progress, the record can
		// be left with junk data on the end. FldCompactText would clean up this junk, but it either wasn't
		// called or simply hasn't yet been called. We will attempt to parse this junk data as blobs, but
		// more than likely these blobs will appear to be invalid. On release builds we want to recover
		// gracefully from this situation by ignoring the junk data.
		if ((Char *)src + recordSize - blobStart <= sizeof (blobId) + sizeof (blobSize))
		{
			ErrNonFatalDisplay("Blob goes beyond end of record - don't let fields edit records directly!");
			return;
		}
		MemMove(&blobId, p, sizeof (blobId));
		p += sizeof (blobId);
		MemMove(&blobSize, p, sizeof (blobSize));		
		p += sizeof (blobSize);
		blobEnd = p + blobSize;			// Blob size excludes space to store ID and size of blob.
		if (blobEnd > (Char *)src + recordSize)
		{
			ErrNonFatalDisplay("Blob goes beyond end of record - don't let fields edit records directly!");
			return;
		}
		switch (blobId)
		{
			case dateTimeZoneBlobId:
				ErrNonFatalDisplayIf(dest->timeZone.name[0] != chrNull, "Duplicate time zone blob");
				MemMove(&dest->timeZone.uTC, p, sizeof (dest->timeZone.uTC));
				p += sizeof (dest->timeZone.uTC);
				MemMove(&dest->timeZone.dSTStart, p, sizeof (dest->timeZone.dSTStart));
				p += sizeof (dest->timeZone.dSTStart);
				MemMove(&dest->timeZone.dSTEnd, p, sizeof (dest->timeZone.dSTEnd));
				p += sizeof (dest->timeZone.dSTEnd);
				MemMove(&dest->timeZone.dSTAdjustmentInMinutes, p, sizeof (dest->timeZone.dSTAdjustmentInMinutes));
				p += sizeof (dest->timeZone.dSTAdjustmentInMinutes);
				MemMove(&dest->timeZone.country, p, sizeof (dest->timeZone.country));
				p += sizeof (dest->timeZone.country);
				MemMove((Char *)&dest->timeZone.name - 1 /*custom flag*/, p, sizeof (UInt8));
				p += sizeof (UInt8);
				ErrNonFatalDisplayIf(StrLen(p) > apptMaxTimeZoneNameLen - 1, "Time zone name too long");
				StrCopy(dest->timeZone.name, p);
				p += StrLen(dest->timeZone.name) + 1;
				//HostTraceOutputTL (appErrorClass, "UnPack wasSetByUser %d uTC %d ",dest->timeZone.wasSetByUser, dest->timeZone.uTC);	
				break;

			case dateMeetingBlobId:
				ErrNonFatalDisplayIf(dest->meetingInfo.numAttendees != 0, "Duplicate meeting blob");
				MemMove(&dest->meetingInfo.meetingStatus, p, sizeof (dest->meetingInfo.meetingStatus));
				p += sizeof (dest->meetingInfo.meetingStatus);
				MemMove(&dest->meetingInfo.apptStatus, p, sizeof (dest->meetingInfo.apptStatus));
				p += sizeof (dest->meetingInfo.apptStatus);
				MemMove(&dest->meetingInfo.numAttendees, p, sizeof (dest->meetingInfo.numAttendees));
				p += sizeof (dest->meetingInfo.numAttendees);
				for (index = 0; index < dest->meetingInfo.numAttendees; index++)
				{
					MemMove(&dest->meetingInfo.attendees[index].role, p, sizeof (dest->meetingInfo.attendees[index].role));
					p += sizeof (dest->meetingInfo.attendees[index].role);
					dest->meetingInfo.attendees[index].name = p;
					p += StrLen(dest->meetingInfo.attendees[index].name) + 1;
					dest->meetingInfo.attendees[index].email = p;
					p += StrLen(dest->meetingInfo.attendees[index].email) + 1;
					dest->meetingInfo.attendees[index].AttendeePos = index;
				}
				break;
/*			
			case dateServerIDBlobId:
				dest->serverID = p;
				p += StrLen(dest->serverID);
				break;
*/			
			default:
			{
				ErrNonFatalDisplayIf (dest->numBlobs >= apptMaxBlobs, "Too many blobs");

				dest->blobs[dest->numBlobs].creatorID = blobId;
				dest->blobs[dest->numBlobs].size = blobSize;
				dest->blobs[dest->numBlobs].content = p;				
	
				dest->numBlobs++;
				p = blobEnd;
				//ErrNonFatalDisplayIf(dest->numBlobs >= apptMaxBlobs, "Too many blobs");
				//MemMove(&dest->blobs[dest->numBlobs], p, sizeof (dest->blobs[dest->numBlobs]));
				break;
			}	
		}
		ErrNonFatalDisplayIf(p != blobEnd, "Blob size does not agree with contents");
		blobStart = blobEnd;	// Next blob starts where last blob ends.
	}
	ErrNonFatalDisplayIf(blobStart != (Char *)src + recordSize,
		"Last blob not aligned with end of record - don't let fields edit records directly!");
	ErrNonFatalDisplayIf(ApptPackedSize(dest) != recordSize, "Blob size mismatch");	
}

/***********************************************************************
 *
 * FUNCTION:    ApptRepeatsOnDate
 *
 * DESCRIPTION: This routine returns true if a repeating appointment
 *              occurrs on the specified date.
 *
 * PARAMETERS:  apptRec - a pointer to an appointment record
 *              date    - date to check              
 *
 * RETURNED:    true if the appointment occurs on the date specified
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/14/95		Initial Revision
 *
 ***********************************************************************/
static Boolean ApptRepeatsOnDate (ApptDBRecordPtr apptRec, DateType date)
{
	Int16  i;
	UInt16 freq;
	UInt16 weeksDiff;
	UInt16 dayInMonth;
	UInt16 dayOfWeek;
	UInt16 dayOfMonth;
	UInt16 firstDayOfWeek;
	Int32 dateInDays;
	Int32 startInDays;
	Boolean onDate;
	DatePtr exceptions;
	DateType startDate;

	// Is the date passed before the start date of the appointment?
	if (DateCompare (date, apptRec->when->date) < 0)
		return (false);

	// Is the date passed after the end date of the appointment?
	if (DateCompare (date, apptRec->repeat->repeatEndDate) > 0)
		return (false);
	

	// Get the frequency of occurrecne (ex: every 2nd day, every 3rd month, etc.).  
	freq = apptRec->repeat->repeatFrequency;
	
	// Get the date of the first occurrecne of the appointment.
	startDate = apptRec->when->date;

	switch (apptRec->repeat->repeatType)
		{
		// Daily repeating appointment.
		case repeatDaily:
			dateInDays = DateToDays (date);
			startInDays = DateToDays (startDate);
			onDate = ((dateInDays - startInDays) % freq) == 0;
			break;
			

		// Weekly repeating appointment (ex: every Monday and Friday). 
		// Yes, weekly repeating appointment can occur more then once a
		// week.
		case repeatWeekly:
			// Are we on a day of the week that the appointment repeats on.
			dayOfWeek = DayOfWeek (date.month, date.day, date.year+firstYear);
			onDate = ((1 << dayOfWeek) & apptRec->repeat->repeatOn);
			if (! onDate) break;

			// Are we in a week in which the appointment occurrs, if not 
			// move to that start of the next week in which the appointment
			// does occur.
			dateInDays = DateToDays (date);
			startInDays = DateToDays (startDate);

			firstDayOfWeek = (DayOfWeek (1, 1, firstYear) - 
				apptRec->repeat->repeatStartOfWeek + daysInWeek) % daysInWeek;

			weeksDiff = (((dateInDays + firstDayOfWeek) / daysInWeek) - 
							 ((startInDays + firstDayOfWeek) / daysInWeek)) %freq;
			onDate = (weeksDiff == 0);
			break;


//			// Compute the first occurrence of the appointment that occurs
//			// on the same day of the week as the date passed.
//			startDayOfWeek = DayOfWeek (startDate.month, startDate.day, 
//				startDate.year+firstYear);
//			startInDays = DateToDays (startDate);
//			if (startDayOfWeek < dayOfWeek)
//				startInDays += dayOfWeek - startDayOfWeek;
//			else if (startDayOfWeek > dayOfWeek)
//				startInDays += dayOfWeek+ (daysInWeek *freq) - startDayOfWeek;
//			
//			// Are we in a week in which the appointment repeats.
//			dateInDays = DateToDays (date);
//			onDate = (((dateInDays - startInDays) / daysInWeek) % freq) == 0;
//			break;


		// Monthly-by-day repeating appointment (ex: the 3rd Friday of every
		// month).
		case repeatMonthlyByDay:
			// Are we in a month in which the appointment repeats.
			onDate = ((((date.year - startDate.year) * monthsInYear) + 
						   (date.month - startDate.month)) % freq) == 0;
			if (! onDate) break;

			// Do the days of the month match (ex: 3rd Friday)
			dayOfMonth = DayOfMonth (date.month, date.day, date.year+firstYear);
			onDate = (dayOfMonth == apptRec->repeat->repeatOn);
			if (onDate) break;
			
			// If the appointment repeats on one of the last days of the month,
			// check if the date passed is also one of the last days of the 
			// month.  By last days of the month we mean: last sunday, 
			// last monday, etc.
			if ((apptRec->repeat->repeatOn >= domLastSun) &&
				 (dayOfMonth >= dom4thSun))
				{
				dayOfWeek = DayOfWeek (date.month, date.day, date.year+firstYear);
				dayInMonth = DaysInMonth (date.month, date.year+firstYear);
				onDate = (((date.day + daysInWeek) > dayInMonth) &&
							 (dayOfWeek == (apptRec->repeat->repeatOn % daysInWeek)));
				}
			break;						


		// Monthly-by-date repeating appointment (ex: the 15th of every
		// month).
		case repeatMonthlyByDate:
			// Are we in a month in which the appointment repeats.
			onDate = ((((date.year - startDate.year) * monthsInYear) + 
						   (date.month - startDate.month)) % freq) == 0;
			if (! onDate) break;
			
			// Are we on the same day of the month as the start date.
			onDate = (date.day == startDate.day);
			if (onDate) break;

			// If the staring day of the appointment is greater then the 
			// number of day in the month passed, and the day passed is the 
			// last day of the month, then the appointment repeats on the day.
			dayInMonth = DaysInMonth (date.month, date.year+firstYear);
			onDate = ((startDate.day > dayInMonth) && (date.day == dayInMonth));
			break;


		// Yearly repeating appointment.
		case repeatYearly:
			// Are we in a year in which the appointment repeats.
			onDate = ((date.year - startDate.year) % freq) == 0;
			if (! onDate) break;
			
			// Are we on the month and day that the appointment repeats.
			onDate = (date.month == startDate.month) &&
				      (date.day == startDate.day);
			if (onDate) break;
			
			// Specal leap day processing.
			if ( (startDate.month == february) && 
				  (startDate.day == 29) &&
				  (date.month == february) && 
				  (date.day == DaysInMonth (date.month, date.year+firstYear)))
				{
				onDate = true;
				}				      
			break;
		}

	// Check for an exception.
	if ((onDate) && (apptRec->exceptions))
		{
		exceptions = &apptRec->exceptions->exception;
		for (i = 0; i < apptRec->exceptions->numExceptions; i++)
			{
			if (DateCompare (date, exceptions[i]) == 0)
				{
				onDate = false;
				break;
				}
			}
		}


	return (onDate);
}

/***********************************************************************
 *
 * FUNCTION:    ApptGetAppointments
 *
 * DESCRIPTION: This routine returns a list of appointments that are in 
 *              the range of dates specified
 *
 * PARAMETERS:  dbP       - pointer to the database
 *              date   	  - start date to search from
 *              days   	  - number a days in search range
 *              apptLists - returned: array of handle of the 
 *									 appointment list (ApptInfoType)
 *              counts    - returned: returned: array of counts of the 
 *                          number of appointments in each list.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	4/7/96	Initial Revision
 *
 ***********************************************************************/
static VoidHand ApptGetAppointments(DmOpenRef dbP, DateType date, UInt16* countP)
{
	Err	error;
	Int16	result;
	Int16	count = 0;
	UInt16	recordNum;
	Boolean repeats;
	MemHandle recordH;
	MemHandle apptListH;
	ApptInfoPtr apptList;
	ApptDBRecordType apptRec;
	ApptPackedDBRecordPtr r;

	// Allocated a block to hold the appointment list.
	apptListH = MemHandleNew (sizeof (ApptInfoType) * apptMaxPerDay);
	ErrFatalDisplayIf(!apptListH, "Out of memory");
	if (! apptListH) return (0);

	apptList = MemHandleLock (apptListH);
	

	// Find the first non-repeating appointment of the day.
	if (ApptFindFirst (dbP, date, &recordNum))
		{
		while (count < apptMaxPerDay)
			{
			// Check if the appointment is on the date passed, if it is 
			// add it to the appointment list.		
			recordH = DmQueryRecord (dbP, recordNum);
			r = MemHandleLock (recordH);
			result = DateCompare (r->when.date, date);

			if (result == 0)
				{
				// Add the record to the appoitment list.
				apptList[count].startTime = r->when.startTime;				
				apptList[count].endTime = r->when.endTime;				
				apptList[count].recordNum = recordNum;	
				count++;
				}
			MemHandleUnlock (recordH);
			if (result != 0) break;

			// Get the next record.
			error = DmSeekRecordInCategory (dbP, &recordNum, 1, dmSeekForward, dmAllCategories);
			if (error == dmErrSeekFailed) break;
			}
		}


	// Add the repeating appointments to the list.  Repeating appointments
	// are stored at the beginning of the database.
	recordNum = 0;
	while (count < apptMaxPerDay)
		{
		recordH = DmQueryNextInCategory (dbP, &recordNum, dmAllCategories);
		if (! recordH) break;
		
		r = (ApptPackedDBRecordPtr) MemHandleLock (recordH);
		
		repeats = (r->flags.repeat != 0);
		if (repeats)
			{
			ApptUnpack (r, &apptRec);
			if (ApptRepeatsOnDate (&apptRec, date))
				{
				// Add the record to the appoitment list.
				apptList[count].startTime = r->when.startTime;				
				apptList[count].endTime = r->when.endTime;				
				apptList[count].recordNum = recordNum;	
				count++;
				}
			}
		MemHandleUnlock (recordH);

		// If the record has no repeating info we've reached the end of the 
		// repeating appointments.
		if (! repeats) break;
		
		 recordNum++;
		}

	
	// Sort the list by start time.
	SysInsertionSort (apptList, count, sizeof (ApptInfoType), (CmpFuncPtr)ApptListCompare, 0L);
	

	// If there are no appointments on the specified day, free the appointment
	// list.
	if (count == 0)
		{
		MemPtrFree (apptList);
		apptListH = 0;
		}

	// Resize the appointment list block to release any unused space.
	else
		{
		MemHandleUnlock (apptListH);
		MemHandleResize (apptListH, count * sizeof (ApptInfoType));
		}

	*countP = count;
	return (apptListH);
}

/***********************************************************************
 *
 * FUNCTION:    IsException
 *
 * DESCRIPTION: This routine returns true the date passed is in a 
 *              repeating appointment's exception list.
 *
 * PARAMETERS:  apptRec - a pointer to an appointment record
 *              date    - date to check              
 *
 * RETURNED:    true if the date is an exception date.
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/14/95		Initial Revision
 *
 ***********************************************************************/
static Boolean IsException (ApptDBRecordPtr apptRec, DateType date)
{
	int i;
	DatePtr exceptions;

	if (apptRec->exceptions)
		{
		exceptions = &apptRec->exceptions->exception;
		for (i = 0; i < apptRec->exceptions->numExceptions; i++)
			{
			if (DateCompare (date, exceptions[i]) == 0)
			return (true);
			}
		}
	return (false);
}

/***********************************************************************
 *
 * FUNCTION:    FindNextRepeat
 *
 * DESCRIPTION: This routine computes the date of the next 
 *              occurrence of a repeating appointment.
 *
 * PARAMETERS:  apptRec - a pointer to an appointment record
 *              date    - passed:   date to start from
 *                        returned: date of next occurrence             
 *              searchForward - search for the next occurrence before or after the 
 *						  specified date
 *
 * RETURNED:    true if an occurrence was found
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/14/95	Initial Revision
 *			gap	09/25/00	Add capability to search for the most recent previous occurrence
 *								(needed for attention manager support)
 *			gap	02/14/01	fix some problems in the weekly repeating event
 *								reverse search code.
 *
 ***********************************************************************/
static Boolean FindNextRepeat (ApptDBRecordPtr apptRec, DatePtr dateP, Boolean searchForward)
{
	Int16  i;
	Int32  adjust;
	Int32  daysTilNext;
	Int32  monthsTilNext;
	UInt16 day;
	UInt16 freq;
	UInt16 year;
	UInt16 weeksDiff;
	UInt16 monthsDiff;
	UInt16 daysInMonth;
	UInt16 dayOfWeek;
	UInt16 apptWeekDay;
	UInt16 firstDayOfWeek;
	UInt32 dateInDays;
	UInt32 startInDays;
	DateType start;
	DateType date;
	DateType next;
	

	date = *dateP;
	
	if (searchForward)
		{
		// Is the date passed after the end date of the appointment?
		if (DateCompare (date, apptRec->repeat->repeatEndDate) > 0)
			return (false);
		
		// Is the date passed before the start date of the appointment?
		if (DateCompare (date, apptRec->when->date) < 0)
			date = apptRec->when->date;
		}
	else
		{
		// Is the date passed is before the start date of the appointment? 
		// return false now
		if (DateCompare (date, apptRec->when->date) < 0)
			return (false);

		// Is the date passed after the end date of the appointment?
		// search backwards from repeat end date for first valid occurrence.
		if (DateCompare (date, apptRec->repeat->repeatEndDate) > 0)
			date = apptRec->repeat->repeatEndDate;
		}

	// apptRec->repeat->repeatEndDate can be passed into this routine
	// or be set in the else case above.  Since apptNoEndDate is not a 
	// valid date (month is 15) set it must be set to the last date 
	// support by the current OS  12/31/31
	if ( DateToInt(date) == apptNoEndDate)
		date.month = 12;

	// Get the frequency on occurrecne (ex: every 2nd day, every 3rd month, etc).  
	freq = apptRec->repeat->repeatFrequency;
	
	// Get the date of the first occurrecne of the appointment.
	start = apptRec->when->date;	

	switch (apptRec->repeat->repeatType)
		{
		// Daily repeating appointment.
		case repeatDaily:
			dateInDays = DateToDays (date);
			startInDays = DateToDays (start);
			if (searchForward)
				daysTilNext = (dateInDays - startInDays + freq - 1) / freq * freq;
			else
				daysTilNext = (dateInDays - startInDays) / freq * freq;
			if (startInDays + daysTilNext > (UInt32) maxDays)
				return (false);
			DateDaysToDate (startInDays + daysTilNext, &next);
			break;
			


		// Weekly repeating appointment (ex: every Monday and Friday). 
		// Yes, weekly repeating appointment can occur more then once a
		// week.
		case repeatWeekly:
			dateInDays = DateToDays (date);
			startInDays = DateToDays (start);

			firstDayOfWeek = (DayOfWeek (1, 1, firstYear) - 
				apptRec->repeat->repeatStartOfWeek + daysInWeek) % daysInWeek;

			dayOfWeek = DayOfWeek (date.month, date.day, date.year+firstYear);
			apptWeekDay = (dayOfWeek - apptRec->repeat->repeatStartOfWeek +
				daysInWeek) % daysInWeek;

			// Are we in a week in which the appointment occurrs, if not 
			// move to that start of the next week in which the appointment
			// does occur.
			weeksDiff = (((dateInDays + firstDayOfWeek) / daysInWeek) - 
							 ((startInDays + firstDayOfWeek) / daysInWeek)) %freq;
			if (weeksDiff)
				{
				if (searchForward)
					{
					adjust = ((freq - weeksDiff) * daysInWeek) - apptWeekDay;
					apptWeekDay = 0;
					dayOfWeek = (dayOfWeek + adjust) % daysInWeek;
					}
				else
					{
					adjust = (weeksDiff * daysInWeek) - ( (daysInWeek-1) - apptWeekDay);
					apptWeekDay = 6;
					dayOfWeek = (dayOfWeek - (adjust % daysInWeek) + daysInWeek) % daysInWeek;
					}
				}
			else
				adjust = 0;
			
			// Find the next day on which the appointment repeats.
			if (searchForward)
				{
				for (i = 0; i < daysInWeek; i++)
					{
					if (apptRec->repeat->repeatOn & (1 << dayOfWeek)) break;
					adjust++;
					if (++dayOfWeek == daysInWeek)
						dayOfWeek = 0;
					if (++apptWeekDay == daysInWeek)
						adjust += (freq - 1) * daysInWeek;
					}

				if (dateInDays + adjust > (UInt32) maxDays)
					return (false);
				DateDaysToDate (dateInDays + adjust, &next);
				}
			else
				{
				for (i = 0; i < daysInWeek; i++)
					{
					if (apptRec->repeat->repeatOn & (1 << dayOfWeek)) break;
					adjust++;
					if (dayOfWeek == 0)
						dayOfWeek = daysInWeek-1;
					else
						dayOfWeek--;
					if (apptWeekDay == 0)
						{
						adjust += (freq - 1) * daysInWeek;
						apptWeekDay = daysInWeek-1;
						}
					else
						apptWeekDay --;
					}		
				
				// determine if date goes past first day (unsigned int wraps around)
				if (dateInDays - adjust > dateInDays) 
					return (false);
					
				DateDaysToDate (dateInDays - adjust, &next);
				}
				
			break;
			


		// Monthly-by-day repeating appointment (ex: the 3rd Friday of every
		// month).
		case repeatMonthlyByDay:
			// Compute the number of month until the appointment repeats again.
			if (searchForward)
				monthsTilNext = ((((date.year - start.year) * monthsInYear) + (date.month - start.month)) + freq - 1) /freq * freq;
			else
				monthsTilNext = (((date.year - start.year) * monthsInYear) + (date.month - start.month)) /freq * freq;

			while (true)
				{
				year = start.year + (start.month - 1 + monthsTilNext) / monthsInYear;
				if (year >= numberOfYears)
					return (false);

				next.year = year;
				next.month = (start.month - 1 + monthsTilNext) % monthsInYear + 1;
	
				dayOfWeek = DayOfWeek (next.month, 1, next.year+firstYear);
				if ((apptRec->repeat->repeatOn % daysInWeek) >= dayOfWeek)
					day = apptRec->repeat->repeatOn - dayOfWeek + 1;
				else
					day = apptRec->repeat->repeatOn + daysInWeek - dayOfWeek + 1;
	
				// If repeat-on day is between the last sunday and the last
				// saturday, make sure we're not passed the end of the month.
				if ( (apptRec->repeat->repeatOn >= domLastSun) &&
					  (day > DaysInMonth (next.month, next.year+firstYear)))
					{
					day -= daysInWeek;
					}
				next.day = day;

				// Its posible that "next date" calculated above is 
				// before the date passed.  If so, move forward
				// by the length of the repeat freguency and perform
				// the calculation again.
				if (searchForward)
					{
					if ( DateToInt(date) > DateToInt (next))
						monthsTilNext += freq;
					else
						break;
					}
				else
					{
					if ( DateToInt(date) < DateToInt (next))
						monthsTilNext -= freq;
					else
						break;
					}
				}
			break;						



		// Monthly-by-date repeating appointment (ex: the 15th of every
		// month).
		case repeatMonthlyByDate:
			// Compute the number of month until the appointment repeats again.
			monthsDiff = ((date.year - start.year) * monthsInYear) + (date.month - start.month);
			if (searchForward)
				{
				monthsTilNext = (monthsDiff + freq - 1) / freq * freq;
				if ((date.day > start.day) && (!(monthsDiff % freq)))
					monthsTilNext += freq;
				}
			else
				{
				monthsTilNext = monthsDiff / freq * freq;
				if ((date.day < start.day) && (!(monthsDiff % freq)))
					monthsTilNext -= freq;
				}
				
			year = start.year + (start.month - 1 + monthsTilNext) / monthsInYear;
			if (year >= numberOfYears)
				return (false);

			next.year = year;
			next.month = (start.month - 1 + monthsTilNext) % monthsInYear + 1;
			next.day = start.day;

			// Make sure we're not passed the last day of the month.
			daysInMonth = DaysInMonth (next.month, next.year+firstYear);
			if (next.day > daysInMonth)
				next.day = daysInMonth;
			break;



		// Yearly repeating appointment.
		case repeatYearly:
			next.day = start.day;
			next.month = start.month;

			if (searchForward)
				{
				year = start.year + ((date.year - start.year + freq - 1) / freq * freq);
				if (	(date.month > start.month) ||
				  		((date.month == start.month) && (date.day > start.day)) )
					year += freq;
				}
			else
				{
				year = start.year + ((date.year - start.year) / freq * freq);
				if (	(date.month < start.month) ||
				  		((date.month == start.month) && (date.day < start.day)) ) 
					year -= freq;
				}
			

			// Specal leap day processing.
			if ( (next.month == february) && (next.day == 29) &&
				  (next.day > DaysInMonth (next.month, year+firstYear)))
				{
				next.day = DaysInMonth (next.month, year+firstYear);
				}				      
			if (year >= numberOfYears)
				return (false);

			next.year = year;	
			break;
		}
		
	if (searchForward)
		{
		// Is the next occurrence after the end date of the appointment?
		if (DateCompare (next, apptRec->repeat->repeatEndDate) > 0)
			return (false);

		ErrFatalDisplayIf ((DateToInt (next) < DateToInt (*dateP)),
			"Calculation error");
		}
	else
		{
		// Is the next occurrence before the start date of the appointment?
		if (DateCompare (next, apptRec->when->date) < 0)
			return (false);
		}

	*dateP = next;
	return (true);
}

/***********************************************************************
 *
 * FUNCTION:    	ApptNextRepeat
 *
 * DESCRIPTION:	This routine computes the next occurrence of a 
 *              	repeating appointment.
 *
 * PARAMETERS:		apptRec - a pointer to an appointment record
 *              	dateP   - passed:   date to start from
 *                       	 returned: date of next occurrence   
 *						searchForward	- true if searching for next occurrence
 *										  	- false if searching for most recent       
 *
 * RETURNED:		true if there is an occurrence of the appointment
 *						between the date passed and the appointment's end date
 *						(if searching forward) or start date (if searching
 *						backwards)
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			art	6/20/95	Initial Revision
 *			gap	9/25/00	Add capability to search backwards for the most
 *								recent occurrence of the event (needed for attention
 *								manager support)
 *
 ***********************************************************************/
static Boolean ApptNextRepeat (ApptDBRecordPtr apptRec, DatePtr dateP, Boolean searchForward)
{
	DateType date;
	
	date = *dateP;
	
	while (true)
		{
		// Compute the next time the appointment repeats.
		if (! FindNextRepeat (apptRec, &date, searchForward))
			return (false);

		// Check if the date computed is in the exceptions list.
		if (! IsException (apptRec, date))
			{
			*dateP = date;
			return (true);
			}
			
		DateAdjust (&date, (searchForward) ? 1 : -1);

		}		
}

/************************************************************
 *
 *  FUNCTION: ApptGetRecord
 *
 *  DESCRIPTION: Get a read-only record from the Appointment Database
 *
 *  PARAMETERS: database pointer
 *				    database index
 *				    database record
 *
 *  RETURNS: ##0 if successful, errorcode if not
 *
 *  CREATED: 1/25/95 
 *
 *  BY: Roger Flores
 *
 *************************************************************/
static Err ApptGetRecord (DmOpenRef dbP, UInt16 index, ApptDBRecordPtr r, 
	MemHandle * handleP)
{
	MemHandle					handle = NULL;
	ApptPackedDBRecordPtr 		src;
	Err							RetErrCode = errNone;

	handle = DmQueryRecord(dbP, index);
	ErrFatalDisplayIf(((RetErrCode = DmGetLastErr()) != errNone), "Error Querying record");
	if(handle != NULL && RetErrCode == errNone)
	{
		src = (ApptPackedDBRecordPtr) MemHandleLock (handle);

		if (DmGetLastErr())
		{
			*handleP = 0;
			return DmGetLastErr();
		}
		
		ApptUnpack(src, r);
		
		*handleP = handle;
	}	
	else
		*handleP = NULL;
	return RetErrCode;

}

/*
 * GetNumAppointments()
 */
UInt16 GetNumAppointments(DateType* pDate, TimeType* pTime, Char* str, UInt16 strLen, Char* nextApptTimeStr)
{
	
	DmOpenRef 					dbP = NULL;
	VoidHand					ApptList = NULL;
	ApptInfoPtr					appts;
	MemHandle					apptH = NULL;
	ApptDBRecordType 			r;
	UInt16						todayCount = 0;
	UInt16						count = 0;
	UInt16						i = 0;
	Boolean						foundFirstAppt = false;
	
	if (!pDate) return (count);
	
	dbP = DmOpenDatabaseByTypeCreator('DATA', kPalmOneCreatorIDCalendar, dmModeReadOnly);
	
	if (dbP)
	{
		ApptList = (VoidHand)ApptGetAppointments(dbP, *pDate, &todayCount);
		
		if ( (pTime) && (ApptList) )
		{
			appts = (ApptInfoPtr)MemHandleLock(ApptList);
			if (appts)
			{	
				for ( i = 0 ; i < todayCount ; i++ )
				{
					ApptGetRecord(dbP, appts[i].recordNum, &r, &apptH);
					
					if ( TimeCompare(r.when->startTime, *pTime) >= 0 )
					{
						
						if ( (str) && (!foundFirstAppt) )
						{
							foundFirstAppt = true;
							MemMove(str, r.description, strLen);
							str[strLen] = chrNull;
							
							if (nextApptTimeStr)
							{
								if (TimeCompare(r.when->startTime, *pTime) > 0)
								{
									UInt16		apptMins = ( r.when->startTime.hours * 60 ) + r.when->startTime.minutes;
									UInt16		nowMins = ( pTime->hours * 60 ) + pTime->minutes;
									
									UInt16		timeMinsToAppt = apptMins - nowMins;
									UInt16		hoursToAppt = timeMinsToAppt / 60;
									UInt16		minsToAppt = timeMinsToAppt % 60;
									
									StrPrintF(nextApptTimeStr, "[%ih %im] ", hoursToAppt, minsToAppt);	
								}
								else
								{
									StrCopy(nextApptTimeStr, "[Now] ");
								}
							}
						}
						
						count++;
					}
					MemHandleUnlock(apptH); // locked by ApptGetRecord()
				}
				MemHandleUnlock(ApptList);
				
				if (ApptList)
					MemHandleFree(ApptList);
			}
			 
		} 
		else
		{
			count = todayCount;
		}
		
		DmCloseDatabase(dbP);
	}	
	return (count);
	
} // GetNumAppointments()

/*
 * DateBook.c
 */