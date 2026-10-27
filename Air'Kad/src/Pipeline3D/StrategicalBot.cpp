#include "pch.h"
#include "StrategicalBot.h"
#include "Pusher.h"
#include "Palet.h"

void StrategicalBot::Destroy()
{
	cpuEngine.Release(m_pEntity);
	cpuEngine.Release(m_pFsm);
}

void StrategicalBot::InitializeBot()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->Add<StateStrategicalBotDefend>();
	m_pFsm->Add<StateStrategicalBotTransition>();
	m_pFsm->Add<StateStrategicalBotSmash>();
	m_pFsm->Add<StateStrategicalBotCounter>();
	m_pFsm->Add<StateStrategicalBotEngage>();
	
	m_pFsm->ToState(CPU_ID(StateStrategicalBotDefend));

}

void StrategicalBot::StartGame()
{
	SetMaxSpeed(50);
	SetSpeed(10);
}

float CalculateAngle(XMFLOAT2 trajectory)
{
	XMFLOAT2 base = { 1, 0 };

	float crossProduct = base.x * trajectory.x + base.y * trajectory.y;

	float normBase = sqrt(base.x * base.x + base.y * base.y);
	float normtrayectory = sqrt(trajectory.x * trajectory.x + trajectory.y * trajectory.y);

	float crossNormProduct = normBase * normtrayectory;

	if (crossNormProduct == 0)
		return 0.0f;

	float angle = std::acos(crossProduct / crossNormProduct);

	return angle;
}

void StrategicalBot::Move()
{
	float dt = cpuTime.delta;
	CalculateTrajectory();

	GoToPosition({ m_finalPosition.x,m_finalPosition.y});

	if(GetPalet()->GetEntity()->transform.pos.z > 6)
	{
		GetEntity()->transform.dir.z = 0.0f;
		GetEntity()->transform.Move(dt * m_speed);
	}

	XMFLOAT2 direccionBall;

	direccionBall.x = GetPalet()->GetEntity()->transform.dir.x;
	direccionBall.y = GetPalet()->GetEntity()->transform.dir.z;

	if (GetPalet()->GetEntity()->transform.pos.z > 18)
	{
		if (CalculateAngle(GetPalet()->GetVelocity()) >= XM_PI/2)
			m_pFsm->ToState(CPU_ID(StateStrategicalBotSmash));
		else
			m_pFsm->ToState(CPU_ID(StateStrategicalBotCounter));
	}

	if(GetEntity()->transform.pos.z <= 20)
		GetEntity()->transform.pos.z += GetSpeed() * dt;
}

void StrategicalBot::Engage()
{
	m_pFsm->ToState(CPU_ID(StateStrategicalBotEngage));
}

void StrategicalBot::Smash()
{
	float dt = cpuTime.delta;

	GetEntity()->transform.dir.x = 0;

	if(GetEntity()->transform.pos.z >= 16)
		GetEntity()->transform.Move(GetSpeed()* dt);

	if (GetPalet()->GetEntity()->transform.pos.z < 17)
		m_pFsm->ToState(CPU_ID(StateStrategicalBotTransition));
}

void StrategicalBot::Counter()
{
	float dt = cpuTime.delta;
	GoToPosition({ GetFinalPosition().x - 0.15f, 22 });
	GetEntity()->transform.Move(GetSpeed() * dt);


	if (GetPalet()->GetEntity()->transform.pos.z < 17)
		m_pFsm->ToState(CPU_ID(StateStrategicalBotTransition));
}

void StrategicalBot::Serve()
{
	float dt = cpuTime.delta;
	
	if (GetEntity()->transform.pos.x >= GetPalet()->GetEntity()->transform.pos.x + 0.3f || GetEntity()->transform.pos.x <= GetPalet()->GetEntity()->transform.pos.x - 0.3f)
	{
		GoToPosition({ GetPalet()->GetEntity()->transform.pos.x, 0 });
		GetEntity()->transform.dir.z = 0;
	}
	else
		GoToPosition({ 0, GetPalet()->GetEntity()->transform.pos.z });

	GetEntity()->transform.Move(GetSpeed() * dt);

	if (cpu::ObbObb(GetEntity()->obb, GetPalet()->GetEntity()->obb))
		m_pFsm->ToState(CPU_ID(StateStrategicalBotDefend));
}

void StrategicalBot::Idle()
{
	float dt = cpuTime.delta;

	if (GetEntity()->transform.pos.z <= 22)
		GetEntity()->transform.Move(GetSpeed() * dt);

	if(GetPalet()->GetEntity()->transform.pos.z < 9)
		m_pFsm->ToState(CPU_ID(StateStrategicalBotDefend));
}

void StateStrategicalBotDefend::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
	cur.SetSpeed(25.0f);
}

void StateStrategicalBotDefend::OnExecute(StrategicalBot& cur)
{
	cur.Move();
}

void StateStrategicalBotDefend::OnExit(StrategicalBot& cur, int to)
{
}

void StateStrategicalBotAttackTransition::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
}

void StateStrategicalBotAttackTransition::OnExecute(StrategicalBot& cur)
{
}

void StateStrategicalBotAttackTransition::OnExit(StrategicalBot& cur, int to)
{

}

void StateStrategicalBotSmash::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
	cur.GoToPosition({ cur.GetFinalPosition().x - 0.15f, cur.GetFinalPosition().y - 0.15f });
	cur.SetSpeed(30.0f);
}

void StateStrategicalBotSmash::OnExecute(StrategicalBot& cur)
{
	cur.Smash();
}

void StateStrategicalBotSmash::OnExit(StrategicalBot& cur, int to)
{
	cur.SetSpeed(25.0f);
}

void StateStrategicalBotCounter::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
	cur.SetSpeed(20.0f);
}

void StateStrategicalBotCounter::OnExecute(StrategicalBot& cur)
{
	cur.Counter();
}

void StateStrategicalBotCounter::OnExit(StrategicalBot& cur, int to)
{
	cur.SetSpeed(25.0f);
}

void StateStrategicalBotEngage::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
	cur.SetSpeed(50.0f);
}

void StateStrategicalBotEngage::OnExecute(StrategicalBot& cur)
{
	cur.Serve();
}

void StateStrategicalBotEngage::OnExit(StrategicalBot& cur, int to)
{
	cur.SetSpeed(25.0f);
}

void StateStrategicalBotTransition::OnEnter(StrategicalBot& cur, int from, void* pParam)
{
	if (from == 1 || from == 2 || from == 3)
		cur.GetEntity()->transform.dir = { 0,0,1 };
}

void StateStrategicalBotTransition::OnExecute(StrategicalBot& cur)
{
	float dt = cpuTime.delta;

	cur.Idle();
}

void StateStrategicalBotTransition::OnExit(StrategicalBot& cur, int to)
{
	float dt = cpuTime.delta;

	cur.GetEntity()->transform.dir.z = -1;
}
