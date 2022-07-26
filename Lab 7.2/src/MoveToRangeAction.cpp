#include "MoveToRangeAction.h"

#include <iostream>

MoveToRangeAction::MoveToRangeAction()
{
	m_name = "Move To Range Action";
}

MoveToRangeAction::~MoveToRangeAction()
= default;

void MoveToRangeAction::Action()
{
	if(GetAgent()->GetActionState() != ActionState::MOVE_TO_PLAYER)
	{
		std::cout << "Performing " << m_name << std::endl;
		GetAgent()->SetActionState(ActionState::MOVE_TO_PLAYER);
	}
	GetAgent()->MoveToRange();
}
