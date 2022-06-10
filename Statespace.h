#pragma once

#include <vector>
#include <set>
#include <numeric>

using namespace std;

class Statespace
{
public:
	Statespace(vector<int>v = vector<int>{})
		:m_puzzle{ v },
		m_heuristic{ 0 }
	{}

	void addMove(int move) { m_moves.push_back(move); }
	vector<int> getMoves() const { return m_moves; }
	int getHeuristic() const { return m_heuristic; }
	void setHeuristic(int newHeuristic) { m_heuristic = newHeuristic; }

	int zeroIndex() const { return find(m_puzzle.begin(), m_puzzle.end(), -1) - m_puzzle.begin(); }
	void swap(int i, int j) { std::swap(m_puzzle[i], m_puzzle[j]); }

	int& operator[] (const int index) { return m_puzzle[index]; }
	const int& operator[] (const int index) const { return m_puzzle[index]; }
	bool operator< (const Statespace& right) const;
	int operator- (const Statespace& right) const;
	bool operator== (const Statespace& right) const;
	bool operator!= (const Statespace& right) const;
private:
	vector<int> m_puzzle;
	vector<int> m_moves;
	int m_heuristic;
};

