#pragma once
#ifndef __TORPEDO__
#define __TORPEDO__

#include "TorpedoAnimationState.h"
#include "Sprite.h"


class Torpedo final : public Sprite
{
public:
	Torpedo(float speed = 0.0f);
	~Torpedo();

	// Life Cycle Methods
	void Draw() override;
	void Update() override;
	void Clean() override;

	// setters
	void SetAnimationState(TorpedoAnimationState new_state);
private:
	void BuildAnimations();

	TorpedoAnimationState m_currentAnimationState;

	float m_speed;
};

#endif /* defined (__TORPEDO__)*/