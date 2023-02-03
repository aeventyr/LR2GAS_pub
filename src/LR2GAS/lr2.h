#pragma once

namespace LR2
{
    enum GAUGE_TYPE
    {
        GAUGE_GROOVE = 0,
        GAUGE_HARD,
        GAUGE_HAZARD,
        GAUGE_EASY,
        GAUGE_P_ATTACK,
        GAUGE_G_ATTACK,
    };
    inline const int GAUGE_COUNT = 6;
    inline const char* GAUGE_TYPE_STR[] = {
        "Groove",
        "Hard",
        "Hazard",
        "Easy",
        "P-Attack",
        "G-Attack"
    };

    enum JUDGE_TYPE
    {
        KPOOR = 0,
        POOR,
        BAD,
        GOOD,
        GREAT,
        PGREAT
    };
    inline const int JUDGE_COUNT = 6;

    enum LAMP_TYPE
    {
        LAMP_NOPLAY = 0,
        LAMP_FAILED,
        LAMP_EASY,
        LAMP_GROOVE,
        LAMP_HARD,
        LAMP_FULLCOMBO
    };
    inline const int LAMP_COUNT = 6;
    inline const char* LAMP_TYPE_STR[] = {
        "No Play",
        "Failed",
        "Easy",
        "Groove",
        "Hard",
        "Full Combo"
    };

    enum BATTLE_TYPE
    {
        BATTLE_OFF = 0,
        BATTLE_BATTLE,
        BATTLE_DBATTLE,
        BATTLE_SP2DP,
        BATTLE_GBATTLE
    };
    inline const int GRAPH_SAMPLE_COUNT = 1000;

    inline int* bmsNotesNum = (int*)0xDC27C;
    inline int* bmsTotal = (int*)0xDC28C;

    inline GAUGE_TYPE* gaugeType = (GAUGE_TYPE*)0xFF840;
    inline BATTLE_TYPE* battleType = (BATTLE_TYPE*)0xFF884;
    inline char* selectPressed = (char*)0x11F5D1;
    inline double* gaugeValue = (double*)0x197200;
    inline double* gaugeValueDisp = (double*)0x197210;
    inline int* totalNotes = (int*)0x197278;
    inline int* runningNotes = (int*)0x1972A4;
    inline LAMP_TYPE* resultLamp = (LAMP_TYPE*)0x1972A8;
    inline int* resultGraph = (int*)0x1973F4;
    struct ReplayCmd
    {
        int time, type, value;
    };  typedef struct ReplayCmd ReplayCmd;
    inline ReplayCmd** replayData = (ReplayCmd**)(0x19F170);
    inline int* replayIdx = (int*)0x19F178;
    inline int* replayStatus = (int*)0x19F17C;
    inline char* gbattleFlag = (char*)0x19F561;
    inline int* expertType = (int*)0x19F688;

    inline auto pfAppendReplay =
        (int(__cdecl*) (int*, int, char, short))(0x4C0C00);

}

