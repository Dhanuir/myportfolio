#pragma once

namespace ui
{
	class Widget
	{
	public:
		Widget( float posX, float posY, float width, float height );
		virtual ~Widget();

		virtual InputEventState ProcessInput(const sf::Event& event);
		virtual void Draw( RenderContext& context ) const = 0;

		virtual float GetPosX() const;
		virtual float GetPosY() const;

		virtual float GetWidth() const;
		virtual float GetHeight() const;

		virtual sf::FloatRect GetBounds() const;

		virtual bool IsPointerOver(const sf::Vector2i mouseposition) const
		{
			return (mouseposition.x >= m_posX && mouseposition.x <= (m_posX + m_width) && mouseposition.y >= m_posY && mouseposition.y <= (m_posY + m_height));
		}
		virtual void SetPosition(float x, float y)
		{
			m_posX = x;
			m_posY = y;
		}

	private:
		float m_posX;
		float m_posY;
		float m_width;
		float m_height;
	};

	// Create a type alias for WidgetPtr to ensure all widget pointers are consistently using std::shared_ptr<Widget>.
	// This alias helps avoid type mismatches when managing widgets in different classes.
	using WidgetPtr = std::shared_ptr< Widget >;
}

