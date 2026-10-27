#include "pch.h"
#include "Button.h"

Button::Button(XMFLOAT2 buttonSize, cpu_sprite* sprite)
{
	m_buttonPosition = { 0.0f, 0.0f };
	m_buttonSize = { buttonSize };
	m_pSprite = sprite;
	SetButtonColor(CPU_WHITE);

	m_text = "";
	m_textColor = CPU_BLACK;
	int m_textAlign = CPU_TEXT_LEFT;
}

void Button::Update()
{

	//Check if mouse left button is pressed and do the button action
	XMFLOAT2 mousePos;
	cpuEngine.GetCursor(mousePos);
	if (IsIn(mousePos))
	{
		m_buttonColor = m_buttonSelectedColor;

		if (cpuInput.vi.IsKey(VK_LBUTTON))
		{
			m_buttonColor = m_buttonClickedColor;

			
		}
		else if (cpuInput.vi.IsKeyReleased(VK_LBUTTON))
		{
			if (m_callback != nullptr)
			{
				m_callback();
			}
		}
	}
	else
	{
		m_buttonColor = m_buttonBaseColor;
	}
}

void Button::Render(int pass)
{
	cpu_font font = App::GetInstance().GetFont();

	XMFLOAT2 buttonPos = GetButtonPosition({ 0.0f, 0.0f });
	assert((buttonPos.x >= 0 && buttonPos.x <= 1920));

	cpuDevice.FillRectangle(buttonPos.x, buttonPos.y, m_buttonSize.x, m_buttonSize.y, m_buttonColor);
	cpuDevice.DrawText(&font, m_text.c_str(), GetButtonPosition().x, GetButtonPosition().y - font.height * 0.5f, m_textAlign, &m_textColor);
}

XMFLOAT2 Button::GetButtonPosition(XMFLOAT2 anchor)
{
	float posX = m_buttonPosition.x + m_buttonSize.x * anchor.x;
	float posY = m_buttonPosition.y + m_buttonSize.y * anchor.y;

	return { posX, posY };
}

void Button::SetPosition(XMFLOAT2 pos, XMFLOAT2 anchor)
{
	m_buttonPosition.x = pos.x - m_buttonSize.x * anchor.x;
	m_buttonPosition.y = pos.y - m_buttonSize.y * anchor.y;
}

void Button::SetButtonColor(XMFLOAT3 col)
{
	m_buttonColor = col;
	m_buttonBaseColor = col;

	m_buttonSelectedColor = { col.x - 0.15f, col.y - 0.15f, col.z - 0.15f };
	m_buttonClickedColor = { col.x - 0.25f, col.y - 0.25f, col.z - 0.25f };
}

void Button::AddText(std::string text, int position)
{
	if (position == Button_Text_Position_Back)
		m_text = text + m_text;
	else
		m_text += text;
}

bool Button::IsIn(XMFLOAT2 point)
{
	XMFLOAT2 leftTopButtonPos = GetButtonPosition({ 0.0f, 0.0f });
	XMFLOAT2 rightBottomButtonPos = GetButtonPosition({ 1.0f, 1.0f });

	bool isInXBounds = point.x >= leftTopButtonPos.x && point.x <= rightBottomButtonPos.x;
	bool isInYBounds = point.y >= leftTopButtonPos.y && point.y <= rightBottomButtonPos.y;

	return isInXBounds && isInYBounds;
}
