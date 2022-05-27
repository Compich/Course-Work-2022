#pragma once

#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

class Settings
{
public:
	Settings(Settings&) = delete;
	void operator=(const Settings&) = delete;
	static Settings* getInstance();

	int getRows() const { return m_rows; }
	void setRows(int rows);
	unsigned int getCellSize() const { return m_cellSize; }
	void setCellSize(unsigned int cellSize);
	unsigned int getSettingsButtonSize() const { return m_settingsButtonSize; }
	float getIndent() const;
	sf::Color getBackgroundColor() const { return m_backgroundColor; }
	void setBackgroundColor(const sf::Color& newColor) { m_backgroundColor = newColor; save(); }
	sf::Color getCellColor() const { return m_cellColor; }
	void setCellColor(const sf::Color& newColor) { m_cellColor = newColor; save(); }

	void load();
	void save();
private:
	Settings();
	static Settings* settings;

	int m_rows;
	unsigned int m_cellSize;
	unsigned int m_settingsButtonSize;
	sf::Color m_backgroundColor;
	sf::Color m_cellColor;
};