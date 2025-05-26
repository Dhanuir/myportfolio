#pragma once
#include "pch.h"
#include "../uiframework/TextWidget.h"

enum class RarityLevel : char
{
	Common,
	Normal,
	Rare,

	MAX
};

struct Item final
{
	std::string m_name;
	RarityLevel m_rarity;
	int m_cost;
	int m_size;
	int m_count;
	std::vector<std::string> m_attributes;
};

class Inventory final
{
public:
	Inventory();
	~Inventory();
	void Load( const std::string& path );
	std::vector<Item>& GetItems() { return m_items; }
	std::string GetOwnerName() { return m_ownerName; }
	int GetMaxSpace() const { return m_maxSpace; }
	int GetCurrentMoney() { return m_currentMoney; }
	void SetCurrentMoney(int money) { m_currentMoney = money; }

		
	void Save(const std::string& path);
	void SetPath(const std::string& path) { m_path = path; }
	std::string GetPath() { return m_path; }

private:
	std::string m_path;
	std::string m_ownerName;
	int m_maxSpace;
	int m_currentMoney;
	std::vector<Item> m_items;
};
