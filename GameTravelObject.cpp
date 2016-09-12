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
	qLength = 1.0f;
	lastQueue = 0.0f;
	qUTime = 0;

}

GameTravelObject::GameTravelObject() : GameObject()
{

	lastTime = 0;
	currentNode = 0;
	goal = 0;
	goalAmounts = 128;
	goals = new unsigned int[goalAmounts];
	speed = 1.5f;
	qLength = 1.0f;
	lastQueue = 0.0f;
	qUTime = 0;

}

char* GameTravelObject::Unserialize(char* data)
{

	memcpy(&currentNode, data, 4);
	memcpy(&finalGoal, &data[4], 4);
	goal = data[8];
	memcpy(&filter, &data[9], 4);
	memcpy(&speed, &data[13], 4);
	goalAmounts = data[17];
	unsigned int goalPlace = goalAmounts*sizeof(unsigned int);
	memcpy(goals, &data[18], goalPlace);

	if (data[18 + goalPlace] == SerializableType_GAMEOBJECTSCENERY)
	{

		return GameObject::Unserialize(&data[18 + goalPlace + 1]);

	}

	return nullptr;

}

char* GameTravelObject::Serialize(unsigned int &size)
{

	unsigned int subSize = 0;
	char* subSerial = GameObject::Serialize(subSize);
	unsigned int goalSize = goalAmounts * sizeof(unsigned int);
	char* goals = new char[goalSize];

	for (unsigned int i = 0; i < goalSize; i+=4)
	{

		goals[i] = goals[i] << 0;
		goals[i+1] = goals[i] << 8;
		goals[i+2] = goals[i] << 16;
		goals[i+3] = goals[i] << 20;

	}

	unsigned int standard = 19;
	size = standard + goalSize + subSize;
	
	char* byteVal = new char[size];

	byteVal[0] = SerializableType_GAMEOBJECTTRAVEL;

	byteVal[1] = currentNode << 0;
	byteVal[2] = currentNode << 8;
	byteVal[3] = currentNode << 16;
	byteVal[4] = currentNode << 20;

	byteVal[5] = finalGoal << 0;
	byteVal[6] = finalGoal << 8;
	byteVal[7] = finalGoal << 16;
	byteVal[8] = finalGoal << 20;

	byteVal[9] = goal;

	byteVal[10] = filter << 0;
	byteVal[11] = filter << 8;
	byteVal[12] = filter << 16;
	byteVal[13] = filter << 20;

	memcpy(&byteVal[14], &speed, 4);
	byteVal[18] = goalAmounts;

	memcpy(&byteVal[standard], goals, goalSize);
	memcpy(&byteVal[standard+goalSize], subSerial, subSize);
	delete[] subSerial;
	delete[] goals;

	status = TravelStatus_READY;

	return byteVal;

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

TravelStatus GameTravelObject::GetStatus() const
{

	return status;

}

float GameTravelObject::GetQueueLength() const
{

	return qLength;

}

float GameTravelObject::LastQueue() const
{

	return lastQueue;

}

unsigned int GameTravelObject::GetQTime() const
{

	return qUTime;

}

void GameTravelObject::SetStatus(TravelStatus status)
{

	this->status = status;

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

void GameTravelObject::SetQueueLength(float qLength)
{
	
	this->qLength = qLength;

}

void GameTravelObject::SetLastQueue(float lastQ)
{

	lastQueue = lastQ;

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

void GameTravelObject::QueueTime(unsigned int time)
{

	qUTime = time;

}

unsigned int GameTravelObject::PeekNextGoal(bool &reCalc) const
{

	reCalc = (goal + 1) % goalAmounts == 0;
	return  !reCalc ? goals[goal + 1] : 0;

}
void GameTravelObject::SetNode(unsigned int node)
{

	currentNode = node;

}

GameTravelObject::~GameTravelObject()
{

	delete[] goals;

}