#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>

#include "ActionState.h"
#include "NavigationObject.h"
#include "Obstacle.h"

class Agent : public NavigationObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void Draw() override = 0;
	void Update() override = 0;
	void Clean() override = 0;

	// getters
	[[nodiscard]] glm::vec2 GetTargetPosition() const;
	[[nodiscard]] glm::vec2 GetCurrentDirection() const;
	[[nodiscard]] float GetLOSDistance() const;
	[[nodiscard]] bool HasLOS() const;
	[[nodiscard]] float GetCurrentHeading() const;
	[[nodiscard]] glm::vec4 GetLOSColour() const;

	glm::vec2 GetLeftLOSEndPoint() const;
	glm::vec2 GetMiddleLOSEndPoint() const;
	glm::vec2 GetRightLOSEndPoint() const;
	bool* GetCollisionWhiskers(); // Returning entire array
	glm::vec4 GetLineColor(int index) const;
	float GetWhiskerAngle() const;

	ActionState GetActionState() const;

	// New For Lab 7 Part 3
	int GetHealth() const;
	void SetHealth(int value);
	void TakeDamage(int value);

	// setters
	void SetTargetPosition(glm::vec2 new_position);
	void SetCurrentDirection(glm::vec2 new_direction);
	void SetLOSDistance(float distance);
	void SetHasLOS(bool state);
	void SetHasLOS(bool state, glm::vec4 colour);

	void SetCurrentHeading(float heading);
	void SetLOSColour(glm::vec4 colour);

	void SetLeftLOSEndPoint(glm::vec2 point);
	void SetMiddleLOSEndPoint(glm::vec2 point);
	void SetRightLOSEndPoint(glm::vec2 point);
	void SetLineColor(int index, glm::vec4 color);
	void SetWhiskerAngle(float angle);

	void UpdateWhiskers(float angle);

	void SetActionState(ActionState state);

	// new for Lab 7 part 2
	// Utility function
	bool CheckAgentLOSToTarget(Agent* agent, DisplayObject* target_object, const std::vector<Obstacle*>& obstacles);
	// Virtual functions
	virtual void Attack(){}
	virtual void MoveToLOS() {}
	virtual void MoveToPlayer() {}
	virtual void MoveToRange() {}
	virtual void Patrol() {}
	// new for Lab 7 part 3
	virtual void Flee() {}
	virtual void MoveToCover() {}
	virtual void WaitBehindCover() {}

private:
	void ChangeDirection(); // From scalar to vec2.

	float m_currentHeading; // Scalar angle in degrees.
	glm::vec2 m_currentDirection; // Vec2 direction/angle.
	glm::vec2 m_targetPosition; // Where the agent wants to be.

	// LOS (line of sight)
	float m_LOSDistance; // (Position + direction) * distance
	bool m_hasLOS; // Used for collision.
	glm::vec4 m_LOSColour;

	// Whisker properties
	glm::vec2 m_leftLOSEndPoint;
	glm::vec2 m_middleLOSEndPoint;
	glm::vec2 m_rightLOSEndPoint;
	glm::vec4 m_lineColor[3]; // left, middle, right.
	bool m_collisionWhiskers[3] = { false };
	float m_whiskerAngle; // Also for you maybe a m_whiskerAngle2;

	// action state
	ActionState m_state;

	// New For Lab 7 part 3
	int m_health = 100;
};



#endif /* defined ( __AGENT__) */