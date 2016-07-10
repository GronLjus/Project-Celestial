#include "stdafx.h"
#include "GameTravelObject.h"

using namespace Resources;

GameTravelObject::GameTravelObject(BoundingBox* baseBox, BoundingSphere* baseSphere, unsigned int meshId) : GameObject(baseBox,baseSphere,meshId)
{

	lastTime = 0;
	currentNode = 0;
	goal = 0;
	goalAmounts = 128;
	goals = new unsigned int[goalAmounts];
	speed = 1.5f;

}

GameObjectType GameTravelObject::GetType() const
{

	return GameObjectType_TRAVELING;

}

unsigned int GameTravelObject::GetGoalNode() const
{

	return goals[goal];

}

unsigned int GameTravelObject::GetFinalGoalNode() const
{

	return finalGoal;

}

unsigned int GameTravelObject::GetFilter() const
{

	return filter;

}

float GameTravelObject::GetSpeed() const
{

	return speed;

}

unsigned int GameTravelObject::GetLastTime() const
{

	return lastTime;

}

unsigned int GameTravelObject::GetNode() const
{

	return currentNode;

}

void GameTravelObject::SetTravelSpeed(float cells)
{

	speed = cells;

}

void GameTravelObject::SetGoalNode(unsigned int* goals)
{

	memcpy(this->goals, goals, goalAmounts);
	goal = 0;

}

void GameTravelObject::SetFinalGoalNode(unsigned int goal)
{

	 finalGoal = goal;

}

void GameTravelObject::SetFilter(unsigned int filter)
{

	this->filter = filter;

}

void GameTravelObject::Time(unsigned int time)
{

	lastTime = time;

}

bool GameTravelObject::StepGoal()
{

	goal++;
	goal %= goalAmounts;
	return goal == 0;

}

void GameTravelObject::SetNode(unsigned int node)
{

	currentNode = node;

}

GameTravelObject::~GameTravelObject()
{

	delete[] goals;

}