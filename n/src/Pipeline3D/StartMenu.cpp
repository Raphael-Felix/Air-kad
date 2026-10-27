#include "pch.h"
#include "StartMenu.h"
#include "Game.h"
#include "Button.h"
#include "MechanicalBot.h"
#include "StrategicalBot.h"

StartMenu::StartMenu()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->SetPostGlobal<StateStartMenuGlobal>();
	m_pFsm->Add<StateStartMenuInactive>();
	m_pFsm->Add<StateStartMenuIdle>();
	m_pFsm->Add<StateStartMenuBotSelection>();
	m_pFsm->ToState(CPU_ID(StateStartMenuInactive));

	m_titleFont.Create(75);
	m_textFont.Create(50);

	Button* button = new Button({ 400.0f, 120.0f });
	button->SetPosition({ 1920 * 0.5f, 1080 * 0.66f }, { 0.5f, 0.5f });
	button->SetButtonColor({ 0.7f, 0.6f, 0.5f });
	button->SetTextAlign(CPU_TEXT_CENTER);
	button->SetText("Start Game");
	*button->GetButtonFunction() = []() { App::GetInstance().GetStartMenu()->GetFsm()->ToState(CPU_ID(StateStartMenuBotSelection)); };

	Button* quitButton = new Button({ 400.0f, 120.0f });
	quitButton->SetPosition({ 1920 * 0.5f, 1080 * 0.85f }, { 0.5f, 0.5f });
	quitButton->SetButtonColor({ 0.5f, 0.5f, 0.4f });
	quitButton->SetTextAlign(CPU_TEXT_CENTER);
	quitButton->SetText("Quit");
	*quitButton->GetButtonFunction() = []() { cpuEngine.Quit(); };

	m_pStartMenuButtons.push_back(button);
	m_pStartMenuButtons.push_back(quitButton);

	Button* larryButton = new Button({ 300.0f, 120.0f });
	larryButton->SetPosition({ 1920 * 0.25f, 1080 * 0.75f }, { 0.5f, 0.5f });
	larryButton->SetButtonColor({ 0.33f, 0.0f, 0.25f });
	larryButton->SetTextAlign(CPU_TEXT_CENTER);
	larryButton->SetText("Play against Larry");
	larryButton->SetTextColor({ 1.0f, 0.75f, 1.0f });
	*larryButton->GetButtonFunction() = []() { App::GetInstance().GetGame()->SetBot(BotNames::Larry); App::GetInstance().GetFsm()->ToState(CPU_ID(StateAppGame)); };

	Button* TalosButton = new Button({ 300.0f, 120.0f });
	TalosButton->SetPosition({ 1920 * 0.5f, 1080 * 0.75f }, { 0.5f, 0.5f });
	TalosButton->SetButtonColor({ 0.85f, 0.75f, 0.25f });
	TalosButton->SetTextAlign(CPU_TEXT_CENTER);
	TalosButton->SetText("Play against Talos");
	TalosButton->SetTextColor({ 0.25f, 0.25f, 0.0f });
	*TalosButton->GetButtonFunction() = []() { App::GetInstance().GetGame()->SetBot(BotNames::Talos); App::GetInstance().GetFsm()->ToState(CPU_ID(StateAppGame)); };

	Button* KarlButton = new Button({ 300.0f, 120.0f });
	KarlButton->SetPosition({ 1920 * 0.75f, 1080 * 0.75f }, { 0.5f, 0.5f });
	KarlButton->SetButtonColor({ 0.05f, 0.25f, 0.050f });
	KarlButton->SetTextAlign(CPU_TEXT_CENTER);
	KarlButton->SetText("Play against Karl");
	KarlButton->SetTextColor({ 0.75f, 1.0f, 0.75f });
	*KarlButton->GetButtonFunction() = []() { App::GetInstance().GetGame()->SetBot(BotNames::Karl); App::GetInstance().GetFsm()->ToState(CPU_ID(StateAppGame)); };

	m_pBotSelectionButtons.push_back(larryButton);
	m_pBotSelectionButtons.push_back(TalosButton);
	m_pBotSelectionButtons.push_back(KarlButton);
}

void StartMenu::StartMenuUpdate()
{
	App app = App::GetInstance();
	app.UpdateButtons(m_pStartMenuButtons);
}

void StartMenu::StartMenuRender(int pass)
{
	App app = App::GetInstance();

	XMFLOAT3 color = { 0.1f, 0.0f, 0.2f };

	app.RenderButtons(pass, m_pStartMenuButtons);

	std::string title = "Air'kad";

	cpuDevice.DrawText(&m_titleFont, title.c_str(), 1920 * 0.5f, 1080 * 0.25f, CPU_TEXT_CENTER);
}

void StartMenu::BotSelectionMenuUpdate()
{
	App app = App::GetInstance();
	app.UpdateButtons(m_pBotSelectionButtons);
}

void StartMenu::BotSelectionMenuRender(int pass)
{
	App app = App::GetInstance();

	XMFLOAT3 color = { 0.1f, 0.0f, 0.2f };

	app.RenderButtons(pass, m_pBotSelectionButtons);

	std::string title = "Who do you want to play with ?";

	cpuDevice.DrawText(&m_textFont, title.c_str(), 1920 * 0.5f, 1080 * 0.25f, CPU_TEXT_CENTER);
}

void StartMenu::ChangeState(int to, void* pParam)
{
	m_pFsm->ToState(to, pParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//States

void StateStartMenuGlobal::OnEnter(StartMenu& cur, int from, void* pParam)
{
}

void StateStartMenuGlobal::OnExecute(StartMenu& cur)
{
	//if (cpuInput.IsBackPressed())
		//cpuEngine.Quit();
}

void StateStartMenuGlobal::OnExit(StartMenu& cur, int to)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateStartMenuIdle::OnEnter(StartMenu& cur, int from, void* pParam)
{

}

void StateStartMenuIdle::OnExecute(StartMenu& cur)
{
	cur.StartMenuUpdate();
}

void StateStartMenuIdle::OnExit(StartMenu& cur, int to)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateStartMenuBotSelection::OnEnter(StartMenu& cur, int from, void* pParam)
{

}

void StateStartMenuBotSelection::OnExecute(StartMenu& cur)
{
	cur.BotSelectionMenuUpdate();
}

void StateStartMenuBotSelection::OnExit(StartMenu& cur, int to)
{
}