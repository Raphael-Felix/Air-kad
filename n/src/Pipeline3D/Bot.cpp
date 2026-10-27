#include "pch.h"
#include "Bot.h"
#include "Palet.h"
Bot::Bot()
{

}

void Bot::Create(cpu_mesh* mesh, XMFLOAT3 pos, XMFLOAT3 col)
{
	m_material.color = col;

	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = mesh;
	m_pEntity->pMaterial = &m_material;
	m_pEntity->transform.sca = { 1.0f, 0.5f, 1.0f };
	SetPosition(pos);

	//m_pFsm = cpuEngine.CreateFSM<Bot>(this);
}

void Bot::Update()
{
	Move();
}


void Bot::GoToPosition(XMFLOAT2 posXZ)
{
	m_destXZ = posXZ;

	XMFLOAT2 curPosXZ = { GetEntity()->transform.pos.x, GetEntity()->transform.pos.z };

	XMVECTOR curVectorXZ = XMLoadFloat2(&curPosXZ);
	XMVECTOR destVectorXZ = XMLoadFloat2(&posXZ);

	XMVECTOR curDestVector = destVectorXZ - curVectorXZ;

	float norm = sqrt(pow(XMVectorGetX(curDestVector), 2.0f) + pow(XMVectorGetY(curDestVector), 2.0f));

	if (norm < 0.1f)
	{
		GetEntity()->transform.pos.x = posXZ.x;
		GetEntity()->transform.pos.z = posXZ.y;

		m_destinationSet = false;

		return;
	}

	if (norm != 0.0f)
		curDestVector /= norm;

	GetEntity()->transform.dir.x = XMVectorGetX(curDestVector);
	GetEntity()->transform.dir.z = XMVectorGetY(curDestVector);

	m_destinationSet = true;
}

void Bot::IsCollidingBounds(float x, float z)
{
	XMFLOAT3 pos = m_pEntity->transform.pos;

	co.XNegatif = (x < m_xBounds.x);

	co.XPositive = (x > m_xBounds.y);

	co.ZNegatif = (z < m_zBounds.x);

	co.ZPositive = (z > m_zBounds.y);

}

void Bot::CalculateTrajectory()
{
	XMFLOAT2 trajectory;
	trajectory.x = m_pPalet->GetEntity()->transform.pos.x;
	trajectory.y = m_pPalet->GetEntity()->transform.pos.z;
	XMFLOAT2 speed = m_pPalet->GetVelocity();
	 
	while (trajectory.y < m_pEntity->transform.pos.z)
	{
		float dt = cpuTime.delta;
		if (speed.x <= 1.0f && speed.y <= 0)
			break;
		IsCollidingBounds(trajectory.x, trajectory.y);
		trajectory.x += speed.x * dt ;
		trajectory.y += speed.y * dt ;
		if (co.XNegatif || co.XPositive)
			speed.x = -speed.x;
		if (trajectory.y > m_pEntity->transform.pos.z - 1.5f)
			break;
	}
	
	m_finalPosition.x = trajectory.x;
	m_finalPosition.y = trajectory.y;
}

void Bot::SetPosition(XMFLOAT3 pos)
{
	m_pEntity->transform.SetPosition(pos.x, pos.y, pos.z);
}

float Bot::GetSpeed()
{
	if (m_pEntity->transform.dir.x != 0 || m_pEntity->transform.dir.y != 0 || m_pEntity->transform.dir.z != 0)
		return m_speed;

	return 1.0f;
}


