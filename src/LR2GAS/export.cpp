#include "gas.h"

struct GaugeGraphs {
	int groove[1000] = {};
	int hard[1000] = {};
	int hazard[1000] = {};
	int easy[1000] = {};
	int pattack[1000] = {};
	int gattack[1000] = {};
};

extern "C" __declspec(dllexport) GaugeGraphs __cdecl GetGaugeGraphs() {
	GaugeGraphs graphs;
	for (auto& gauge : GAS::gaugeArr) {
		switch (gauge.getType()) {
		case GAUGE_GROOVE:
			memcpy(&graphs.groove, &gauge.graph, sizeof(graphs.groove));
			break;
		case GAUGE_HARD:
			memcpy(&graphs.hard, &gauge.graph, sizeof(graphs.hard));
			break;
		case GAUGE_HAZARD:
			memcpy(&graphs.hazard, &gauge.graph, sizeof(graphs.hazard));
			break;
		case GAUGE_EASY:
			memcpy(&graphs.easy, &gauge.graph, sizeof(graphs.easy));
			break;
		case GAUGE_P_ATTACK:
			memcpy(&graphs.pattack, &gauge.graph, sizeof(graphs.pattack));
			break;
		case GAUGE_G_ATTACK:
			memcpy(&graphs.gattack, &gauge.graph, sizeof(graphs.gattack));
			break;
		}
	}
	return graphs;
}