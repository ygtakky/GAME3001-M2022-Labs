#include "LOSCondition.h"

LOSCondition::LOSCondition(const bool LOS)
{
	m_name = "LOS Condition";
	SetLOS(LOS);
}

LOSCondition::~LOSCondition()
= default;

void LOSCondition::SetLOS(const bool state)
{
	m_hasLOS = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}
