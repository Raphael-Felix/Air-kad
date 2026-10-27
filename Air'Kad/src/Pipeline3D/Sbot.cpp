
#include "pch.h"
#include "Sbot.h"
#include "Pusher.h"
#include "Palet.h"
#include <cstdlib>

void Sbot::Destroy()
{
    cpuEngine.Release(m_pEntity);
    cpuEngine.Release(m_pFsm);
}

void Sbot::InitializeBot()
{
    m_pFsm = cpuEngine.CreateFSM(this);

    m_pFsm->Add<StateSBotDefend>();
    m_pFsm->Add<StateSBotAttack>();

    m_pFsm->ToState(CPU_ID(StateSBotDefend));

    SetSpeed(2.0f);
}

void Sbot::Jump()
{
    float dt = cpuTime.delta;

    if (m_JumpCooldown > 0.0f)
        m_JumpCooldown -= dt;

    // Le bot saute uniquement quand il ne saute pas déjà.
    if (!m_IsJumping && m_JumpCooldown <= 0.0f)
    {
        m_IsJumping = true;
        m_JumpTimer = 0.0f;
        m_JumpCooldown = 0.5f;
    }

    if (m_IsJumping)
    {
        m_JumpTimer += dt;

        // Petit saut simple.
        GetEntity()->transform.pos.y =
            1.0f * sinf(m_JumpTimer / m_JumpDuration * 3.14159f);

        if (m_JumpTimer >= m_JumpDuration)
        {
            m_IsJumping = false;
            GetEntity()->transform.pos.y = 0.0f;
        }
    }
}

void Sbot::StartGame()
{
}

void Sbot::Engage()
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
        m_pFsm->ToState(CPU_ID(StateSBotDefend));
}

void Sbot::Move()
{
    float dt = cpuTime.delta;

    float targetZ = 20;
    float& botZ = GetEntity()->transform.pos.z;
    if (botZ < targetZ)
        botZ += GetSpeed() * dt;
    else if (botZ > targetZ)
        botZ -= GetSpeed() * dt;

    // Le bot reste aligné avec le palet en défense.
    GetEntity()->transform.pos.x =
        GetPalet()->GetEntity()->transform.pos.x;

    // Saute pendant la défense.
    Jump();
}

void Sbot::Attack()
{
    float dt = cpuTime.delta;


    // Se déplace vers le palet.
    float targetX = GetPalet()->GetEntity()->transform.pos.x;
    float targetZ = GetPalet()->GetEntity()->transform.pos.z;

    float& botX = GetEntity()->transform.pos.x;
    float& botZ = GetEntity()->transform.pos.z;

    if (botX < targetX)
        botX += GetSpeed() * dt;
    else if (botX > targetX)
        botX -= GetSpeed() * dt;

    if (botZ < targetZ)
        botZ += GetSpeed() * dt;
    else if (botZ > targetZ)
        botZ -= GetSpeed() * dt;

    // Saute également lorsqu'il va chercher le palet.
    Jump();

    // Retour en défense quand le palet est revenu de notre côté.
    if (targetZ < 10.0f || cpu::ObbObb(GetEntity()->obb, GetPalet()->GetEntity()->obb))
    {
        m_pFsm->ToState(CPU_ID(StateSBotDefend));
    }
}

void StateSBotDefend::OnEnter(
    Sbot& cur, int from, void* pParam)
{
}

void StateSBotDefend::OnExecute(Sbot& cur)
{
    cur.Move();

    float ballZ = cur.GetPalet()->GetEntity()->transform.pos.z;



    // Détecte l'arrivée du palet de notre côté.
    if (ballZ > 10.0f && !cur.m_WasBallOnOurSide)
    {
        cur.m_WasBallOnOurSide = true;

        // Une attaque toutes les trois arrivées environ.
        cur.m_AttackCounter++;

        if (cur.m_AttackCounter >= 3)
        {
            cur.m_AttackCounter = 0;
            cur.m_pFsm->ToState(CPU_ID(StateSBotAttack));
        }
    }

    // Réarme la détection quand le palet repart.
    if (ballZ <= 10.0f)
        cur.m_WasBallOnOurSide = false;
}

void StateSBotDefend::OnExit(
    Sbot& cur, int to)
{
}

void StateSBotAttack::OnEnter(
    Sbot& cur, int from, void* pParam)
{
}

void StateSBotAttack::OnExecute(Sbot& cur)
{
    cur.Attack();
}

void StateSBotAttack::OnExit(
    Sbot& cur, int to)
{
}
