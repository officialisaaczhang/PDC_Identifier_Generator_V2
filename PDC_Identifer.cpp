// PDC_identifier.cpp : Defines the exported functions for the DLL application.

#include "pch.h"
#include "PDC_identifier.h"
#include <windows.h>
#include<tchar.h>
#include <comdef.h>
// #include <algorithm>
#pragma comment(lib,"user32.lib")

using namespace EuroScopePlugIn;
using namespace std;

PDC_identifier* gpPDC_identifier = NULL;

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance) {
	*ppPlugInInstance = gpPDC_identifier = new PDC_identifier;
}

void __declspec (dllexport) EuroScopePlugInExit(void) {
	delete gpPDC_identifier;
}

const int TAG_ITEM_PDC = 5000;
const int TAG_FUNC_PDC_REQ = 5001;
const int TAG_FUNC_PDC_RESET = 5002;

const int TAG_ITEM_FLAG = 5100;
const int TAG_FUNC_FLAG_SET = 5101;

// const int TAG_ITEM_DEBUG = 5999;

COLORREF pdc_Code = RGB(64, 255, 0);
COLORREF pdc_Sent = RGB(255, 214, 51);
COLORREF pdc_Read = pdc_Code;
COLORREF pdc_Delete = RGB(255, 71, 26);

const char* sync_Com;
const char* target_Callsign;

// vector<const char*> online_ctr = {};

PDC_identifier::PDC_identifier() : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
	"PDC_identifier",
	"2.1.2.hf1",
	"Isaac Zhang",
	"Free to be distributed as source code") {

	RegisterTagItemType("PDC", TAG_ITEM_PDC);
	RegisterTagItemFunction("Grant PDC", TAG_FUNC_PDC_REQ);
	RegisterTagItemFunction("Reset", TAG_FUNC_PDC_RESET);

	RegisterTagItemType("PDC Flag", TAG_ITEM_FLAG);
	RegisterTagItemFunction("Set Status", TAG_FUNC_FLAG_SET);

	// RegisterTagItemType("Debug", TAG_ITEM_DEBUG);

	DisplayUserMessage("PDC_identifier", "PDC_identifier", "v2.1.2.hf1 initialized.", false, true, true, true, false);
	DisplayUserMessage("PDC_identifier", "PDC_identifier", "Don't forget to sync Code and Status if you are in a lower position before you assign any new PDC.", false, true, true, true, false);
	DisplayUserMessage("PDC_identifier", "PDC_identifier", "Simply type 'syncPDC' as a private message to another controller.", false, true, true, true, false);
}

PDC_identifier::~PDC_identifier()
{
}

EuroScopePlugIn::CRadarScreen* PDC_identifier::OnRadarScreenCreated(const char* sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated)
{
	return nullptr;
}

void PDC_identifier::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
	EuroScopePlugIn::CRadarTarget RadarTarget,
	int ItemCode,
	int TagData,
	char sItemString[16],
	int* pColorCode,
	COLORREF* pRGB,
	double* pFontSize) {

	if (strcmp((char*)FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(8), "") != 0
		&& strcmp((char*)FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(7), "") != 0) {

		// controller_Sync(FlightPlan);

		// Syncs existing PDC and Status to the controller who requested the syncing.
		if (sync_Com == "syncPDC") {
			DisplayUserMessage("PDC_identifier", "PDC", "Flightstrip pushed", false, true, true, false, false);
			CFlightPlan currentfp = FlightPlanSelectFirst();

			while (currentfp.IsValid() == TRUE) {
				currentfp.PushFlightStrip(target_Callsign);;
				currentfp = FlightPlanSelectNext(currentfp);
			}
		}
	}

		if (ItemCode == TAG_ITEM_PDC) {
			const char* ID = (char*)FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(8);
			strcpy_s(sItemString, 16, ID);
			*pColorCode = TAG_COLOR_RGB_DEFINED;
			COLORREF c_code = pdc_Code;
			*pRGB = c_code;
		}

		if (ItemCode == TAG_ITEM_FLAG) {
			const char* pdc_STS = FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(7);

			// Set PDC Status tracker shape and color according to status.
			if (strcmp(pdc_STS, "ASN") == 0) {
				strcpy_s(sItemString, 16, "\x00A4");
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = pdc_Sent;
			}
			else if (strncmp(pdc_STS, "RBC", 3) == 0) {
				strcpy_s(sItemString, 16, "\x00A4");
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = pdc_Read;
			}
			else if (strncmp(pdc_STS, "DEL", 3) == 0) {
				strcpy_s(sItemString, 16, "\x00A4");
				*pColorCode = TAG_COLOR_RGB_DEFINED;
				*pRGB = pdc_Delete;
			}
			else {
				strcpy_s(sItemString, 16, "\x00AC");
				*pColorCode = TAG_COLOR_DEFAULT;
			}
		}

		/* if (ItemCode == TAG_ITEM_DEBUG) {
			const char* text = (char*)FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(7);
			strcpy_s(sItemString, 16, text);
		
		} */
}

inline void PDC_identifier::OnFunctionCall(int FunctionId, const char* sItemString, POINT Pt, RECT Area)
{
	CFlightPlan fp;
	fp = FlightPlanSelectASEL();
	CFlightPlanData data;
	data = fp.GetFlightPlanData();
	CFlightPlanControllerAssignedData cData;
	cData = fp.GetControllerAssignedData();
	CController Atc;
	Atc = ControllerMyself();

	if (FunctionId == TAG_FUNC_PDC_REQ) {

		// Generate code
		string Code_Given = code_Gen();
		const char* code = Code_Given.c_str();

		const char* str = cData.GetFlightStripAnnotation(8);
		size_t Size = strlen(str);

		// If no code has been previously assigned, assign a new code.
		if (Size != 4) {

			// Preview and copy to send
			preview_Window(fp, data, cData, Atc, code);

		}
		else {
			// If a code exists, then confirm to overwrite.
			if (overwrite_Window() == 1) {
				preview_Window(fp, data, cData, Atc, code);
			}
		}
	}

	// Confirm to delete existing code.
	if (FunctionId == TAG_FUNC_PDC_RESET && strcmp(cData.GetFlightStripAnnotation(8), "") != 0) {
		if (delete_Window() == 1) {
			// Reminder to revert back to voice procedures.
			fp.GetControllerAssignedData().SetFlightStripAnnotation(8, "VOIC");

			// Set PDC Status tracker to "DEL" if identifier is deleted.
			fp.GetControllerAssignedData().SetFlightStripAnnotation(7, "DEL");

			// Pushes updated flightstrip to everyone else supposedly.
			data_Sync(fp);
		}
	}

	if (FunctionId == TAG_FUNC_FLAG_SET) {

		//Set PDC Status tracker to "Read Back Correct" only if the previous status shows "ASN".
		if (strncmp(fp.GetControllerAssignedData().GetFlightStripAnnotation(7), "ASN", 3) == 0) {
			fp.GetControllerAssignedData().SetFlightStripAnnotation(7, "RBC");

			// Pushes updated flightstrip to everyone else supposedly.
			data_Sync(fp);
		}
	}
}

// Delete confirm window
int PDC_identifier::delete_Window() {

	const int result = MessageBox(NULL, "This will delete the existing code. Please coordinate and confirm.", "Existing PDC Identifier", MB_YESNOCANCEL);

	int output;

	switch (result)
	{
	case IDYES:
		output = 1;
		break;
	case IDNO:
		output = 0;
		break;
	case IDCANCEL:
		output = 2;
		break;
	}

	return output;
}

// Overwriting warning window
int PDC_identifier::overwrite_Window() {

	const int result = MessageBox(NULL, "This will overwrite the existing code. Please coordinate and confirm.", "Existing PDC Identifier", MB_YESNOCANCEL);

	int output;

	switch (result)
	{
	case IDYES:
		output = 1;
		break;
	case IDNO:
		output = 0;
		break;
	case IDCANCEL:
		output = 2;
		break;
	}

	return output;
}

// Preview of the PDC constucted and ready to send
inline void PDC_identifier::preview_Window(CFlightPlan fp, CFlightPlanData data, CFlightPlanControllerAssignedData cData, CController Atc, const char* code) {

	string PDC_preview = PDC_construct(fp, data, cData, Atc, code);

	const int result = MessageBoxA(NULL,PDC_preview.c_str(), "Copy to Clipboard?", MB_YESNO);

	switch (result)
	{
	case IDYES:
		fp.GetControllerAssignedData().SetFlightStripAnnotation(8, code);

		// Set Status tracker to "ASN" after assigning/re-assigning a PDC.
		fp.GetControllerAssignedData().SetFlightStripAnnotation(7, "ASN");
		
		// Pushes updated flightstrip to everyone else supposedly.
		data_Sync(fp);

		clipBoard_O(".msg " + string(fp.GetCallsign()) + " " + PDC_preview);
		break;
	case IDNO:
		break;
	}

}

// PDC Constructor
string PDC_identifier::PDC_construct(CFlightPlan fp, CFlightPlanData data, CFlightPlanControllerAssignedData cData, CController Atc, const char* code) {

	// PDC data
	string Callsign = fp.GetCallsign();
	string DEP = data.GetOrigin();
	string ARR = data.GetDestination();
	string FALTT = flightLevel(fp.GetFinalAltitude());
	string SID = data.GetSidName();
	string DRWY = data.GetDepartureRwy();
	string SQWK = cData.GetSquawk();
	const char* TYPE = data.GetAircraftInfo();
	string ROUTE = data.GetRoute();
	string PDC_IDENT = code;
	const char* PDC_GIVER = Atc.GetCallsign();
	string PDC_FREQ = to_string(Atc.GetPrimaryFrequency());

	// PDC message construction
	// string message_old = "PDC - CALLSIGN: " + Callsign + " - DEP: " + DEP + " - ARR: " + ARR + " - FLIGHT PLANNED ROUTE - ALTITUDE: " + FALTT + " - SID: " + SID + " - DEP RWY: " + DRWY + " - SQUAWK: " + SQWK + " - DEP HDG & ALT AS PUBLISHED ON SID - IDENTIFIER: " + assignedIdent[fp.GetCallsign()] + " - READ BACK IDENTIFIER AND SID ON FREQUENCY";
	string message = "PDC - " + Callsign + " SQUAWK " + SQWK + " " + DEP + " - " + TYPE + " - " + FALTT + " - " + ROUTE +
		" - USE SID " + SID + " - DEP RWY " + DRWY + " DESTINATION " + ARR + " CONTACT " + PDC_GIVER + " " + PDC_FREQ.substr(0,7) + " WITH IDENTIFIER " + PDC_IDENT + " - END OF MESSAGE";
	return message;
}

// Random PDC Identifier Generator
string PDC_identifier::code_Gen()
{
	srand(time(NULL));
	string num1 = to_string(rand() % 10);
	string num2 = to_string(rand() % 10);
	string num3 = to_string(rand() % 10);
	char letter = 'A' + rand() % 26;
	string code = num1 + num2 + num3 + letter;
	return code;
}

// Flightlevel Expression Converter
string PDC_identifier::flightLevel(int alt) {
	if (alt >= 18000) {
		return "FL" + to_string(alt).substr(0, 3);
	}
	else {
		return to_string(alt);
	}
}

// Clipboard Output
void PDC_identifier::clipBoard_O(string message) {
	const char* output = message.c_str();
	const size_t len = strlen(output) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

// Syncs data when new PDC issued/readback status is updated.
void PDC_identifier::data_Sync(CFlightPlan fp) {
	const char* result = "";
	CController current = ControllerSelectFirst();

	while (current.IsValid() == TRUE) {
		if (current.IsOngoingAble()) {
			fp.PushFlightStrip(current.GetCallsign());
			DisplayUserMessage("PDC_identifier", "PDC", "Flightstrip broadcasted", false, true, true, false, false);
		}
		current = ControllerSelectNext(current);
	}
}
/*
void PDC_identifier::controller_Sync(CFlightPlan fltpln) {

	CController current = ControllerSelectFirst();

	while (current.IsValid() == TRUE) {
		if (current.IsOngoingAble() == TRUE) {

			if ((find(online_ctr.begin(), online_ctr.end(), current.GetCallsign()) != online_ctr.end())) {

			}
			else {
				// DisplayUserMessage("PDC_identifier","PDC", current.GetCallsign(), false, true, true, false, false);
				online_ctr.push_back(current.GetCallsign());
				fltpln.PushFlightStrip(current.GetCallsign());
				// DisplayUserMessage("PDC_identifier", "PDC", "Flightstrip pushed", false, true, true, false, false);
			}
		}
		current = ControllerSelectNext(current);
	}
} */

/* inline void PDC_identifier::OnControllerDisconnect(CController Controller) {
	if ((find(online_ctr.begin(), online_ctr.end(), Controller.GetCallsign()) != online_ctr.end())) {
		// remove(online_ctr.begin(), online_ctr.end(), Controller.GetCallsign());
	}
} */

// Manual syncing of existing PDC and Status. Done when a new controller logs on.
inline void PDC_identifier::OnCompilePrivateChat(const char* sSenderCallsign, const char* sReceiverCallsign, const char* sChatMessage) {
	if (sChatMessage = "syncPDC") {
		sync_Com = sChatMessage;
		target_Callsign = sSenderCallsign;
	}
}