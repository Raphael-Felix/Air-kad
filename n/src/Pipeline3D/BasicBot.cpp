#include "pch.h"
#include "BasicBot.h"
#include "Palet.h"

void BasicBot::Destroy()
{
	cpuEngine.Release(m_pEntity);
	cpuEngine.Release(m_pFsm);
}

void BasicBot::InitializeBot()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->Add<StateBasicBotDefend>();
	m_pFsm->Add<StateBasicBotHesitate>();

	m_pFsm->ToState(CPU_ID(StateBasicBotDefend));
}

void BasicBot::StartGame()
{
}

void BasicBot::Move()
{
	float dt = cpuTime.delta;

	GetEntity()->transform.pos.x = GetPalet()->GetEntity()->transform.pos.x;
}

void BasicBot::Engage()
{
}

void BasicBot::Hesitate()
{
	float dt = cpuTime.delta;

	GetEntity()->transform.pos.x += 0;
	IncrementDt(0.5f);

	if (GetDt() > 0.5f)
	{
		SetDirection(-GetDirection());
		ResetDt();
		m_pFsm->ToState(CPU_ID(StateBasicBotDefend));
	}
}

void StateBasicBotDefend::OnEnter(BasicBot& cur, int from, void* pParam)
{
}

void StateBasicBotDefend::OnExecute(BasicBot& cur)
{
	cur.Move();
}

void StateBasicBotDefend::OnExit(BasicBot& cur, int to)
{
}

void StateBasicBotHesitate::OnEnter(BasicBot& cur, int from, void* pParam)
{
}

void StateBasicBotHesitate::OnExecute(BasicBot& cur)
{
	cur.Hesitate();
}

void StateBasicBotHesitate::OnExit(BasicBot& cur, int to)
{
}