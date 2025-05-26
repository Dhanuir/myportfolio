#include "pch.h"
#include "widget.h"

namespace ui
{
	Widget::Widget(float posX, float posY, float width, float height )
		: m_posX( posX )
		, m_posY( posY )
		, m_width( width )
		, m_height( height )
	{
	}

	Widget::~Widget() = default;

	float Widget::GetPosX() const
	{
		return m_posX;
	}

	float Widget::GetPosY() const
	{
		return m_posY;
	}

	float Widget::GetWidth() const
	{
		return m_width;
	}

	float Widget::GetHeight() const
	{
		return m_height;
	}

	sf::FloatRect Widget::GetBounds() const
	{
		return sf::FloatRect(static_cast<float>(m_posX), static_cast<float>(m_posY),
			static_cast<float>(m_width), static_cast<float>(m_height));
	}

	InputEventState Widget::ProcessInput(const sf::Event& event)
	{
		// The base implementation can be left empty or have some generic input handling logic
		return InputEventState::Unhandled;
	}
}
