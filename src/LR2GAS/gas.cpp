#include "gas.h"

unsigned int GAS::notesNum = 0;
double GAS::bmsTotal = 300;
bool GAS::expertMode = false;
GAUGE_TYPE GAS::initialGauge = (GAUGE_TYPE) -1;
std::vector<Gauge> GAS::gaugeArr = std::vector<Gauge>();
unsigned int GAS::gaugeCycleIdx = 0;


void GAS::Init()
{
	gaugeArr.clear();
    gaugeArr.emplace_back(Gauge(GAUGE_P_ATTACK));
    gaugeArr.emplace_back(Gauge(GAUGE_HAZARD));
    gaugeArr.emplace_back(Gauge(GAUGE_HARD));
    gaugeArr.emplace_back(Gauge(GAUGE_GROOVE));
    gaugeArr.emplace_back(Gauge(GAUGE_EASY));
}

Gauge& GAS::PickDisplay()
{
    unsigned int i = 0;
    for (; i < gaugeArr.size(); i++)
        if (gaugeArr[i].getType() == initialGauge)
            break;
    for (; i < gaugeArr.size(); i++)
        if (gaugeArr[i].validForDisplay())
            return gaugeArr[i];
    return gaugeArr[gaugeArr.size() - 1];
}

double GAS::GetHealthMultiplier()
{
	double tNotes;
	// low notes multiplier
	if (notesNum < 20)
		tNotes = 10.0;
	else if (notesNum < 30)
		tNotes = 14.0 - (notesNum / 5.0);
	else if (notesNum < 60)
		tNotes = 9.0 - (notesNum / 15.0);
	else if (notesNum < 125)
		tNotes = 5.0 - ((notesNum - 60.0) / 65.0);
	else if (notesNum < 250)
		tNotes = 5.0 - (notesNum / 125.0);
	else if (notesNum < 500)
		tNotes = 4.0 - (notesNum / 250.0);
	else if (notesNum < 1000)
		tNotes = 3.0 - (notesNum / 500.0);
	else
		tNotes = 1.0;
	// low total multiplier
	double tTotal = (bmsTotal <= 80) ? (10) : (160.0 / (bmsTotal - 80));
	return std::max(tTotal, tNotes);
}