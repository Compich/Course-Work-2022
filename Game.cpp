#include "Game.h"

Game::Game()
	:settingsOpened{ false },
	m_settings{ Settings::getInstance() },
	m_win{ VideoMode(
		m_settings->getIndent() * 2 + m_settings->getRows() * (m_settings->getCellSize() + 1),
		m_settings->getIndent() * 2 + m_settings->getRows() * (m_settings->getCellSize() + 1)
	), L"Курсова робота", Style::Titlebar | Style::Close, ContextSettings{0, 0, 8 } },
	m_done{ false }
{
	Font* font = new Font;
	auto pFont = loadResource(IDR_FONT);
	font->loadFromMemory(pFont.first, pFont.second);

	m_font = font;
	restart();
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

	while (m_win.isOpen())
	{
		pollEvents();

		m_win.clear(m_settings->getBackgroundColor());

		m_win.draw(m_settingsButton);
		m_field.draw(m_win);

		if (settingsOpened)
		{
			showImGui();
		}

		m_win.display();
	}

	delete settingTexture;
}

void Game::showImGui()
{
	static int rows = m_settings->getRows();
	static int cellSize = m_settings->getCellSize();
	static Clock imguiClock;
	static ImVec4 backColor{ m_settings->getBackgroundColor().r / 255.0f, m_settings->getBackgroundColor().g / 255.0f, m_settings->getBackgroundColor().b / 255.0f, 1.0f };
	static ImVec4 refBackColor{ 250.0f / 255.0f, 220.0f / 255.0f, 100.0f / 255.0f, 1.0f };
	static ImVec4 cellColor{ m_settings->getCellColor().r / 255.0f, m_settings->getCellColor().g / 255.0f, m_settings->getCellColor().b / 255.0f, 1.0f };
	static ImVec4 refCellColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	ImGui::SFML::Update(m_win, imguiClock.restart());

	ImGui::Begin(u8"Налаштування");
	ImGui::SliderInt(u8"Розмір поля", &rows, 3, 10, "%d", ImGuiSliderFlags_NoInput);
	ImGui::SliderInt(u8"Ширина клітинок", &cellSize, 20, 200, "%d", ImGuiSliderFlags_NoInput);

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

	if (ImGui::Button(u8"Зберегти"))
	{
		if (rows != m_settings->getRows())
		{
			m_settings->setRows(rows);
			restart();
		}

		if (cellSize != m_settings->getCellSize())
		{
			m_settings->setCellSize(cellSize);
			restart();
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

		settingsOpened = false;
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"Перезапустити"))
	{
		restart();
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"Скасувати"))
	{
		settingsOpened = false;
	}

	ImGui::End();

	ImGui::SFML::Render(m_win);
}

void Game::restart()
{
	m_field = Field{ Vector2f{ m_settings->getIndent(), m_settings->getIndent() }, m_settings->getRows(), m_settings->getCellSize(), *m_font, m_settings->getCellColor() };
	resize();
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
			if (!settingsOpened)
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
							if (MessageBox(m_win.getSystemHandle(), L"Вітаю! Ви пройшли гру. Бажаєте перезапустити?", L"Кінець", MB_YESNO) == 6)
							{
								restart();
							}
						}
					}
					else if (m_settingsButton.getGlobalBounds().contains(mouse_pos) && !settingsOpened)
					{
						settingsOpened = !settingsOpened;
					}
				}
			}
			break;
		}
		case Event::KeyReleased:
			if (event.key.code == Keyboard::Escape && settingsOpened)
			{
				settingsOpened = !settingsOpened;
			}
			break;
		default:
			break;
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
		ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB,
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

		ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
		if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
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