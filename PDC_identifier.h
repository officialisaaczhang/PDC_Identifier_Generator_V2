#pragma once
#include <EuroScopePlugIn.h>
#include <string>
#include <random>
#include <time.h>
#include <map>

using namespace EuroScopePlugIn;
using namespace std;

class PDC_identifier :
	public EuroScopePlugIn::CPlugIn {
public:

	map<string, string> assignedIdent;

	PDC_identifier();
	virtual ~PDC_identifier();
	EuroScopePlugIn::CRadarScreen* OnRadarScreenCreated(const char* sDisplayName, bool NeedRadarContent, bool GeoReferenced, bool CanBeSaved, bool CanBeCreated);
	
	virtual void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
		EuroScopePlugIn::CRadarTarget RadarTarget,
		int ItemCode,
		int TagData,
		char sItemString[16],
		int* pColorCode,
		COLORREF* pRGB,
		double* pFontSize);

	inline virtual void OnFunctionCall(int FunctionId,
		const char* sItemString,
		POINT Pt,
		RECT Area);

	virtual string code_Gen();
	string flightLevel(int alt);
	virtual int delete_Window();
	virtual int overwrite_Window();

	inline virtual void preview_Window(CFlightPlan fp, CFlightPlanData data, CFlightPlanControllerAssignedData cData, CController Atc, const char* code);

	virtual string PDC_construct(CFlightPlan fp, CFlightPlanData data, CFlightPlanControllerAssignedData cData, CController Atc, const char* code);

	virtual void clipBoard_O(string message);

	inline virtual void OnFlightPlanControllerAssignedDataUpdate(CFlightPlan FlightPlan,
		int DataType);
};