#include "EnemyHitCondition.h"

EnemyHitCondition::EnemyHitCondition(const bool hit)
{
	m_name = "Enemy Hit Condition";
	SetIsHit(hit);
}

EnemyHitCondition::~EnemyHitCondition()
= default;

void EnemyHitCondition::SetIsHit(const bool hit)
{
	m_isHit = hit;
}

bool EnemyHitCondition::Condition()
{
	return m_isHit;
}
