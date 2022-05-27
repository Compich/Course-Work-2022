#include "Field.h"

template<typename T>
void shuffle(vector<T>& v)
{
	random_device rd{};
	auto rng = default_random_engine{rd()};
	shuffle(v.begin(), v.end(), rng);
}

void Field::draw(RenderWindow& win) const
{
	for (const Cell& cell : cells)
	{
		cell.draw(win);
	}
}

Field::Field(const Vector2f& pos, int rows, unsigned int cellSize, const Font& font, const Color& cellColor)
	: position{ pos.x + 1, pos.y + 1 },
	m_rows{rows}
{
	vector<int> numbers(static_cast<size_t>(pow(rows, 2) - 1));
	iota(numbers.begin(), numbers.end(), 1);
	numbers.push_back(-1);
	shuffle(numbers);

	for (int iy = 0; iy < rows; ++iy)
	{
		for (int ix = 0; ix < rows; ++ix)
		{
			Cell cell{ numbers[iy * rows + ix], font, cellSize / 2, Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)) };

			cell.setPosition(pos.x + cellSize * ix + ix + 1, pos.y + cellSize * iy + iy + 1);
			cell.setFillColor(cellColor);

			cells.push_back(cell);
		}
	}

	Cell first_cell = cells[0];
	Cell last_cell = cells[cells.size() - 1];

	float size = last_cell.getPosition().x + last_cell.getSize().x + last_cell.getOutlineThickness() - (first_cell.getPosition().x - first_cell.getOutlineThickness());

	this->size = Vector2f{ size, size };
}

Field::Field()
	:cells{ vector<Cell>{} },
	position{0, 0},
	size{0, 0},
	m_rows{ 0 }
{}

FloatRect Field::getGlobalBounds() const
{
	float left = position.x - cells[0].getOutlineThickness();
	float top = position.y - cells[0].getOutlineThickness();
	float width = size.x;
	float height = size.y;
	return FloatRect{ left, top, width, height };
}

void Field::setCellFillColor(const Color& newColor)
{
	for (Cell& cell : cells)
	{
		cell.setFillColor(newColor);
	}
}

void Field::handleMouseClick(const Vector2f& pos)
{
	int pressed_cell = -1;

	for (size_t i = 0; i < cells.size(); ++i)
	{
		if (cells[i].getGlobalBounds().contains(pos))
		{
			pressed_cell = i;
		}
	}

	if (pressed_cell != -1)
	{
		move(pressed_cell);
	}
}

void Field::move(int cell)
{
	if (m_rows <= cell && cells[cell - m_rows].isEmpty())
	{
		cells[cell - m_rows].setNum(cells[cell].getNum());
	}
	else if (cell < pow(m_rows, 2) - m_rows && cells[cell + m_rows].isEmpty())
	{
		cells[cell + m_rows].setNum(cells[cell].getNum());
	}
	else if (cell % m_rows != 0 && cells[cell - 1].isEmpty())
	{
		cells[cell - 1].setNum(cells[cell].getNum());
	}
	else if (cell % m_rows < m_rows - 1 && cells[cell + 1].isEmpty())
	{
		cells[cell + 1].setNum(cells[cell].getNum());
	}
	else
	{
		return;
	}

	cells[cell].setNum(-1);
}

bool Field::isCorrect() const
{
	for (int i = 0; i < cells.size() - 2; ++i)
	{
		if (cells[i + 1].getNum() < cells[i].getNum())
		{
			return false;
		}
	}
	cout << "correct" << endl;
	return true;
}
