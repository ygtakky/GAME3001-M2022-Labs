#include "PlayerDetectedCondition.h"

PlayerDetectedCondition::PlayerDetectedCondition(const bool detected)
{
	m_name = "Player Detected Condition";
	SetDetected(detected);
}

PlayerDetectedCondition::~PlayerDetectedCondition()
= default;

bool PlayerDetectedCondition::GetDetected() const
{
	return m_detected;
}

void PlayerDetectedCondition::SetDetected(const bool detected)
{
	m_detected = detected;
}

bool PlayerDetectedCondition::Condition()
{
	return m_detected;
}
