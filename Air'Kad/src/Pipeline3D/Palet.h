#pragma once

class Pusher;

class Bot;

class Palet
{
private:
	cpu_mesh m_mesh;
	cpu_entity* m_pEntity;
	XMFLOAT2 m_vel = { 0.0f, 0.0f };

public:
	Palet(Pusher* pusher);

	void UpdatePhysicsPlayer(float dt, float minX, float maxX, float maxZ, Pusher* pusher, XMFLOAT2 pusherVelocity);
	void UpdatePhysicsBot(float dt, float minX, float maxX, float maxZ, Bot* bot, XMFLOAT2 pusherVelocity);
	void SetPosition(XMFLOAT3 position);
	void ResetDepth(float z);
	float GetDepth() const;
	void Activate(bool active);
	cpu_entity* GetEntity() { return m_pEntity; };

	XMFLOAT2 GetVelocity() { return m_vel; }
};

