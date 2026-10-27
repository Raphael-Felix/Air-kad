
#pragma once
#include "Bot.h"

class Sbot : public Bot
{
public:
    cpu_fsm<Sbot>* m_pFsm;

    float m_JumpTimer = 0.0f;
    float m_JumpDuration = 0.25f;
    float m_JumpCooldown = 0.0f;

    bool m_IsJumping = false;
    bool m_WasBallOnOurSide = false;

    int m_AttackCounter = 0;

    void Destroy() override;
    void InitializeBot() override;
    void Move() override;
    void Attack();
    void Jump();
    void StartGame() override;

    void Update();

    void Engage() override;

};

struct StateSBotDefend
{
    void OnEnter(Sbot& cur, int from, void* pParam);
    void OnExecute(Sbot& cur);
    void OnExit(Sbot& cur, int to);
};

struct StateSBotAttack
{
    void OnEnter(Sbot& cur, int from, void* pParam);
    void OnExecute(Sbot& cur);
    void OnExit(Sbot& cur, int to);
};
