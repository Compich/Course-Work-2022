#pragma once

#include "Statespace.h"
#include "Equal.h"

class Solver
{
public:
	Solver(const vector<int>& v, const bool* solvingIndicator = nullptr);

	bool solve();
	vector<int> getMoves() const { return m_currentState.getMoves(); }
	int getExploredSize() const { return m_explored.size(); }
private:
	enum class Direction
	{
		Up, Down, Left, Right
	};

	int m_rows;
	const bool* m_solvingIndicator;

	Statespace m_initial;
	Statespace m_goal;
	Statespace m_currentState;
	set<Statespace, Equal> m_openlist;
	vector<Statespace> m_explored;

	void move(const Direction& dir);
	void gbfs();
};
