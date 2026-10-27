#pragma once

class Pusher
{
private:
	cpu_entity* m_pEntity;
	cpu_fsm<Pusher>* m_pFsm;

	float m_speed;
	float m_force;
	float m_moveTimer;
	XMFLOAT2 m_directionVector;

	XMFLOAT2 m_xBounds;
	XMFLOAT2 m_zBounds;

public:
	Pusher();
	~Pusher();

	void Create(cpu_mesh* mesh, XMFLOAT3 pos);

	void Update();

	void Move();

	void LimitPositionInBounds();

	void SetBounds(XMFLOAT2 xBounds, XMFLOAT2 zBounds) { m_xBounds = xBounds, m_zBounds = zBounds; }
	void SetPosition(XMFLOAT3 pos);

	float GetForce() { return m_force; }
	XMFLOAT2 GetDirectionVector() { return m_directionVector; }
	cpu_entity* GetEntity() { return m_pEntity; }
	cpu_fsm<Pusher>* GetFsm() { return m_pFsm; }
};