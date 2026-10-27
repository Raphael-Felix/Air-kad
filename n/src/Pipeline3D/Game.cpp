#include "pch.h"
#include "Game.h"
#include "Pusher.h"
#include "Sbot.h"
#include "StrategicalBot.h"
#include "MechanicalBot.h"
#include "Palet.h"
#include "Button.h"
Game::Game()
{
	m_pFsm = cpuEngine.CreateFSM(this);
	m_pFsm->SetPostGlobal<StateGameGlobal>();
	m_pFsm->Add<StateGamePlay>();
	m_pFsm->Add<StateGameOver>();
	m_pFsm->Add<StateGameWin>();

	m_pFsm->ToState(CPU_ID(StateGameInactive));

	m_playerScore = 0;
	m_botScore = 0;

	//Mesh
	m_skyBoxMesh.CreateSkyBox(25.f, { 0.2f, 0.6f, 0.8f });
	m_playerMesh.CreateCube();
	m_botMesh.CreateCube(0.5f, CPU_WHITE);
	m_tableMesh.CreateCube(10.0f, {0.4f, 0.3f, 0.25f});
	m_tableBordMesh.CreateCube(0.5f, {0.1f, 0.05f, 0.0f});


	//Objects
	m_pSkyBox = cpuEngine.CreateEntity();
	m_pSkyBox->pMesh = &m_skyBoxMesh;

	m_pTable = cpuEngine.CreateEntity();
	m_pTable->pMesh = &m_tableMesh;
	m_pTable->transform.pos.z += 10.0f;
	m_pTable->transform.pos.y -= 1.25f;
	//m_pTable->transform.SetYPR(0.0f, XM_PIDIV2);
	m_pTable->transform.sca = { 0.40f, 0.1f, 1.3f };

	m_pTableBord1 = cpuEngine.CreateEntity();
	m_pTableBord1->pMesh = &m_tableBordMesh;
	m_pTableBord1->transform.pos.x += 3.75f;
	m_pTableBord1->transform.pos.z += 9.98f;
	m_pTableBord1->transform.sca = { 0.666f, 0.4f, 26.0f };

	m_pTableBord2 = cpuEngine.CreateEntity();
	m_pTableBord2->pMesh = &m_tableBordMesh;
	m_pTableBord2->transform.pos.x -= 3.75f;
	//m_pTableBord2->transform.pos.y += 0.04f;
	m_pTableBord2->transform.pos.z += 9.98f;
	m_pTableBord2->transform.sca = { 0.666f, 0.4f, 26.0f };

	m_pPlayer = new Pusher();
	m_pPlayer->Create(&m_playerMesh, { 0.0f, 0.0f, 0.0f });
	m_pPlayer->SetBounds(XMFLOAT2{ m_pTable->transform.pos.x - 7.0f / 2 + 0.75f,  m_pTable->transform.pos.x + 7.0f / 2 - 0.75f }, XMFLOAT2{ -2.0f, 2.0f });
	m_pPlayer->GetEntity()->transform.sca = { 1.0f, 0.5f, 1.0f };
	
	m_pPalet = new Palet(m_pPlayer);

	m_pBot = nullptr;

	ActivateObjects(false);

	Button* button = new Button({250, 100});
	button->SetPosition({ 1920 * 0.5f, 1080 * 0.90f }, { 0.5f, 0.5f });
	button->SetButtonColor({ 0.5f, 0.7f, 1.0f });
	button->SetTextAlign(CPU_TEXT_CENTER);
	button->SetText("Back to Menu");
	*button->GetButtonFunction() = []() {App::GetInstance().GetFsm()->ToState(CPU_ID(StateAppStartMenu)); };
	m_pGameOverButtons.push_back(button);
	m_pGameWinButtons.push_back(button);
}

void Game::FixedUpdate(float fixedDt)
{ 

	const float minX = m_pTableBord2->transform.pos.x, maxX = m_pTableBord1->transform.pos.x;
	const float maxZ = 23.0f;
	m_pPalet->UpdatePhysicsPlayer(fixedDt, minX, maxX, maxZ, m_pPlayer, m_playerVel);
	m_pPalet->UpdatePhysicsBot(fixedDt, minX, maxX, maxZ, m_pBot, m_botVel);
}

void Game::StartGame()
{
	App app = App::GetInstance();

	//System
	SetCursorPos(app.GetScreenMiddlePos().x, app.GetScreenMiddlePos().y);
	ShowCursor(false);
	m_gameOver = false;
	m_gameWin = false;
	m_overtime = false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_gameTimerMinutes = 0.0f;
	m_gameTimerSeconds = 30.0f;

	ActivateObjects(true);

	m_pPlayer->SetPosition({ 0.0f, 0.0f, 0.0f });
	m_pBot->SetPosition({ 0.0f, 0.0f, 20.0f });
	m_pPalet->SetPosition({0.0f, 0.05f, 2.0f});
	m_pPalet->ResetDepth(1.0f);

	m_pBot->StartGame();

	//Cursor camera
	cpuEngine.GetCamera()->transform.pos = XMFLOAT3{ 0, 5.0f, -10.0f };
	cpuEngine.GetCamera()->transform.SetYPR(0.0f, 0.3f, 0.0f);
}

void Game::GameUpdate()
{
	float dt = cpuTime.delta; // dt

	UpdateTimer();

	m_Accumulator += dt;

	while (m_Accumulator >= m_FixedDeltaTime)
	{
		FixedUpdate(m_FixedDeltaTime);
		m_Accumulator -= m_FixedDeltaTime;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_gameTimerSeconds <= 0.000f && m_overtime == false)
	{
		if (m_botScore > m_playerScore)
		{
			m_gameOver = true;
		}
		else if (m_botScore < m_playerScore)
		{
			m_gameWin = true;
		}
		else if (m_botScore == m_playerScore)
		{
			m_overtime = true;
		}

		return;
	}

	XMFLOAT3 oldCubePos = m_pPlayer->GetEntity()->transform.pos;
	m_pPlayer->Update();

	m_playerVel.x = m_pPlayer->GetForce() * 3.0f;
	m_playerVel.y = m_pPlayer->GetForce() * 3.0f;

	float botSpeed = m_pBot->GetSpeed();

	m_botVel.x = std::max(0.1f, (botSpeed / m_pBot->GetMaxSpeed())) * 3.0f;
	m_botVel.y = std::max(0.1f, (botSpeed / m_pBot->GetMaxSpeed())) * 3.0f;

	Goal();

}

void Game::GameRenderUpdate(int pass)
{
	cpu_font font = App::GetInstance().GetFont();

	std::string textScore1 = "Player: " + CPU_STR(m_playerScore);
	std::string textScore2 = "Bot: " + CPU_STR(m_botScore);
	std::string textTimer = CPU_STR(m_gameTimerMinutes) + ":" + CPU_STR((int)m_gameTimerSeconds);

	cpuDevice.DrawText(&font, textScore1.c_str(), 10, 10);
	cpuDevice.DrawText(&font, textScore2.c_str(), 10, 40);
	cpuDevice.DrawText(&font, textTimer.c_str(), 1920 * 0.5f, 50, CPU_TEXT_CENTER);
}

void Game::GameRender(int pass)
{
	if (GetFsm()->state == CPU_ID(StateGamePlay))
	{
		GameRenderUpdate(pass);
	}
	else if (GetFsm()->state == CPU_ID(StateGameOver))
	{
		GameOverRender(pass);
	}
	else if (GetFsm()->state == CPU_ID(StateGameWin))
	{
		GameWinRender(pass);
	}
}

void Game::QuitGame()
{
	ShowCursor(true);

	ActivateObjects(false);
}

void Game::ActivateObjects(bool activate)
{
	if (activate)
	{
		m_playerScore = 0;
		m_botScore = 0;
	}	

	m_pPlayer->GetEntity()->active = activate;
	m_pPlayer->GetFsm()->active = activate;

	m_pTable->active = activate;

	m_pTableBord1->active = activate;

	m_pTableBord2->active = activate;

	m_pPalet->Activate(activate);

	if (m_pBot != nullptr)
		m_pBot->GetEntity()->active = activate;
}

void Game::Goal()
{
	if (m_pPalet->GetDepth() > 22.f)
	{
		m_playerScore++;

		// Bot engages
		m_pPalet->SetPosition({ 0.0f, 0.15f, 14.5f });
		m_pPalet->ResetDepth(14.5f);
		m_pBot->Engage();
	}
		
	else if (m_pPalet->GetDepth() < -3)
	{
		m_botScore++;
		m_pPalet->SetPosition({ 0.0f, 0.15f, 2.0f });
		m_pPalet->ResetDepth(2.0f);
	}
	else
		return;

	

	if (IsOvertime())
		if (m_playerScore > m_botScore)
			m_gameWin = true;
		else
			m_gameOver = true;
}

void Game::GameWinUpdate()
{
	App app = App::GetInstance();

	app.UpdateButtons(m_pGameWinButtons);
}

void Game::GameWinRender(int pass)
{
	App app = App::GetInstance();
	cpu_font font = app.GetFont();

	std::string gameWinText = "You Win !";
	std::string score = "Competitor : " + std::to_string(m_playerScore);
	score += " / " + std::to_string(m_botScore);

	switch (m_botName)
	{
	case BotNames::Larry:
		score += " : Larry";
		break;
	case BotNames::Talos:
		score += " : Talos";
		break;
	case BotNames::Karl:
		score += " : Karl";
		break;
	}

	cpuDevice.DrawText(&font, gameWinText.c_str(), 1920 * 0.5f, 1080 * 0.5f, CPU_TEXT_CENTER);
	cpuDevice.DrawText(&font, score.c_str(), 1920 * 0.5f, 50, CPU_TEXT_CENTER);

	app.RenderButtons(pass, m_pGameWinButtons);
}

void Game::GameOverUpdate()
{
	App app = App::GetInstance();

	app.UpdateButtons(m_pGameOverButtons);
}

void Game::GameOverRender(int pass)
{
	App app = App::GetInstance();

	std::string gameOverText = "You Lose !";
	std::string score = "Competitor : " + std::to_string(m_playerScore);
	score += " / " + std::to_string(m_botScore);
	switch (m_botName)
	{
	case BotNames::Larry:
		score += " : Larry";
		break;
	case BotNames::Talos:
		score += " : Talos";
		break;
	case BotNames::Karl:
		score += " : Karl";
		break;
	}
	cpu_font font = app.GetFont();

	cpuDevice.DrawText(&font, gameOverText.c_str(), 1920 * 0.5f, 1080 * 0.5f, CPU_TEXT_CENTER);
	cpuDevice.DrawText(&font, score.c_str(), 1920 * 0.5f, 50, CPU_TEXT_CENTER);

	app.RenderButtons(pass, m_pGameOverButtons);
}

void Game::UpdateTimer()
{
	float dt = cpuTime.delta;

	if (m_overtime == false)
	{
		if (m_gameTimerSeconds <= 0.000f)
		{
			m_gameTimerSeconds = 60.000f;
			m_gameTimerMinutes = std::max(0, m_gameTimerMinutes - 1);
		}

		m_gameTimerSeconds -= dt;
	}
	else
	{
		if (m_gameTimerSeconds >= 60.000f)
		{
			m_gameTimerSeconds = 0.000f;
			m_gameTimerMinutes += 1;
		}

		m_gameTimerSeconds += dt;
	}
	
	
}

void Game::SetBot(BotNames botname)
{
	if (m_pBot != nullptr)
	{
		m_pBot->Destroy();
		delete m_pBot;
	}

	XMFLOAT3 color = { 0.0f, 0.0f, 0.0f };

	m_botName = botname;

	switch (botname)
	{
		case BotNames::Larry:
			m_pBot = new MechanicalBot();
			color = { 0.33f, 0.0f, 0.25f };
			break;
		case BotNames::Talos:
			m_pBot = new StrategicalBot();
			color = { 0.85f, 0.75f, 0.25f };
			break;
		case BotNames::Karl:
			m_pBot = new Sbot();
			m_pBot->SetMaterialColor({ 0.05f, 0.25f, 0.05f });
			break;
	}
	
	m_pBot->Create(&m_botMesh, { 0.0f, 0.0f, 0.0f }, color);
	m_pBot->SetBounds(XMFLOAT2{ m_pTable->transform.pos.x - 7.0f / 2 + 0.75f,  m_pTable->transform.pos.x + 7.0f / 2 - 0.75f }, XMFLOAT2{ 15.0f, 22.0f });
	m_pBot->SetPallet(m_pPalet);
	m_pBot->SetPlayer(m_pPlayer);
	m_pBot->InitializeBot();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//States

void StateGameGlobal::OnEnter(Game& cur, int from, void* pParam)
{
}

void StateGameGlobal::OnExecute(Game& cur)
{
	// return to menu
	if (cpuInput.IsBackPressed())
		App::GetInstance().GetFsm()->ToState(CPU_ID(StateAppStartMenu));
}

void StateGameGlobal::OnExit(Game& cur, int to)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateGamePlay::OnEnter(Game& cur, int from, void* pParam)
{
	cur.StartGame();
}

void StateGamePlay::OnExecute(Game& cur)
{
	//Update
	cur.GameUpdate();

	if (cur.IsGameOver())
		cur.GetFsm()->ToState(CPU_ID(StateGameOver));
	else if (cur.IsGameWin())
		cur.GetFsm()->ToState(CPU_ID(StateGameWin));
}

void StateGamePlay::OnExit(Game& cur, int to)
{
	//Stop Game Loop
	cur.QuitGame();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateGameOver::OnEnter(Game& cur, int from, void* pParam)
{
	
}

void StateGameOver::OnExecute(Game& cur)
{
	cur.GameOverUpdate();
}

void StateGameOver::OnExit(Game& cur, int to)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StateGameWin::OnEnter(Game& cur, int from, void* pParam)
{
	
}

void StateGameWin::OnExecute(Game& cur)
{
	cur.GameWinUpdate();
}

void StateGameWin::OnExit(Game& cur, int to)
{

}