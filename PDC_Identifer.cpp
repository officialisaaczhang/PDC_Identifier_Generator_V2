// PDC_identifier.cpp : Defines the exported functions for the DLL application.

#include "pch.h"
#include "PDC_identifier.h"
#include <windows.h>
#include<tchar.h>
#include <cmath>
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
string PDC_code;

PDC_identifier::PDC_identifier() : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
	"PDC_identifier",
	"2.0.0",
	"Isaac Zhang",
	"Free to be distributed as source code") {

	RegisterTagItemType("PDC", TAG_ITEM_PDC);
	RegisterTagItemFunction("Grant PDC", TAG_FUNC_PDC_REQ);
	RegisterTagItemFunction("Reset", TAG_FUNC_PDC_RESET);

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

		if (ItemCode == TAG_ITEM_PDC) {
			const char* ID = (char*)FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(8);
			strcpy_s(sItemString, 16, ID);
	}
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
	if (FunctionId == TAG_FUNC_PDC_RESET && delete_Window() == 1) {
		fp.GetControllerAssignedData().SetFlightStripAnnotation(8, "");
	}

}

// Delete confirm window
int PDC_identifier::delete_Window() {

	const int result = MessageBox(NULL, L"This will delete the existing code. Please coordinate and confirm.", L"Existing PDC Identifier", MB_YESNOCANCEL);

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

	const int result = MessageBox(NULL, L"This will overwrite the existing code. Please coordinate and confirm.", L"Existing PDC Identifier", MB_YESNOCANCEL);

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
		clipBoard_O(PDC_preview);
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
	string message = "PDC - " + Callsign + " " + SQWK + " " + DEP + " - " + TYPE + " - " + FALTT + " - " + ROUTE +
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

inline void PDC_identifier::OnFlightPlanControllerAssignedDataUpdate(CFlightPlan FlightPlan, int DataType)
{
	PDC_code = FlightPlan.GetControllerAssignedData().GetFlightStripAnnotation(8);
	// if (assignedIdent.count(FlightPlan.GetCallsign())) {
		// string newSratch = FlightPlan.GetControllerAssignedData().GetScratchPadString();
		// if (newSratch.length() == 4) {
			// assignedIdent[FlightPlan.GetCallsign()].swap(newSratch);
		// }
	// }
	// else {
		// assignedIdent.insert({ FlightPlan.GetCallsign(), FlightPlan.GetControllerAssignedData().GetScratchPadString() });
	// }
}