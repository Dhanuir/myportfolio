#pragma once
#include "../uiframework/BackgroundChanger.h"
#include "../uiframework/pointerwidget.h"
#include "../uiframework/InventoryContainer.h"
#include "../uiframework/ButtonWidget.h"
#include "../uiframework/TradingWidget.h"

class Inventory;

namespace ui
{
	class Window;
	class PointerWidget;
	class SortingWidget;
}

class Application final
{
public:
	Application();
	~Application();

	void Initialize();
	void Run();

	ui::Window* GetMainWindow() const;
	const std::string& GetDataPath() const;

	void InitiateTrade(std::shared_ptr<ui::TradingWidget> TradingWidget);
	void PrintVendorMoney()
	{
		if (m_playerInventory) { std::cout << m_playerInventory->GetCurrentMoney(); }
	}
	

private:
	void SetDataPath( const char* dataPath );
	void LoadInventories();

	std::string m_dataPath;

	std::shared_ptr< ui::Window > m_mainWindow;
	std::unique_ptr< RenderContext > m_renderContext;

	std::shared_ptr< Inventory > m_playerInventory;
	std::shared_ptr< Inventory > m_vendorInventory;

	std::shared_ptr<ui::InventoryContainer> m_playerInventoryContainer;
	std::shared_ptr<ui::InventoryContainer> m_vendorInventoryContainer;

	std::unique_ptr<BackgroundChanger> m_backgroundChanger;
	std::unique_ptr<ui::PointerWidget> m_pointerWidget;

	std::shared_ptr<ui::SortingWidget> playerSortingWidget;
	std::shared_ptr<ui::SortingWidget> vendorSortingWidget;

	bool m_isDraggingItem = false;
	bool m_isDraggingPlayerItem = false;
	bool m_isDraggingVendorItem = false;

	std::shared_ptr<sf::Sprite> m_draggedItemSprite;
	std::shared_ptr<Item> m_draggedItem;
	std::shared_ptr<ui::ButtonWidget> playerSortButton;
	std::shared_ptr<ui::ButtonWidget> vendorSortButton;

	std::shared_ptr<ui::TradingWidget> playerTradingWidget;
	std::shared_ptr<ui::TradingWidget> vendorTradingWidget;
};
