#pragma once

class Palet;
class Pusher;
class Bot;
class Button;

enum class BotNames
{
	Larry,
	Talos,
	Karl
};

class Game
{
private:
	cpu_fsm<Game>* m_pFsm;

	cpu_mesh m_skyBoxMesh;
	cpu_mesh m_botMesh;
	cpu_mesh m_playerMesh;
	cpu_mesh m_tableMesh;
	cpu_mesh m_tableBordMesh;

	cpu_entity* m_pTable;
	cpu_entity* m_pTableBord1;
	cpu_entity* m_pTableBord2;
	cpu_entity* m_pSkyBox;

	Pusher* m_pPlayer;
	Bot* m_pBot;
	Palet* m_pPalet;

	BotNames m_botName;

	int m_playerScore;
	int m_botScore;

	float m_gameTimerSeconds;
	int m_gameTimerMinutes;

	XMFLOAT2 m_playerVel = { 0.0f, 0.0f };
	XMFLOAT2 m_botVel = { 0.0f, 0.0f };

	float m_FixedDeltaTime = 0.00001f;
	float m_Accumulator = 0.0f;
	
	std::list<Button*> m_pGameOverButtons;
	std::list<Button*> m_pGameWinButtons;

	bool m_gameOver;
	bool m_gameWin;
	bool m_overtime;

public:
	Game();

	cpu_fsm<Game>* GetFsm() { return m_pFsm; }

	void FixedUpdate(float fixedDt);
	void StartGame();
	void GameUpdate();
	void GameRenderUpdate(int pass);

	void GameRender(int pass);
	void QuitGame();

	void ActivateObjects(bool activate);

	void Goal();

	void GameWinUpdate();
	void GameWinRender(int pass);

	void GameOverUpdate();
	void GameOverRender(int pass);

	void UpdateTimer();

	bool IsGameOver() { return m_gameOver; }
	bool IsGameWin() { return m_gameWin; }
	bool IsOvertime() { return m_overtime; }

	void SetBot(BotNames botname);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct StateGameGlobal
{
	void OnEnter(Game& cur, int from, void* pParam);
	void OnExecute(Game& cur);
	void OnExit(Game& cur, int to);
};

//State to put the machine in when nothing must happen (eg: Machine will be disactivated)
struct StateGameInactive
{
	void OnEnter(Game& cur, int from, void* pParam) {};
	void OnExecute(Game& cur) {};
	void OnExit(Game& cur, int to) {};
};

struct StateGamePlay
{
	void OnEnter(Game& cur, int from, void* pParam);
	void OnExecute(Game& cur);
	void OnExit(Game& cur, int to);
};

struct StateGameOver
{
	void OnEnter(Game& cur, int from, void* pParam);
	void OnExecute(Game& cur);
	void OnExit(Game& cur, int to);
};

struct StateGameWin
{
	void OnEnter(Game& cur, int from, void* pParam);
	void OnExecute(Game& cur);
	void OnExit(Game& cur, int to);
};