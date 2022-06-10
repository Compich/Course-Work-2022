#include "Equal.h"

bool Equal::operator() (const Statespace& start, const Statespace& end) const
{
	if (m_greater)
		return !(start < end);
	else
		return (start < end);
}
