#include "pch.h"
#include "Pusher.h"

Pusher::Pusher()
{
	m_pEntity = nullptr;
	m_pFsm = nullptr;

	m_speed = 1.5f;
	m_force = 0.0f;
	m_moveTimer = 0.0f;
	m_directionVector = XMFLOAT2{ 0.0f, 0.0f };

	m_xBounds = { -5.0f, 5.0f }; // Get the app table limits
	m_zBounds = { 0.0f, 5.0f }; // Get the app table limits
}

Pusher::~Pusher()
{

}

void Pusher::Create(cpu_mesh* mesh, XMFLOAT3 pos)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = mesh;
	SetPosition(pos);

	m_pFsm = cpuEngine.CreateFSM<Pusher>(this);
}

void Pusher::Update()
{
	float dt = cpuTime.delta;

	m_moveTimer += dt;

	App app = App::GetInstance();

	XMFLOAT2 cursorDirectionFloat = app.GetCursorDirectionVector();
	XMVECTOR cursorDirectionVector = XMLoadFloat2(&cursorDirectionFloat);
	XMVECTOR middleCursorVector = XMLoadFloat2(&m_directionVector);

	XMStoreFloat2(&m_directionVector, (middleCursorVector + cursorDirectionVector));

	//Reset cursor
	SetCursorPos(app.GetScreenMiddlePos().x, app.GetScreenMiddlePos().y);

	if (m_moveTimer >= 0.005f)
	{
		Move();

		//Limits the object in the bounds given to it
		LimitPositionInBounds();

		//Reset variables
		m_directionVector = { 0.0f, 0.0f };
		m_moveTimer = 0.0f;
	}
}

void Pusher::Move()
{
	XMVECTOR cursorDirVector = XMLoadFloat2(&m_directionVector);
	float vectorLength = XMVectorGetX(XMVector2Length(cursorDirVector));
	if (vectorLength != 0)
	{
		cursorDirVector /= vectorLength;
	}

	m_force = cpu::Clamp(vectorLength / 150.0f);

	//Move Pusher
	m_pEntity->transform.pos.x += XMVectorGetX(cursorDirVector) * m_force * m_speed;
	m_pEntity->transform.pos.z += -XMVectorGetY(cursorDirVector) * m_force * m_speed;
}

void Pusher::LimitPositionInBounds()
{
	XMFLOAT3 pos = m_pEntity->transform.pos;

	if (pos.x < m_xBounds.x)
		pos.x = m_xBounds.x;
	else if (pos.x > m_xBounds.y)
		pos.x = m_xBounds.y;

	if (pos.z < m_zBounds.x)
		pos.z = m_zBounds.x;
	else if (pos.z > m_zBounds.y)
		pos.z = m_zBounds.y;

	m_pEntity->transform.pos = pos;
}

void Pusher::SetPosition(XMFLOAT3 pos)
{
	m_pEntity->transform.SetPosition(pos.x, pos.y, pos.z);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////