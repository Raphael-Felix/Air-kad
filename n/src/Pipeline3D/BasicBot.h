#pragma once
#include "Bot.h"
class BasicBot : public Bot
{
public:
	cpu_fsm<BasicBot>* m_pFsm;
	void Destroy() override;
	void InitializeBot() override;
	void StartGame() override;
	void Move() override;
	void Engage() override;
	void Hesitate();
};

struct StateBasicBotDefend
{
	void OnEnter(BasicBot& cur, int from, void* pParam);
	void OnExecute(BasicBot& cur);
	void OnExit(BasicBot& cur, int to);
};

struct StateBasicBotHesitate
{
	void OnEnter(BasicBot& cur, int from, void* pParam);
	void OnExecute(BasicBot& cur);
	void OnExit(BasicBot& cur, int to);
};