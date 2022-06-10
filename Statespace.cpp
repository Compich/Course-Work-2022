#include "Statespace.h"

bool Statespace::operator< (const Statespace& right) const
{
	return (m_heuristic < right.m_heuristic);
}

int Statespace::operator- (const Statespace& right) const
{
	int count = 0;
	for (int i = 0; i < m_puzzle.size(); i++)
	{
		if ((*this)[i] != right[i] && (*this)[i] != -1)
			++count;
	}
	return count;
}


//bool Statespace::operator== (const Statespace& right) const
//{
//	return *this - right == 0;
//}

bool Statespace::operator== (const Statespace& right) const
{
	for (size_t i = 0; i < m_puzzle.size(); ++i)
	{
		if ((*this)[i] != right[i])
		{
			return false;
		}
	}
	return true;
}

bool Statespace::operator!= (const Statespace& right) const
{
	return !(*this == right);
}
