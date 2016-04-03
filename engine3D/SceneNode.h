#pragma once

#include <iostream>
#include <vector>
#include <glew.h>

#include <glfw3.h>

#include <glm.hpp>


class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	bool AddChild(SceneNode* child);
	bool SetParent(SceneNode* parent);

private:
	std::vector<SceneNode*>children;
	SceneNode* parent;
	bool rootNode;



};



