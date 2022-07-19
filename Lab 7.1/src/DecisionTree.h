#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

#include <vector>

class DecisionTree
{
public:
	// Constructor(s)
	DecisionTree(); // default constructor
	DecisionTree(Agent* agent);

	// Destructor
	~DecisionTree();

	// Getters and Setters (Accessors and Mutators)
	[[nodiscard]] Agent* GetAgent() const;
	void SetAgent(Agent* agent);

	LOSCondition* GetLOSNode() const;
	RadiusCondition* GetRadiusNode() const;
	CloseCombatCondition* GetCloseCombatNode() const;

	// Public Utility / Convenience Methods
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();
	void Clean();

	void MakeDecision(); // in-order traversal

private:
	// Private Data Members (Fields)
	Agent* m_agent;
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;

	// TreeNode List
	std::vector<TreeNode*> m_treeNodeList;

	// Private Methods
	void m_buildTree();
};

#endif /* defined (__DECISION_TREE__) */