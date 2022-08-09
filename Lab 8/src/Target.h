#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "NavigationObject.h"

class Target final : public NavigationObject
{
public:
	Target();
	~Target() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	void Reset();

private:
	void Move();
	void CheckBounds();
	

	glm::vec2 m_startPos;
};


#endif /* defined (__TARGET__) */