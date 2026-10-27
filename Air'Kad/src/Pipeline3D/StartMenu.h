#pragma once

class Button;

class StartMenu
{
private:
	cpu_fsm<StartMenu>* m_pFsm;

	cpu_font m_titleFont;
	cpu_font m_textFont;

	std::list<Button*> m_pStartMenuButtons;
	std::list<Button*> m_pBotSelectionButtons;

public:
	StartMenu();

	void StartMenuUpdate();
	void StartMenuRender(int pass);

	void BotSelectionMenuUpdate();
	void BotSelectionMenuRender(int pass);

	cpu_fsm<StartMenu>* GetFsm() { return m_pFsm; }
	void ChangeState(int to, void* pParam = (void*)nullptr);
};

struct StateStartMenuGlobal
{
	void OnEnter(StartMenu& cur, int from, void* pParam);
	void OnExecute(StartMenu& cur);
	void OnExit(StartMenu& cur, int to);
};

//State to put the machine in when nothing must happen (eg: Machine will be disactivated)
struct StateStartMenuInactive
{
	void OnEnter(StartMenu& cur, int from, void* pParam) {};
	void OnExecute(StartMenu& cur) {};
	void OnExit(StartMenu& cur, int to) {};
};

struct StateStartMenuIdle
{
	void OnEnter(StartMenu& cur, int from, void* pParam);
	void OnExecute(StartMenu& cur);
	void OnExit(StartMenu& cur, int to);
};

struct StateStartMenuBotSelection
{
	void OnEnter(StartMenu& cur, int from, void* pParam);
	void OnExecute(StartMenu& cur);
	void OnExit(StartMenu& cur, int to);
};