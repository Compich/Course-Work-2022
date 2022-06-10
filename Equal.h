#pragma once

#include "Statespace.h"

class Equal
{
public:
	Equal(const bool& lesser = true)
		:m_greater{ !lesser } {}

	bool operator() (const Statespace& start, const Statespace& end) const;
private:
	bool m_greater;
};