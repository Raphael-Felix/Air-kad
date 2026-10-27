#pragma once
#include "Bot.h"
class CheaterBot : public Bot
{
public:
	cpu_fsm<CheaterBot>* m_pFsm;
	void InitializeBot() override;
	void Move() override;
	void Hesitate();
};

struct StateCheaterBotDefend
{
	void OnEnter(CheaterBot& cur, int from, void* pParam);
	void OnExecute(CheaterBot& cur);
	void OnExit(CheaterBot& cur, int to);
};

struct StateCheaterBotAttack
{
	void OnEnter(CheaterBot& cur, int from, void* pParam);
	void OnExecute(CheaterBot& cur);
	void OnExit(CheaterBot& cur, int to);
};

