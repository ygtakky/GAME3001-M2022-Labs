#pragma once
#ifndef __ENEMY_HEALTH_CONDITION__
#define __ENEMY_HEALTH_CONDITION__

#include "ConditionNode.h"

class EnemyHealthCondition : public ConditionNode
{
public:
	EnemyHealthCondition(bool healthy = true);
	virtual ~EnemyHealthCondition();

	// Getters and Setters
	void SetHealth(bool healthy);

	virtual bool Condition() override;
private:
	bool m_healthy;
};

#endif /* defined (__ENEMY_HEALTH_CONDITION__) */