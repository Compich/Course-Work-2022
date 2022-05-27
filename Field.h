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
	Field(const Vector2f& pos, int rows, unsigned int cellSize, const Font& font, const Color& cellColor);
	Field();
	void draw(RenderWindow& win) const;

	FloatRect getGlobalBounds() const;
	Vector2f getPosition() const { return position; }
	Vector2f getSize() const { return size; }
	void setCellFillColor(const Color& newColor);

	void handleMouseClick(const Vector2f& pos);
	void move(int cell);

	bool isCorrect() const;
private:
	int m_rows;
	vector<Cell> cells;
	Vector2f position;
	Vector2f size;
};

