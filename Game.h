#pragma once
#include "SFML/Window.hpp"
#include "resourceLoading.h"
#include "resource.h"
#include "Field.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Settings.h"
#include <Windows.h>
#include "Solver.h"
#include <thread>
#include <chrono>
#include <sstream>

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
	bool m_settingsOpened;
	bool m_solveError;
	bool m_done;
	bool m_solving;
	Settings* m_settings;
	RenderWindow m_win;
	RectangleShape m_settingsButton;

	void restart(const vector<int>& positions = vector<int>{});
	void resize();
	void showImGui();
	void pollEvents();
	void solve();

	void showImGuiPalette(const char* title, ImVec4& color, ImVec4& refColor);
};