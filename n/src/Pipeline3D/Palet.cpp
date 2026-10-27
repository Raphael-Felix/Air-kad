#include "pch.h"
#include "Palet.h"
#include "Pusher.h"
#include "Bot.h"

Palet::Palet(Pusher* pusher)
{
	m_mesh.CreateCylinder(0.05f, 0.5f, 20, true, true, {0.0f, 0.4f, 1.0f});

	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = &m_mesh;
	m_pEntity->transform.pos.y += 0.3f;

	if (pusher && pusher->GetEntity())
	{
		m_pEntity->transform.pos.z = pusher->GetEntity()->transform.pos.z;
		m_pEntity->transform.pos.x = pusher->GetEntity()->transform.pos.x;
	}
}

void Palet::UpdatePhysicsPlayer(float dt, float minX, float maxX, float maxZ, Pusher* pusher, XMFLOAT2 pusherVelocity)
{
	const float paletRadius = 0.5f;
	XMFLOAT3& position = m_pEntity->transform.pos;

	position.x += m_vel.x * dt;
	position.z += m_vel.y * dt;
	if (position.x < minX + paletRadius)
	{
		position.x = minX + paletRadius;
		m_vel.x = -m_vel.x * 0.8f;
	}

	if (position.x > maxX - paletRadius)
	{
		position.x = maxX - paletRadius;
		m_vel.x = -m_vel.x * 0.8f;
	}

	if (position.z > maxZ - paletRadius)
	{
		position.z = maxZ - paletRadius;
		m_vel.y = -m_vel.y * 0.8f;
	}
	const float pusherHalfSize = 0.5f;
	XMFLOAT3& pusherPosition = pusher->GetEntity()->transform.pos;

	float closestX = fmaxf(pusherPosition.x - pusherHalfSize, fminf(position.x, pusherPosition.x + pusherHalfSize));
	float closestZ = fmaxf(pusherPosition.z - pusherHalfSize, fminf(position.z, pusherPosition.z + pusherHalfSize));

	float dx = position.x - closestX;
	float dz = position.z - closestZ;
	float distance = sqrtf(dx * dx + dz * dz);

	if (distance < paletRadius)
	{
		float normalX = 0.0f, normalZ = 1.0f;
		if (distance > 0.0001f) { normalX = dx / distance; normalZ = dz / distance; }

		position.x += normalX * (paletRadius - distance);
		position.z += normalZ * (paletRadius - distance);

		float relativeVelocityX = m_vel.x - pusherVelocity.x;
		float relativeVelocityZ = m_vel.y - pusherVelocity.y;
		float normalVelocity = relativeVelocityX * normalX + relativeVelocityZ * normalZ;

		if (normalVelocity < 0.0f)
		{
			m_vel.x = pusherVelocity.x + (relativeVelocityX - 2.0f * normalVelocity * normalX);
			m_vel.y = pusherVelocity.y + (relativeVelocityZ - 2.0f * normalVelocity * normalZ);
		}
		else
		{
			m_vel.x = pusherVelocity.x + (relativeVelocityX + 2.0f * normalVelocity * normalX);
			m_vel.y = pusherVelocity.y + (relativeVelocityZ + 2.0f * normalVelocity * normalZ);
		}

		float speed = sqrtf(m_vel.x * m_vel.x + m_vel.y * m_vel.y);
		const float maxSpeed = 35.0f;
		if (speed > maxSpeed) { m_vel.x *= maxSpeed / speed; m_vel.y *= maxSpeed / speed; }
	}
}

void Palet::UpdatePhysicsBot(float dt, float minX, float maxX, float maxZ, Bot* bot, XMFLOAT2 pusherVelocity)
{
	const float paletRadius = 0.5f;
	XMFLOAT3& position = m_pEntity->transform.pos;

	
	const float pusherHalfSize = 0.5f;
	XMFLOAT3& pusherPosition = bot->GetEntity()->transform.pos;

	float closestX = fmaxf(pusherPosition.x - pusherHalfSize, fminf(position.x, pusherPosition.x + pusherHalfSize));
	float closestZ = fmaxf(pusherPosition.z - pusherHalfSize, fminf(position.z, pusherPosition.z + pusherHalfSize));

	float dx = position.x - closestX;
	float dz = position.z - closestZ;
	float distance = sqrtf(dx * dx + dz * dz);

	if (distance < paletRadius)
	{
		float normalX = 0.0f, normalZ = 1.0f;
		if (distance > 0.0001f) { normalX = dx / distance; normalZ = dz / distance; }

		position.x += normalX * (paletRadius - distance);
		position.z -= normalZ * (paletRadius - distance);

		float relativeVelocityX = m_vel.x - pusherVelocity.x;



		float relativeVelocityZ = m_vel.y - pusherVelocity.y;
		float normalVelocity = relativeVelocityX * normalX + relativeVelocityZ * normalZ;

		if (normalVelocity < 0.0f)
		{
			m_vel.x = pusherVelocity.x + (relativeVelocityX - 2.0f * normalVelocity * normalX);
			m_vel.y = pusherVelocity.y + (relativeVelocityZ - 2.0f * normalVelocity * normalZ);
		}
		else
		{
			m_vel.x = pusherVelocity.x + (relativeVelocityX + 2.0f * normalVelocity * normalX);
			m_vel.y = pusherVelocity.y + (relativeVelocityZ + 2.0f * normalVelocity * normalZ);
		}

		float speed = sqrtf(m_vel.x * m_vel.x + m_vel.y * m_vel.y);
		const float maxSpeed = 25.0f;
		if (speed > maxSpeed) { m_vel.x *= maxSpeed / speed; m_vel.y *= maxSpeed / speed; }
	}
}

void Palet::SetPosition(XMFLOAT3 position)
{
	m_pEntity->transform.SetPosition(position.x, position.y, position.z);
}

void Palet::ResetDepth(float z)
{
	m_pEntity->transform.pos.z = z;
	m_vel = { 0.0f, 0.0f };
}

float Palet::GetDepth() const
{
	return m_pEntity->transform.pos.z;
}

void Palet::Activate(bool active)
{
	m_pEntity->active = active;
}
