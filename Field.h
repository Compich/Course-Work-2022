#pragma 
#include "Cell.h"
#include <random>
#include <algorithm>
#include <numeric>

using namespace std;

template<typename T>
void shuffle(vector<T>& v);

class Field
{
public:
	Field(const Vector2f& pos, int rows, unsigned int cellSize, const Font& font, const Color& cellColor, vector<int> numbers = vector<int>{});
	Field();
	void draw(RenderWindow& win) const;

	FloatRect getGlobalBounds() const;
	Vector2f getPosition() const { return m_position; }
	Vector2f getSize() const { return m_size; }
	void setCellFillColor(const Color& newColor);
	vector<int> getNumbers() const;

	void handleMouseClick(const Vector2f& pos);
	void move(int cell);

	bool isCorrect() const;
private:
	int m_rows;
	vector<Cell> m_cells;
	Vector2f m_position;
	Vector2f m_size;

	int getInversions() const;
	bool isSolvable() const;
};

