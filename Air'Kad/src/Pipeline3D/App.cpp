#include "pch.h"
#include "App.h"
#include "StartMenu.h"
#include "Game.h"
#include "Button.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);
}

void App::OnStart()
{
	//System
	srand(time(NULL));
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	m_screenMiddlePos = { GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f };

	m_pStartMenu = new StartMenu();
	m_pGame = new Game();

	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->Add<StateAppStartMenu>();
	m_pFsm->Add<StateAppGame>();
	m_pFsm->ToState(CPU_ID(StateAppStartMenu));
}

void App::OnUpdate()
{



}

void App::FixedUpdate()
{




}

void App::OnExit()
{
}

void App::OnRender(int pass)
{	
	if (m_pStartMenu->GetFsm()->state != CPU_ID(StateStartMenuInactive))
	{
		if (m_pStartMenu->GetFsm()->state == CPU_ID(StateStartMenuIdle))
			m_pStartMenu->StartMenuRender(pass);
		else if (m_pStartMenu->GetFsm()->state == CPU_ID(StateStartMenuBotSelection))
			m_pStartMenu->BotSelectionMenuRender(pass);
	}
	else if (m_pGame->GetFsm()->state != CPU_ID(StateGameInactive))
	{
		m_pGame->GameRender(pass);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

XMFLOAT2 App::GetCursorDirectionVector()
{
	XMFLOAT2 cursorPos;
	cpuEngine.GetCursor(cursorPos);

	XMFLOAT2 screenMiddlePos = { 1920 / 2, 1080 / 2 };

	XMVECTOR cursorPoint = XMLoadFloat2(&cursorPos);
	XMVECTOR middleScreenPoint = XMLoadFloat2(&screenMiddlePos);
	XMVECTOR middleCursorDirVector = cursorPoint - middleScreenPoint;

	XMFLOAT2 middleCursorFloat;
	XMStoreFloat2(&middleCursorFloat, middleCursorDirVector);

	return middleCursorFloat;
}

void App::UpdateButtons(std::list<Button*>& buttons)
{
	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		Button* button = *it;

		button->Update();
	}
}

void App::RenderButtons(int pass, std::list<Button*>& buttons)
{
	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		Button* button = *it;

		button->Render(pass);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//States

void StateAppStartMenu::OnEnter(App& cur, int from, void* pParam)
{
	cpu_fsm<StartMenu>* startMenuFsm = cur.GetStartMenu()->GetFsm();

	startMenuFsm->active = true;
	startMenuFsm->ToState(CPU_ID(StateStartMenuIdle));
}

void StateAppStartMenu::OnExecute(App& cur)
{

}

void StateAppStartMenu::OnExit(App& cur, int to)
{
	cpu_fsm<StartMenu>* startMenuFsm = cur.GetStartMenu()->GetFsm();

	startMenuFsm->ToState(CPU_ID(StateStartMenuInactive));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateAppGame::OnEnter(App& cur, int from, void* pParam)
{
	cpu_fsm<Game>* gameFsm = cur.GetGame()->GetFsm();

	gameFsm->ToState(CPU_ID(StateGamePlay));
}

void StateAppGame::OnExecute(App& cur)
{

}

void StateAppGame::OnExit(App& cur, int to)
{
	cpu_fsm<Game>* gameFsm = cur.GetGame()->GetFsm();

	gameFsm->ToState(CPU_ID(StateGameInactive));
}