#include "Solver.h"
#include <iostream>

Solver::Solver(const vector<int>& v, const bool* solvingIndicator)
	:m_rows{ static_cast<int>(sqrt(v.size())) },
	m_solvingIndicator{ solvingIndicator },
	m_initial{ v },
	m_currentState{ v }
{
	vector<int> goal(v.size());
	iota(goal.begin(), goal.end() - 1, 1);
	goal[goal.size() - 1] = -1;

	m_goal = goal;

	m_initial.setHeuristic(m_initial - m_goal);
	m_openlist.insert(m_initial);
}

bool Solver::solve()
{
	while (m_currentState != m_goal && m_openlist.size() && (!m_solvingIndicator || *m_solvingIndicator))
	{
		gbfs();
	}

	return m_currentState == m_goal;
}

void Solver::move(const Direction& dir)
{
	int s = m_currentState.zeroIndex();
	int bias = 0;

	switch (dir)
	{
	case Direction::Up:
		bias = -m_rows;
		break;
	case Direction::Down:
		bias = m_rows;
		break;
	case Direction::Left:
		bias = -1;
		break;
	case Direction::Right:
		bias = 1;
		break;
	default:
		break;
	}

	m_currentState.swap(s, s + bias);
	m_currentState.addMove(s + bias);

	auto it = find(m_explored.begin(), m_explored.end(), m_currentState);
	
	if (it == m_explored.end())
	{
		int heuristic = m_goal - m_currentState;
		m_currentState.setHeuristic(heuristic);

		m_openlist.insert(m_currentState);
	}

	m_currentState = m_initial;
}


void Solver::gbfs()
{
	m_currentState = *m_openlist.begin();
	m_openlist.erase(m_openlist.begin());

	if (m_currentState != m_goal)
	{
		m_explored.push_back(m_currentState);

		m_initial = m_currentState;

		int zeroPos = m_currentState.zeroIndex();

		if (m_rows - 1 < zeroPos)
			move(Direction::Up);
		if (zeroPos < pow(m_rows, 2) - m_rows)
			move(Direction::Down);
		if (zeroPos % m_rows != 0)
			move(Direction::Left);
		if (zeroPos % m_rows != m_rows - 1)
			move(Direction::Right);
	}
}
