#include "pch.h"
#include "window.h"

namespace ui
{
	Window::Window() = default;
	Window::~Window() = default;

	void Window::ProcessInput( const InputEvent& event )
	{
		for( auto& widget : m_widgets )
		{
			if( widget->ProcessInput( event ) == InputEventState::Handled )
			{
				return;
			}
		}
	}

	void Window::Draw( RenderContext& context ) const
	{
		// Changed loop to use 'const auto&' to ensure widgets are only read and drawn without modification.
		for( auto& widget : m_widgets )
		{
			widget->Draw( context );
		}
	}

	void Window::AddWidget( WidgetPtr widget )
	{
		// Push the widget to the vector using std::move() to ensure efficient transfer of ownership.
		// This avoids making a copy of the shared pointer when adding it to the vector.
		m_widgets.push_back( std::move( widget ) );
	}

	void Window::RemoveWidget(std::shared_ptr<Widget> widget)
	{
		// Use remove-erase idiom to remove the widget from the vector
		auto it = std::remove(m_widgets.begin(), m_widgets.end(), widget);
		if (it != m_widgets.end())
		{
			m_widgets.erase(it, m_widgets.end());
		}
	}

	const std::vector<std::shared_ptr<Widget>>& Window::GetWidgets() const
	{
		return m_widgets;
	}

}
