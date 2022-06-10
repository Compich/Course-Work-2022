#include "Game.h"

Game::Game()
	:m_settingsOpened{ false },
	m_settings{ Settings::getInstance() },
	m_win{ VideoMode(
		m_settings->getIndent() * 2 + m_settings->getRows() * (m_settings->getCellSize() + 1),
		m_settings->getIndent() * 2 + m_settings->getRows() * (m_settings->getCellSize() + 1)
	), L"Курсова робота", Style::Titlebar | Style::Close, ContextSettings{0, 0, 8 } },
	m_done{ false },
	m_solving{ false }
{
	Font* font = new Font;
	auto pFont = loadResource(IDR_FONT);
	font->loadFromMemory(pFont.first, pFont.second);

	m_font = font;
	restart(vector<int>{1, 5, 8, 4, 6, 2, 3, -1, 7});
}

Game::~Game()
{
	m_settings->save();
	delete m_font;
}

void Game::start()
{
	ContextSettings settings(0, 0, 8);

	Image icon;
	pair<LPVOID, DWORD> ptrIcon = loadResource(IDR_ICON);
	icon.loadFromMemory(ptrIcon.first, ptrIcon.second);
	m_win.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	ImGui::SFML::Init(m_win, false);

	auto pFont = loadResource(IDR_FONT);
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	ImFontConfig fontCfg;
	fontCfg.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF(pFont.first, pFont.second, 16, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	ImGui::SFML::UpdateFontTexture();

	ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8);

	m_settingsButton = RectangleShape{ Vector2f(m_settings->getSettingsButtonSize(), m_settings->getSettingsButtonSize()) };

	Image settingsImage;
	pair<LPVOID, DWORD> ptrSettingsImage = loadResource(IDR_SETTINGS);
	settingsImage.loadFromMemory(ptrSettingsImage.first, ptrSettingsImage.second);

	Texture* settingTexture = new Texture;
	settingTexture->loadFromImage(settingsImage);
	m_settingsButton.setTexture(settingTexture);

	Clock imguiClock;

	int rows = m_settings->getRows();

	m_win.setVerticalSyncEnabled(true);

	bool showInfoMessage = false;

	while (m_win.isOpen())
	{
		pollEvents();

		m_win.clear(m_settings->getBackgroundColor());

		m_win.draw(m_settingsButton);
		m_field.draw(m_win);

		if (m_settingsOpened)
		{
			showImGui();
		}

		m_win.display();

		if (!showInfoMessage)
		{
			MessageBox(
				m_win.getSystemHandle(),
				L"Вітаю!\n"
				L"Ви зайшли в \"Гру у 15\"\n"
				L"Мета гри полягає в тому, щоб усі клітинки на полі стояли в порядку зростання їх номерів, а остання клітинка була порожньою\n"
				L"Якщо Ви хочете змінити налаштування, ви можете зробити це, натиснувши на відповідну кнопку в лівому верхньому куті вікна\n"
				L"Там же Ви зможете подивитися на розв'язання поля\n"
				L"Приємної гри!",
				L"Вітаю у грі!",
				MB_OK
			);
			showInfoMessage = true;
		}
	}

	delete settingTexture;
}

void Game::showImGui()
{
	static int rows = m_settings->getRows();
	static int cellSize = m_settings->getCellSize();
	static bool customField = false;
	static bool showError = false;
	static string errorMessage = "";
	static Clock imguiClock;
	static ImVec4 backColor{ m_settings->getBackgroundColor().r / 255.0f, m_settings->getBackgroundColor().g / 255.0f, m_settings->getBackgroundColor().b / 255.0f, 1.0f };
	static Color clRefBackColor = Settings::getDefaults()->getBackgroundColor();
	static ImVec4 refBackColor{ clRefBackColor.r / 255.0f, clRefBackColor.g / 255.0f, clRefBackColor.b / 255.0f, clRefBackColor.a / 255.0f };
	static ImVec4 cellColor{ m_settings->getCellColor().r / 255.0f, m_settings->getCellColor().g / 255.0f, m_settings->getCellColor().b / 255.0f, 1.0f };
	static Color clRefCellColor = Settings::getDefaults()->getCellColor();
	static ImVec4 refCellColor{ clRefCellColor.r / 255.0f, clRefCellColor.g / 255.0f, clRefCellColor.b / 255.0f, clRefCellColor.a / 255.0f };

	ImGui::SFML::Update(m_win, imguiClock.restart());

	ImGui::Begin(u8"Налаштування", &m_settingsOpened);
	ImGui::SliderInt(u8"Розмірність поля", &rows, 3, 10, "%d", ImGuiSliderFlags_NoInput);
	ImGui::SliderInt(u8"Розмір клітинок", &cellSize, 20, 200, "%d", ImGuiSliderFlags_NoInput);

	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f;
		}
		saved_palette_init = false;
	}

	bool openBackgroundColPalette = ImGui::ColorButton("BackColor##3b", backColor);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	openBackgroundColPalette |= ImGui::Button(u8"Фоновий колір");
	if (openBackgroundColPalette)
	{
		ImGui::OpenPopup("backcolor");
	}
	if (ImGui::BeginPopup("backcolor"))
	{
		showImGuiPalette(u8"Вибір фонового кольору", backColor, refBackColor);
	}

	bool openCellColPalette = ImGui::ColorButton("CellColor##3b", cellColor);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	openCellColPalette |= ImGui::Button(u8"Колір клітинок");
	if (openCellColPalette)
	{
		ImGui::OpenPopup("cellcolor");
	}
	if (ImGui::BeginPopup("cellcolor"))
	{
		showImGuiPalette(u8"Вибір кольору клітинок", cellColor, refCellColor);
	}

	static char texBufs[100][3];

	ImGui::Checkbox(u8"Задати власне поле", &customField);

	ImGui::SameLine();

	ostringstream oss;
	oss << u8"Якщо Ви хочете, щоб номери на полі були"
		<< endl
		<< u8"довільними, Ви можете зробити це. Пам'ятайте," 
		<< endl
		<< u8"що всі номери мають бути унікальними від 0 до"
		<< endl
		<< static_cast<int>(pow(rows, 2) - 1)
		<< u8", де 0 означає порожню клітинку";

	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(oss.str().c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	if (customField && ImGui::BeginTable("table_padding_2", rows, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		static bool init = true;
		if (init)
		{
			for (int i = 0; i < 100; ++i)
			{
				strcpy_s(texBufs[i], "0");
			}
		}
		for (int cell = 0; cell < pow(rows, 2); cell++)
		{
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::PushID(cell);
			ImGui::InputText("##cell", texBufs[cell], ceil(log(pow(rows, 2)) / log(10)) + 1);
			ImGui::PopID();
		}
		init = false;
		ImGui::EndTable();
	}

	if (ImGui::Button(u8"Зберегти"))
	{
		bool correctField = true;
		vector<int> numbers{};

		if (customField)
		{
			for (int i = 0; i < pow(rows, 2); ++i)
			{
				string s = texBufs[i];
				bool isNum = true;
				for (const char& c : s)
				{
					if (!isdigit(c))
					{
						isNum = false;
						correctField = false;
					}
				}
				if (isNum)
				{
					int num = s.length() ? stoi(s) : -1;
					if (num == 0)
					{
						num = -1;
					}
					if (pow(rows, 2) <= num || find(numbers.begin(), numbers.end(), num) != numbers.end())
					{
						correctField = false;
					}
					numbers.push_back(num);
				}
			}
		}

		if (correctField)
		{
			if (numbers.size() || rows != m_settings->getRows() || cellSize != m_settings->getCellSize())
			{
				m_settings->setRows(rows);
				m_settings->setCellSize(cellSize);
				restart(numbers);
			}

			Color clBackgroundColor{ static_cast<Uint8>(backColor.x * 255), static_cast<Uint8>(backColor.y * 255), static_cast<Uint8>(backColor.z * 255) };
			if (clBackgroundColor != m_settings->getBackgroundColor())
			{
				m_settings->setBackgroundColor(clBackgroundColor);
			}

			Color clCellColor{ static_cast<Uint8>(cellColor.x * 255), static_cast<Uint8>(cellColor.y * 255), static_cast<Uint8>(cellColor.z * 255) };
			if (clCellColor != m_settings->getCellColor())
			{
				m_settings->setCellColor(clCellColor);
				m_field.setCellFillColor(clCellColor);
			}

			m_settingsOpened = false;
		}
		else
		{
			showError = true;
			errorMessage = u8"Ви ввели некоректні\nномери клітинок! Всі\nзначення мають бути\nунікальними, а одна\nклітинка має бути\nпозначена як \"0\",\nпозначаючи\nпорожню клітинку";
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"Перезапустити"))
	{
		restart();
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"Розв'язати"))
	{
		m_solveError = false;
		thread solvingThread{ &Game::solve, this };
		solvingThread.detach();
	}

	if (m_solving)
	{
		ImGui::OpenPopup("solving_popup");
	}
	if (ImGui::BeginPopup("solving_popup"))
	{
		if (m_solving)
		{
			ImGui::Text(u8"Пошук розв'язання..\nДля зупинки натисніть\nEscape");
		}
		else if (m_solveError)
		{
			showError = true;
			errorMessage = u8"На жаль, розв'язати дане поле неможливо";
		}

		ImGui::EndPopup();
	}

	if (showError)
	{
		ImGui::OpenPopup("show_error");
		showError = false;
	}
	if (ImGui::BeginPopup("show_error"))
	{
		ImGui::Text(errorMessage.c_str());

		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::SFML::Render(m_win);
}

void Game::restart(const vector<int>& positions)
{
	auto numbers = vector<int>{ 4, 5, 2, 8, 6, -1, 7, 3, 1 };
	m_field = Field{ Vector2f{ m_settings->getIndent(), m_settings->getIndent() }, m_settings->getRows(), m_settings->getCellSize(), *m_font, m_settings->getCellColor(), positions};
	resize();
	m_done = false;
}

void Game::resize()
{
	Vector2u newSize{ static_cast<unsigned int>(m_settings->getIndent() * 2 + m_field.getSize().x), static_cast<unsigned int>(m_settings->getIndent() * 2 + m_field.getSize().y) };
	m_win.setSize(newSize);
}

void Game::pollEvents()
{
	Event event;

	while (m_win.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		switch (event.type)
		{
		case Event::Closed:
			m_win.close();
			break;
		case Event::Resized:
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			m_win.setView(sf::View(visibleArea));
			resize();
			break;
		}
		case Event::MouseButtonReleased:
		{
			if (!m_settingsOpened)
			{
				Vector2f mouse_pos{ static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y) };
				if (event.mouseButton.button == Mouse::Button::Left)
				{
					if (m_field.getGlobalBounds().contains(mouse_pos))
					{
						m_field.handleMouseClick(mouse_pos);
						if (m_field.isCorrect() && !m_done)
						{
							m_done = true;
							if (MessageBox(nullptr, L"Вітаю! Ви пройшли гру. Бажаєте перезапустити?", L"Кінець", MB_YESNO) == 6)
							{
								restart();
							}
						}
					}
					else if (m_settingsButton.getGlobalBounds().contains(mouse_pos) && !m_settingsOpened)
					{
						m_settingsOpened = !m_settingsOpened;
					}
				}
			}
			break;
		}
		case Event::KeyReleased:
			if (event.key.code == Keyboard::Escape)
			{
				if (m_settingsOpened)
				{
					m_settingsOpened = false;
				}
				if (m_solving)
				{
					m_solving = false;
				}
			}
			break;
		default:
			break;
		}
	}
}

#include <iostream>

void Game::solve()
{
	m_solving = true;
	Solver solver(m_field.getNumbers(), &m_solving);
	bool solved = solver.solve();

	cout << solver.getExploredSize() << endl;
	if (m_solving)
	{
		if (solved && m_solving)
		{
			m_solving = false;
			m_done = true;
			//m_settingsOpened = false;
			vector<int> moves = solver.getMoves();

			for (int move : moves)
			{
				m_field.move(move);
				//this_thread::sleep_for(chrono::milliseconds(250));
			}
		}
		else if (m_solving)
		{
			m_solving = false;
			m_solveError = true;
			//this_thread::sleep_for(chrono::seconds(5));
			m_solveError = false;
		}
	}
}

void Game::showImGuiPalette(const char* title, ImVec4& color, ImVec4& refColor)
{
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f;
		}
		saved_palette_init = false;
	}

	ImGui::Text(title);
	ImGui::Separator();
	ImGui::ColorPicker4(
		u8"##picker",
		reinterpret_cast<float*>(&color),
		ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoInputs,
		reinterpret_cast<float*>(&refColor)
	);
	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text(u8"Поточний");
	ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
	ImGui::Text(u8"Стандартний");
	if (ImGui::ColorButton("##default", refColor, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
	{
		color = refColor;
	}
	ImGui::Separator();
	ImGui::Text(u8"Палітра");
	for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
	{
		ImGui::PushID(n);
		if ((n % 8) != 0)
			ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

		if (ImGui::ColorButton("##palette", saved_palette[n], ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)))
			color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
				memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
				memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
			ImGui::EndDragDropTarget();
		}

		ImGui::PopID();
	}
	ImGui::EndGroup();
	ImGui::EndPopup();
}