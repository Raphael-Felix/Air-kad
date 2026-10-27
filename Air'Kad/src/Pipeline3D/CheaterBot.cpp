#include "pch.h"
#include "CheaterBot.h"

void CheaterBot::InitializeBot()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->Add<StateCheaterBotDefend>();
	m_pFsm->Add<StateCheaterBotAttack>();

	m_pFsm->ToState(CPU_ID(StateCheaterBotDefend));
}

void CheaterBot::Move()
{
}

void CheaterBot::Hesitate()
{
}

void StateCheaterBotDefend::OnEnter(CheaterBot& cur, int from, void* pParam)
{
}

void StateCheaterBotDefend::OnExecute(CheaterBot& cur)
{
	cur.Move();
}

void StateCheaterBotDefend::OnExit(CheaterBot& cur, int to)
{
}

void StateCheaterBotAttack::OnEnter(CheaterBot& cur, int from, void* pParam)
{
}

void StateCheaterBotAttack::OnExecute(CheaterBot& cur)
{
	cur.Hesitate();
}

void StateCheaterBotAttack::OnExit(CheaterBot& cur, int to)
{
}