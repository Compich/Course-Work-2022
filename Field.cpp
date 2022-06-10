#include "Field.h"

template<typename T>
void shuffle(vector<T>& v)
{
	random_device rd{};
	auto rng = default_random_engine{ rd() };
	shuffle(v.begin(), v.end(), rng);
}

void Field::draw(RenderWindow& win) const
{
	for (const Cell& cell : m_cells)
	{
		cell.draw(win);
	}
}

Field::Field(const Vector2f& pos, int rows, unsigned int cellSize, const Font& font, const Color& cellColor, vector<int> numbers)
	: m_position{ pos.x + 1, pos.y + 1 },
	m_rows{ rows }
{
	bool randomNumbers = !numbers.size();

	while ((!randomNumbers && !m_cells.size()) || !numbers.size() || (randomNumbers && !isSolvable()))
	{
		if (randomNumbers)
		{
			numbers = vector<int>(static_cast<size_t>(pow(rows, 2) - 1));
			iota(numbers.begin(), numbers.end(), 1);
			numbers.push_back(-1);
			shuffle(numbers);
		}

		if (m_cells.size())
		{
			for (size_t i = 0; i < numbers.size(); ++i)
			{
				m_cells[i].setNum(numbers[i]);
			}
		}
		else
		{
			for (int iy = 0; iy < rows; ++iy)
			{
				for (int ix = 0; ix < rows; ++ix)
				{
					Cell cell{ numbers[iy * rows + ix], font, cellSize / 2, Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)) };

					cell.setPosition(pos.x + cellSize * ix + ix + 1, pos.y + cellSize * iy + iy + 1);
					cell.setFillColor(cellColor);

					m_cells.push_back(cell);
				}
			}
		}
	}

	Cell first_cell = m_cells[0];
	Cell last_cell = m_cells[m_cells.size() - 1];

	float size = last_cell.getPosition().x + last_cell.getSize().x + last_cell.getOutlineThickness() - (first_cell.getPosition().x - first_cell.getOutlineThickness());

	this->m_size = Vector2f{ size, size };
}

Field::Field()
	:m_cells{ vector<Cell>{} },
	m_position{ 0, 0 },
	m_size{ 0, 0 },
	m_rows{ 0 }
{}

FloatRect Field::getGlobalBounds() const
{
	float left = m_position.x - m_cells[0].getOutlineThickness();
	float top = m_position.y - m_cells[0].getOutlineThickness();
	float width = m_size.x;
	float height = m_size.y;
	return FloatRect{ left, top, width, height };
}

void Field::setCellFillColor(const Color& newColor)
{
	for (Cell& cell : m_cells)
	{
		cell.setFillColor(newColor);
	}
}

vector<int> Field::getNumbers() const
{
	vector<int> numbers;

	for (const Cell& c : m_cells)
	{
		numbers.push_back(c.getNum());
	}

	return numbers;
}

void Field::handleMouseClick(const Vector2f& pos)
{
	int pressed_cell = -1;

	for (size_t i = 0; i < m_cells.size(); ++i)
	{
		if (m_cells[i].getGlobalBounds().contains(pos))
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
	if (m_rows <= cell && m_cells[cell - m_rows].isEmpty())
	{
		m_cells[cell - m_rows].setNum(m_cells[cell].getNum());
	}
	else if (cell < pow(m_rows, 2) - m_rows && m_cells[cell + m_rows].isEmpty())
	{
		m_cells[cell + m_rows].setNum(m_cells[cell].getNum());
	}
	else if (cell % m_rows != 0 && m_cells[cell - 1].isEmpty())
	{
		m_cells[cell - 1].setNum(m_cells[cell].getNum());
	}
	else if (cell % m_rows < m_rows - 1 && m_cells[cell + 1].isEmpty())
	{
		m_cells[cell + 1].setNum(m_cells[cell].getNum());
	}
	else
	{
		return;
	}

	m_cells[cell].setNum(-1);
}

bool Field::isCorrect() const
{
	for (size_t i = 0; i < m_cells.size(); ++i)
	{
		if (m_cells[i].getNum() != i + 1)
		{
			return false;
		}
	}
	return m_cells[m_cells.size() - 1].getNum() == -1;
}

int Field::getInversions() const
{
	int inversions = 0;

	for (size_t i = 0; i < m_cells.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < m_cells.size(); ++j)
		{
			if (m_cells[i].getNum() != -1 && m_cells[j].getNum() != -1 && m_cells[j].getNum() < m_cells[i].getNum())
			{
				++inversions;
			}
		}
	}

	return inversions;
}

bool Field::isSolvable() const
{
	int inversions = getInversions();

	if (m_cells.size() % 2 == 1)
	{
		return inversions % 2 == 0;
	}
	else
	{
		int pos = -1;

		for (int i = m_cells.size() - 1; 0 <= i; --i)
		{
			if (m_cells[i].getNum() == -1)
			{
				pos = static_cast<size_t>(sqrt(m_cells.size())) - i / static_cast<size_t>(sqrt(m_cells.size()));
			}
		}
		if (pos % 2 == 1)
		{
			return inversions % 2 == 0;
		}
		else
		{
			return inversions % 2 == 1;
		}
	}
}