#include "LOSCondition.h"

LOSCondition::LOSCondition()
{
	m_name = "LOS Condition";
}

LOSCondition::~LOSCondition()
= default;

bool LOSCondition::Condition()
{
	return GetAgent()->HasLOS();
}
