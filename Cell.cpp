#include "Cell.h"

void writeToRectangle(const String& s, Text& t, const RectangleShape& r)
{
	t.setString(s);
	Vector2f pos = r.getPosition();
	Vector2f size = r.getSize();
	FloatRect lcb = t.getLocalBounds();
	t.setPosition(Vector2f(int(pos.x + (size.x - lcb.width) / 2 - lcb.left),
		int(pos.y + (size.y - lcb.height) / 2 - lcb.top)));
	
}

Cell::Cell(int n, const Font& f, unsigned int chSize, const Vector2f& size)
	:m_shape(size),
	m_text{ Text() }
{
	m_shape.setFillColor(Color::White);
	m_shape.setOutlineThickness(1);
	m_shape.setOutlineColor(Color::Black);
	m_text.setCharacterSize(chSize);
	m_text.setFont(f);
	if (n != -1)
	{
		writeToRectangle(to_string(n), m_text, m_shape);
	}
	m_text.setFillColor(Color::Black);
}