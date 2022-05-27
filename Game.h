#pragma once
#include "SFML/Window.hpp"
#include "resourceLoading.h"
#include "resource.h"
#include "Field.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Settings.h"
#include <Windows.h>

using namespace std;
using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void start();
private:
	Field m_field;
	Font* m_font;
	bool settingsOpened;
	Settings* m_settings;
	RenderWindow m_win;
	RectangleShape m_settingsButton;
	bool m_done;

	void restart();
	void resize();
	void showImGui();
	void pollEvents();

	static void showImGuiPalette(const char* title, ImVec4& color, ImVec4& refColor);
};