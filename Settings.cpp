#include "Settings.h"

Settings* Settings::settings = nullptr;

Settings::Settings()
{
	load();
}

Settings* Settings::getInstance()
{
	if (!settings)
	{
		settings = new Settings;
	}
	return settings;
}

void Settings::setRows(int rows)
{
	m_rows = rows;

	save();
}

void Settings::setCellSize(unsigned int cellSize)
{
	m_cellSize = cellSize;

	save();
}

float Settings::getIndent() const
{
	return static_cast<float>(m_cellSize / 2 < m_settingsButtonSize ? m_settingsButtonSize : m_cellSize / 2);
}

void Settings::load()
{
	std::ifstream ist{ "settings", std::ios_base::binary };
	if (!ist.read(reinterpret_cast<char*>(&m_rows), sizeof(m_rows)))
	{
		m_rows = 4;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_cellSize), sizeof(m_cellSize)))
	{
		m_cellSize = 100;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_settingsButtonSize), sizeof(m_settingsButtonSize)))
	{
		m_settingsButtonSize = 35;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_backgroundColor), sizeof(m_backgroundColor)))
	{
		m_backgroundColor = sf::Color(250, 220, 100);
	}
	if (!ist.read(reinterpret_cast<char*>(&m_cellColor), sizeof(m_cellColor)))
	{
		m_cellColor = sf::Color::White;
	}

	save();
}

void Settings::save()
{
	std::ofstream ost{ "settings", std::ios_base::binary };
	ost.write(reinterpret_cast<char*>(&m_rows), sizeof(m_rows));
	ost.write(reinterpret_cast<char*>(&m_cellSize), sizeof(m_cellSize));
	ost.write(reinterpret_cast<char*>(&m_settingsButtonSize), sizeof(m_settingsButtonSize));
	ost.write(reinterpret_cast<char*>(&m_backgroundColor), sizeof(m_backgroundColor));
	ost.write(reinterpret_cast<char*>(&m_cellColor), sizeof(m_cellColor));
}