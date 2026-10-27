#pragma once
#include "Bot.h"
class MechanicalBot : public Bot
{
private:
	cpu_fsm<MechanicalBot>* m_pFsm;
	bool m_hasEngaged = false;

	float m_waitTimer = 0.0f;
	float m_waitDuration = 0.0f;

public:
	void Destroy() override;

	void InitializeBot() override;
	void StartGame() override;

	void Engage() override;
	void Move() override;

	cpu_fsm<MechanicalBot>* GetFsm() { return m_pFsm; }

	bool HasEngaged() { return m_hasEngaged; }
	void SetEngaged(bool engage) { m_hasEngaged = engage; }

	void UpdateWaitTimer() { m_waitTimer += cpuTime.delta; }

	float GetWaitTimer() { return m_waitTimer; }
	float GetWaitDuration() { return m_waitDuration; }

	void ResetWaitTimer() { m_waitTimer = 0.0f; }
	void SetWaitDuration(float waitDuration) { m_waitDuration = waitDuration; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct StateMechanicalBotGlobal
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};

struct StateMechanicalBotInactive
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam) {};
	void OnExecute(MechanicalBot& cur) {};
	void OnExit(MechanicalBot& cur, int to) {};
};

struct StateMechanicalBotIdle
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};

struct StateMechanicalBotDefend
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};

struct StateMechanicalBotAttack
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};

struct StateMechanicalBotWallBounceAttack
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};

struct StateMechanicalBotWait
{
	void OnEnter(MechanicalBot& cur, int from, void* pParam);
	void OnExecute(MechanicalBot& cur);
	void OnExit(MechanicalBot& cur, int to);
};