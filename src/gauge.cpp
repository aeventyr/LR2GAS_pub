#include "gauge.h"
#include "gas.h"

Gauge::Gauge(GAUGE_TYPE gaugeType)
{
	failed = false;
	this->gaugeType = gaugeType;
	double basePerNote = GAS::bmsTotal / GAS::notesNum;
	double hardMultplier = GAS::GetHealthMultiplier();
	switch (gaugeType)
	{
	case GAUGE_GROOVE:
		judgeGaugeIncrements[KPOOR] = -2.0;
		judgeGaugeIncrements[POOR] = -6.0;
		judgeGaugeIncrements[BAD] = -4.0;
		judgeGaugeIncrements[GOOD] = basePerNote / 2;
		judgeGaugeIncrements[GREAT] = basePerNote;
		judgeGaugeIncrements[PGREAT] = basePerNote;
		gaugeValue = 20.0;
		isSurvival = false;
		break;
	case GAUGE_EASY:
		judgeGaugeIncrements[KPOOR] = 0.8 * -2.0;
		judgeGaugeIncrements[POOR] = 0.8 * -6.0;
		judgeGaugeIncrements[BAD] = 0.8 * -4.0;
		judgeGaugeIncrements[GOOD] = 1.2 * basePerNote / 2;
		judgeGaugeIncrements[GREAT] = 1.2 * basePerNote;
		judgeGaugeIncrements[PGREAT] = 1.2 * basePerNote;
		gaugeValue = 20.0;
		isSurvival = false;
		break;
	case GAUGE_HARD:
		judgeGaugeIncrements[KPOOR] = -2.0 * hardMultplier;
		judgeGaugeIncrements[POOR] = -10.0 * hardMultplier;
		judgeGaugeIncrements[BAD] = -6.0 * hardMultplier;
		judgeGaugeIncrements[GOOD] = 0.1 / 2;
		judgeGaugeIncrements[GREAT] = 0.1;
		judgeGaugeIncrements[PGREAT] = 0.1;
		gaugeValue = 100.0;
		isSurvival = true;
		break;
	case GAUGE_HAZARD:
		judgeGaugeIncrements[KPOOR] = 0;
		judgeGaugeIncrements[POOR] = -100;
		judgeGaugeIncrements[BAD] = -100;
		judgeGaugeIncrements[GOOD] = 0;
		judgeGaugeIncrements[GREAT] = 0;
		judgeGaugeIncrements[PGREAT] = 0;
		gaugeValue = 100.0;
		isSurvival = true;
		break;
	case GAUGE_P_ATTACK:
		judgeGaugeIncrements[KPOOR] = -100;
		judgeGaugeIncrements[POOR] = -100;
		judgeGaugeIncrements[BAD] = -100;
		judgeGaugeIncrements[GOOD] = -100;
		judgeGaugeIncrements[GREAT] = -1.0;
		judgeGaugeIncrements[PGREAT] = 0.1;
		gaugeValue = 100.0;
		isSurvival = true;
		break;
	case GAUGE_G_ATTACK:
		judgeGaugeIncrements[KPOOR] = -2.0 * hardMultplier;
		judgeGaugeIncrements[POOR] = -10.0 * hardMultplier;
		judgeGaugeIncrements[BAD] = -6.0 * hardMultplier;
		judgeGaugeIncrements[GOOD] = 0.1;
		judgeGaugeIncrements[GREAT] = -1.0;
		judgeGaugeIncrements[PGREAT] = -10.0 * hardMultplier;
		gaugeValue = 100.0;
		isSurvival = true;
		break;
	default:
		gaugeValue = 0.0;
		isSurvival = true;

	}
}

void Gauge::onJudge(int judge)
{
	if (failed)
		return;

	bool applyLowHealthMultiplier = (int(gaugeValue) / 2 * 2 <= 30)
		&& (gaugeType != GAUGE_GROOVE && gaugeType != GAUGE_EASY)
		&& judge <= BAD;
	if (applyLowHealthMultiplier)
		gaugeValue += 0.6 * judgeGaugeIncrements[judge];
	else
		gaugeValue += judgeGaugeIncrements[judge];

	if (gaugeValue <= 2.0)
	{
		if (isSurvival)
			failed = true;
		else
			gaugeValue = 2.0;
	}
	if (gaugeValue >= 100.0)
		gaugeValue = 100.0;
	if (gaugeValue <= 0.0)
		gaugeValue = 0.0;
}

void Gauge::recordGraphSample()
{
	recordGraphSample((int)gaugeValue);
}

void Gauge::recordGraphSample(int value)
{
	if (graphArrIdx >= GRAPH_SAMPLE_COUNT)
		return;
	graph[graphArrIdx] = value;
	graphArrIdx++;
}

void Gauge::fillGraph()
{
	while (graphArrIdx < GRAPH_SAMPLE_COUNT)
	{
		graph[graphArrIdx] = graph[graphArrIdx-1];
		graphArrIdx++;
	}
}

GAUGE_TYPE Gauge::getType()
{
	return gaugeType;
}

double Gauge::getValue()
{
	return gaugeValue;
}

bool Gauge::validForDisplay()
{
	if (isSurvival)
		return !failed;
	else
		return gaugeValue >= 80.0;
}
