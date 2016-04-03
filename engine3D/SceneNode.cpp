#include "SceneNode.h"


SceneNode::SceneNode()
{
}


SceneNode::~SceneNode()
{
}

bool SceneNode::AddChild(SceneNode* child)
{
	while (parent != nullptr)
	{
		SceneNode* temp = parent->parent;
	}
	return false;
}

bool SceneNode::SetParent(SceneNode* parent)
{
	if (rootNode == false && parent != nullptr)
	{
		this->parent = parent;
		return true;
	}
	else if (rootNode == false && parent == nullptr)
	{
		std::cout << "Error: This node is not a parent, nullptr not applicable" << std::endl;
		return false;
	}
	else if (rootNode == true && parent != nullptr)
	{
		std::cout << "Error: This nose is a RootNode, it cant have a parent" << std::endl;
		return false;
	}
	else if(rootNode == true && parent == nullptr)
	{
		this->parent = nullptr;
	}
}