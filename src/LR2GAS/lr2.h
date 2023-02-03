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

    inline int* bmsNotesNum;
    inline int* bmsTotal;

    inline GAUGE_TYPE* gaugeType;
    inline BATTLE_TYPE* battleType;
    inline char* selectPressed;
    inline double* gaugeValue;
    inline double* gaugeValueDisp;
    inline int* totalNotes;
    inline int* runningNotes;
    inline LAMP_TYPE* resultLamp;
    inline int* resultGraph;
    struct ReplayCmd
    {
        int time, type, value;
    };  typedef struct ReplayCmd ReplayCmd;
    inline ReplayCmd** replayData;
    inline int* replayIdx;
    inline int* replayStatus;
    inline char* gbattleFlag;
    inline int* expertType;

    inline int(__cdecl *pfAppendReplay) (int*, int, char, short);

    void initOffsets();

}

