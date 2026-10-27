#pragma once
#include "Bot.h"
class StrategicalBot : public Bot
{
public:
	cpu_fsm<StrategicalBot>* m_pFsm;
	void Destroy() override;
	void InitializeBot() override;
	void StartGame() override;
	void Move() override;
	void Engage() override;
	void Smash();
	void Counter();
	void Serve();
	void Idle();
};

struct StateStrategicalBotDefend
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

struct StateStrategicalBotAttackTransition
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

struct StateStrategicalBotSmash
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

struct StateStrategicalBotCounter
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

struct StateStrategicalBotEngage
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

struct StateStrategicalBotTransition
{
	void OnEnter(StrategicalBot& cur, int from, void* pParam);
	void OnExecute(StrategicalBot& cur);
	void OnExit(StrategicalBot& cur, int to);
};

