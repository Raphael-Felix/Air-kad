#pragma once
class Pusher;
class Palet;

struct Coordinate
{
	bool XPositive = false;
	bool XNegatif = false;
	bool ZPositive = false;
	bool ZNegatif = false;
};

class Bot
{
protected:
	//cpu_fsm<Bot>* m_pFsm;

	cpu_entity* m_pEntity;
	Palet* m_pPalet;
	Pusher* m_pPlayer;

	float dt = 0.0f;
	float m_speed = 3.0f;
	float m_maxSpeed = 5.0f;
	int m_direction = 1;
	float m_force;
	float m_moveTimer;
	XMFLOAT2 m_directionVector;

	cpu_material m_material;

	XMFLOAT2 m_destXZ = { 0.0f, 0.0f };
	bool m_destinationSet = false;

	XMFLOAT2 m_xBounds;
	XMFLOAT2 m_zBounds;

	XMFLOAT2 m_finalPosition;

public:
	Bot();
	virtual ~Bot() {};

	Coordinate co;

	void Create(cpu_mesh* mesh, XMFLOAT3 pos, XMFLOAT3 col = CPU_BLUE);
	virtual void Destroy() = 0;

	virtual void InitializeBot() = 0;
	virtual void StartGame() = 0;

	void Update();

	virtual void Engage() = 0;

	virtual void Move() = 0;

	void GoToPosition(XMFLOAT2 posXZ);

	void IsCollidingBounds(float x, float z);

	bool IsDestinationSet() { return m_destinationSet; }

	void CalculateTrajectory();


	void ResetDt() { dt = 0; };
	void IncrementDt(float value) { dt += value; };
	void SetSpeed(float speed) { m_speed = std::min(speed, m_maxSpeed); };
	void SetMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
	void SetDirection(float direction) { m_direction = direction; };
	void SetBounds(XMFLOAT2 xBounds, XMFLOAT2 zBounds) { m_xBounds = xBounds, m_zBounds = zBounds; };
	void SetPosition(XMFLOAT3 pos);
	void SetPallet(Palet* pallet) { m_pPalet = pallet; };
	void SetPlayer(Pusher* player) { m_pPlayer = player; };
	void SetMaterialColor(XMFLOAT3 col) { m_material.color = col; }

	float GetDt() { return dt; };
	float GetSpeed();
	float GetMaxSpeed() { return m_maxSpeed; };
	float GetDirection() { return m_direction; };
	float GetForce() { return m_force; };
	XMFLOAT2 GetXBounds() { return m_xBounds; };
	XMFLOAT2 GetZBounds() { return m_zBounds; };
	XMFLOAT2 GetFinalPosition() { return m_finalPosition; };
	cpu_entity* GetEntity() { return m_pEntity; };
	Palet* GetPalet() { return m_pPalet; };
	Pusher* GetPlayer() { return m_pPlayer; };
	//cpu_fsm<Bot>* GetFsm() { return m_pFsm; };
};

