#pragma once

#include "widget.h"

namespace ui
{	
	class Window final
	{
	public:
		Window();
		~Window();
		
		void ProcessInput( const InputEvent& event );
		void Draw( RenderContext& context ) const;

		// Changed AddWidget to accept WidgetPtr by value instead of WidgetPtr&&.
		// This makes the function more flexible and allows passing both temporary (rvalue) and existing (lvalue) shared pointers.
		// It also simplifies the code by avoiding the need for explicit std::move() at the call site.
		void AddWidget( WidgetPtr widget );
		void RemoveWidget(std::shared_ptr<Widget> widget);
		const std::vector<std::shared_ptr<Widget>>& GetWidgets() const;
		
	private:
		std::vector< WidgetPtr > m_widgets;
		
	};
}
