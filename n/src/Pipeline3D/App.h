#pragma once
#include "pch.h"

class Pusher;

class StartMenu;
class Game;
class Button;

class App
{
public:
    App();
    static App& GetInstance() { return *s_pApp; }

    cpu_fsm<App>* m_pFsm;

    void OnStart();
    void OnUpdate();
    void FixedUpdate();
    void OnExit();
    void OnRender(int pass);

    cpu_fsm<App>* GetFsm() { return m_pFsm; }

    StartMenu* GetStartMenu() { return m_pStartMenu; }
    Game* GetGame() { return m_pGame; }

    cpu_font GetFont() { return m_font; }

    XMFLOAT2 GetCursorDirectionVector();
    XMFLOAT2 GetScreenMiddlePos() { return m_screenMiddlePos; }

    void UpdateButtons(std::list<Button*>& buttons);
    void RenderButtons(int pass, std::list<Button*>& buttons);

private:
    inline static App* s_pApp = nullptr;

    StartMenu* m_pStartMenu;
    Game* m_pGame;

    cpu_font m_font;

    XMFLOAT2 m_screenMiddlePos;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct StateAppStartMenu
{
    void OnEnter(App& cur, int from, void* pParam);
    void OnExecute(App& cur);
    void OnExit(App& cur, int to);
};

struct StateAppGame
{
    void OnEnter(App& cur, int from, void* pParam);
    void OnExecute(App& cur);
    void OnExit(App& cur, int to);
};