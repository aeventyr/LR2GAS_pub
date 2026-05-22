#include <Windows.h>
#include <thread>

#include "lr2.h"

#include <LR2Mem/LR2Bindings.hpp>

using namespace LR2;

void LR2::matchOffsets()
{
    while(!LR2::isInit)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LR2::game& game = *LR2::pGame;

    gaugeType = (GAUGE_TYPE *)(&game.config.play.gaugeOption[0]);
    battleType = (BATTLE_TYPE *)(&game.config.play.battle);
    selectPressedP1 = (char *)(&game.KeyInput.p1_buttonInput[13]);
    selectPressedP2 = (char *)(&game.KeyInput.p2_buttonInput[13]);
    gaugeValue = &game.gameplay.player[0].HP;
    gaugeValueDisp = &game.gameplay.player[0].HP_print;
    totalNotes = &game.gameplay.player[0].totalnotes;
    runningNotes = &game.gameplay.player[0].note_current;
    resultLamp = (LAMP_TYPE *)(&game.gameplay.player[0].clearType);
    resultGraph = (int *)(&game.gameplay.statgraph[0].hp);
    replayData = (ReplayCmd **)(&game.gameplay.replay.data);
    replayIdx = &game.gameplay.replay.count;
    replayStatus = &game.gameplay.replay.status;
    gbattleFlag = &game.gameplay.ghostBattle;
    expertType = &game.gameplay.courseType;
}
