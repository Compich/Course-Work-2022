#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

void writeToRectangle(const String& s, Text& t, const RectangleShape& r);

class Cell
{
public:
	Cell(int n, const Font& font, unsigned int chSize, const Vector2f& size);

	void draw(RenderWindow& win) const { win.draw(m_shape); win.draw(m_text); }

	void setPosition(float x, float y) { m_shape.setPosition(x, y); writeToRectangle(m_text.getString(), m_text, m_shape); }
	Color getFillColor() const { return m_shape.getFillColor(); }
	void setFillColor(const Color& c) { m_shape.setFillColor(c); }
	Vector2f getSize() const { return m_shape.getSize(); }
	Vector2f getPosition() const { return m_shape.getPosition(); }
	float getOutlineThickness() const { return m_shape.getOutlineThickness(); }
	FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }

	bool isEmpty() const { return !(m_text.getString().getSize()); }
	int getNum() const { string s = m_text.getString(); return s.size() == 0 ? -1 : stoi(s); }
	void setNum(int n) { string s = n == -1 ? "" : to_string(n); writeToRectangle(s, m_text, m_shape); }
private:
	RectangleShape m_shape;
	Text m_text;
};
