#pragma once
#define Button_Text_Position_Front 0
#define Button_Text_Position_Back 0

class Button
{
private:
	XMFLOAT2 m_buttonPosition;
	XMFLOAT2 m_buttonSize;
	XMFLOAT2 m_buttonAnchor;

	cpu_sprite* m_pSprite;

	XMFLOAT3 m_buttonColor;
	XMFLOAT3 m_buttonBaseColor;
	XMFLOAT3 m_buttonSelectedColor;
	XMFLOAT3 m_buttonClickedColor;

	std::string m_text;
	XMFLOAT3 m_textColor;
	int m_textAlign;

	std::function<void()> m_callback = nullptr;

public:
	Button(XMFLOAT2 buttonSize, cpu_sprite* sprite = nullptr);

	void Update();
	void Render(int pass);

	XMFLOAT2 GetButtonPosition(XMFLOAT2 anchor = { 0.5f, 0.5f });
	std::function<void()>* GetButtonFunction() { return &m_callback; }
	void SetButtonFunction(std::function<void()> function) { m_callback = function; }

	void SetPosition(XMFLOAT2 pos, XMFLOAT2 anchor = { 0.5f, 0.5f });
	//void SetButtonSize(XMFLOAT2 size) { m_buttonSize = size; } // Must update position
	void SetButtonColor(XMFLOAT3 col);
	void SetButtonSelectedColor(XMFLOAT3 col) { m_buttonSelectedColor = col; }
	void SetButtonClickedColor(XMFLOAT3 col) { m_buttonClickedColor = col; }

	void SetSprite(cpu_sprite* sprite) { m_pSprite = sprite; }
	 
	void SetTextAlign(int align) { m_textAlign = align; }
	void SetText(std::string text) { m_text = text; }
	void SetTextColor(XMFLOAT3 col) { m_textColor = col; }
	void AddText(std::string text, int position = Button_Text_Position_Front);

	bool IsIn(XMFLOAT2 point);
};