#pragma once

#include <vector>
#include <algorithm>

#include "lr2.h"
#include "gauge.h"

using namespace LR2;

class GAS
{
public:
    static unsigned int notesNum;
    static double bmsTotal;
    static bool expertMode;
    static GAUGE_TYPE initialGauge;
    static std::vector<Gauge> gaugeArr;
    static unsigned int gaugeCycleIdx;

    static void Init();
    static Gauge& PickDisplay();
    static double GetHealthMultiplier();
};