#include "pch.h"
#include "MechanicalBot.h"
#include "Palet.h"

void MechanicalBot::Destroy()
{
	cpuEngine.Release(m_pEntity);
	cpuEngine.Release(m_pFsm);
}

void MechanicalBot::InitializeBot()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->SetPostGlobal<StateMechanicalBotGlobal>();

	m_pFsm->Add<StateMechanicalBotInactive>();
	m_pFsm->Add<StateMechanicalBotIdle>();
	m_pFsm->Add<StateMechanicalBotDefend>();
	m_pFsm->Add<StateMechanicalBotAttack>();
	m_pFsm->Add<StateMechanicalBotWallBounceAttack>();
	m_pFsm->Add<StateMechanicalBotWait>();

	m_pFsm->ToState(CPU_ID(StateMechanicalBotInactive));

	SetSpeed(2.0f);
	SetMaxSpeed(15.0f);
}

void MechanicalBot::StartGame()
{
	m_pFsm->ToState(CPU_ID(StateMechanicalBotIdle));
}

void MechanicalBot::Engage()
{
	m_pFsm->ToState(CPU_ID(StateMechanicalBotWallBounceAttack));
}

void MechanicalBot::Move()
{
	float dt = cpuTime.delta;

	IsCollidingBounds(m_pEntity->transform.pos.x, m_pEntity->transform.pos.y);

	GetEntity()->transform.Move(dt * m_speed);

	if (m_destinationSet)
	{
		XMFLOAT2 curPosXZ = { GetEntity()->transform.pos.x, GetEntity()->transform.pos.z };

		XMVECTOR curVectorXZ = XMLoadFloat2(&curPosXZ);
		XMVECTOR destVectorXZ = XMLoadFloat2(&m_destXZ);

		XMVECTOR curDestVector = destVectorXZ - curVectorXZ;

		float norm = sqrt(pow(XMVectorGetX(curDestVector), 2.0f) + pow(XMVectorGetY(curDestVector), 2.0f));

		if (norm < 0.2f + (m_speed / m_maxSpeed) * 0.10f) // Ajuster selon la vitesse
		{
			GetEntity()->transform.pos.x = m_destXZ.x;
			GetEntity()->transform.pos.z = m_destXZ.y;

			GetEntity()->transform.dir = { 0.0f, 0.0f, 0.0 };

			m_destinationSet = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotGlobal::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
}

void StateMechanicalBotGlobal::OnExecute(MechanicalBot& cur)
{
	Palet* palet = cur.GetPalet();

	cur.Move();
}

void StateMechanicalBotGlobal::OnExit(MechanicalBot& cur, int to)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotIdle::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
	int side = rand() % 100;
	if (side % 2)
		side = 1;
	else
		side = -1;

	cur.SetSpeed(10.0f);

	cur.GoToPosition({ (side * 7.0f) / 2 - (side * 0.75f), 21.5f});
}

void StateMechanicalBotIdle::OnExecute(MechanicalBot& cur)
{
	Palet* palet = cur.GetPalet();

	if (palet->GetEntity()->transform.pos.z >= 17.0f || palet->GetVelocity().y > 20.0f && palet->GetEntity()->transform.pos.z <= 13.0f)
	{
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotDefend));
		return;
	}

	if (palet->GetVelocity().y < 20.0f)
	{
		if (palet->GetEntity()->transform.pos.z >= 15.0f)
		{
			cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotAttack));
			return;
		}
	}
}

void StateMechanicalBotIdle::OnExit(MechanicalBot& cur, int to)
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotDefend::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
	Palet* palet = cur.GetPalet();

	XMFLOAT2 paletPos = { palet->GetEntity()->transform.pos.x, palet->GetEntity()->transform.pos.z };

	XMFLOAT2 paletNextPos = { paletPos.x + palet->GetVelocity().x * (1.0f - palet->GetVelocity().x / 50.0f), paletPos.y + palet->GetVelocity().y * (1.0f - palet->GetVelocity().y / 30.0f) };

	if (paletNextPos.x < cur.GetXBounds().x + 1.0f)
		paletNextPos.x = cur.GetXBounds().x;
	else if (paletNextPos.x > cur.GetXBounds().y - 1.0f)
		paletNextPos.x = cur.GetXBounds().y;

	if (paletNextPos.y < cur.GetZBounds().x + 1.0f)
		paletNextPos.y = cur.GetZBounds().x;
	else if (paletNextPos.y > cur.GetZBounds().y - 1.0f)
		paletNextPos.y = cur.GetZBounds().y;

	cur.GoToPosition({ paletNextPos.x, 21.0f });

	cur.SetSpeed(15.0f);
}

void StateMechanicalBotDefend::OnExecute(MechanicalBot & cur)
{
	Palet* palet = cur.GetPalet();

	XMFLOAT2 paletPos = { palet->GetEntity()->transform.pos.x, palet->GetEntity()->transform.pos.z };
	XMFLOAT2 botPos = { cur.GetEntity()->transform.pos.x, cur.GetEntity()->transform.pos.z };

	float distanceBetweenBotAndPalet = sqrt(pow((paletPos.x - botPos.x), 2) + pow((paletPos.y - botPos.y), 2));

	if (distanceBetweenBotAndPalet >= 1.5f)
	{
		XMFLOAT2 paletPos = { palet->GetEntity()->transform.pos.x, palet->GetEntity()->transform.pos.z };
		cur.GoToPosition({ paletPos.x, 21.0f });
	}
	else
	{
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotAttack));
	}

	if (cur.IsDestinationSet() == false && palet->GetEntity()->transform.pos.z <= 16.0f && palet->GetVelocity().y < 0)
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotIdle));

	if (palet->GetVelocity().y < 20.0f)
	{
		if (palet->GetEntity()->transform.pos.z >= 15.0f)
		{
			cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotAttack));
			return;
		}
	}
}

void StateMechanicalBotDefend::OnExit(MechanicalBot & cur, int to)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotAttack::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
	Palet* palet = cur.GetPalet();

	XMFLOAT2 paletPos = { palet->GetEntity()->transform.pos.x, palet->GetEntity()->transform.pos.z };

	XMFLOAT2 paletNextPos = { paletPos.x + palet->GetVelocity().x * 0.33f, paletPos.y + palet->GetVelocity().y * 0.33f };

	if (paletNextPos.x < cur.GetXBounds().x + 1.0f)
		paletNextPos.x = cur.GetXBounds().x;
	else if (paletNextPos.x > cur.GetXBounds().y - 1.0f)
		paletNextPos.x = cur.GetXBounds().y;

	if (paletNextPos.y < cur.GetZBounds().x + 1.0f)
		paletNextPos.y = cur.GetZBounds().x;
	else if (paletNextPos.y > cur.GetZBounds().y - 1.0f)
		paletNextPos.y = cur.GetZBounds().y;

	cur.GoToPosition({ paletNextPos.x, paletNextPos.y });
}

void StateMechanicalBotAttack::OnExecute(MechanicalBot & cur)
{
	cur.SetSpeed(cur.GetSpeed() + 0.5f);

	Palet* palet = cur.GetPalet();

	XMFLOAT2 paletPos = { palet->GetEntity()->transform.pos.x, palet->GetEntity()->transform.pos.z };
	XMFLOAT2 botPos = { cur.GetEntity()->transform.pos.x, cur.GetEntity()->transform.pos.z };

	float distanceBetweenBotAndPalet = sqrt(pow((paletPos.x - botPos.x), 2) + pow((paletPos.y - botPos.y), 2));

	if (distanceBetweenBotAndPalet <= 1.5f)
	{
		cur.SetSpeed(15.0f);

		int side;
		if (botPos.x < 7.0f * 0.5f)
			side = 1;
		else
			side = -1;

		cur.GoToPosition({ paletPos.x - (side * 0.25f), paletPos.y + 1.30f});
		cur.SetWaitDuration(0.5f);
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotWait));
		return;
	}

	if (cur.IsDestinationSet() == false && palet->GetEntity()->transform.pos.z <= 16.0f && palet->GetVelocity().y < 0.0f)
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotIdle));
	else if (cur.IsDestinationSet() == false)
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotDefend));
}

void StateMechanicalBotAttack::OnExit(MechanicalBot & cur, int to)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotWallBounceAttack::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
	cur.SetEngaged(false);

	int side = rand() % 100;
	if (side % 2)
		side = -1;
	else
		side = 1;

	cur.SetSpeed(5.0f);

	cur.GoToPosition({ (side * 5.0f) / 2 - (side * 0.75f), 15.0f });
}

void StateMechanicalBotWallBounceAttack::OnExecute(MechanicalBot& cur)
{
	if (cur.IsDestinationSet() == false && cur.HasEngaged() == false)
	{
		cur.SetEngaged(true);
		cur.SetSpeed(15.0f);

		int side;

		if (cur.GetEntity()->transform.pos.x < 0.0f)
			side = -1;
		else
			side = 1;

		cur.GoToPosition({ (-side * 4.0f) * 0.5f - (-side * 1.25f), cur.GetEntity()->transform.pos.z});
	}
	else if (cur.IsDestinationSet() == false)
	{
		cur.SetSpeed(15.0f);
		cur.GoToPosition({ cur.GetEntity()->transform.pos.x, cur.GetEntity()->transform.pos.z + 0.25f });
		cur.SetWaitDuration(0.20f);
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotWait));
	}
		
}

void StateMechanicalBotWallBounceAttack::OnExit(MechanicalBot & cur, int to)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateMechanicalBotWait::OnEnter(MechanicalBot& cur, int from, void* pParam)
{
	cur.ResetWaitTimer();
}

void StateMechanicalBotWait::OnExecute(MechanicalBot& cur)
{
	cur.UpdateWaitTimer();

	if (cur.GetWaitTimer() > cur.GetWaitDuration())
		cur.GetFsm()->ToState(CPU_ID(StateMechanicalBotDefend));
}

void StateMechanicalBotWait::OnExit(MechanicalBot& cur, int to)
{

}

