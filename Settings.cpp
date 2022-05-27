#include "Settings.h"

Settings* Settings::m_settings = nullptr;
Settings* Settings::m_defaults = nullptr;

Settings::Settings(bool defaults)
{
	if (!defaults)
	{
		load();
	}
	else
	{
		m_rows = 4;
		m_cellSize = 100;
		m_settingsButtonSize = 35;
		m_backgroundColor = sf::Color(250, 220, 100);
		m_cellColor = sf::Color::White;
	}
}

Settings* Settings::getInstance()
{
	if (!m_settings)
	{
		m_settings = new Settings;
	}
	return m_settings;
}

Settings* Settings::getDefaults()
{
	if (!m_defaults)
	{
		m_defaults = new Settings(true);
	}
	return m_defaults;
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
	if (!m_defaults)
	{
		getDefaults();
	}

	std::ifstream ist{ "settings", std::ios_base::binary };
	if (!ist.read(reinterpret_cast<char*>(&m_rows), sizeof(m_rows)))
	{
		m_rows = m_defaults->m_rows;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_cellSize), sizeof(m_cellSize)))
	{
		m_cellSize = m_defaults->m_cellSize;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_settingsButtonSize), sizeof(m_settingsButtonSize)))
	{
		m_settingsButtonSize = m_defaults->m_settingsButtonSize;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_backgroundColor), sizeof(m_backgroundColor)))
	{
		m_backgroundColor = m_defaults->m_backgroundColor;
	}
	if (!ist.read(reinterpret_cast<char*>(&m_cellColor), sizeof(m_cellColor)))
	{
		m_cellColor = m_defaults->m_cellColor;
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