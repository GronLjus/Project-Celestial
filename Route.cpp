#include "stdafx.h"
#include "Route.h"

using namespace Entities;

Route::Route() : Route(0.0f, CelestialMath::Vector3())
{

	travelDir = Direction_NA;
	id = 0;

}

Route::Route(float dist, CelestialMath::Vector3 direction)
{

	id = 0;
	upnode = 0;
	downNode = 0;

	travelDir = Direction_NA;
	this->dist = dist;
	this->direction = direction;
	qLength = 0.0f;

	lastObject = 0;
	qDiff = 0.0f;
	qTime = 0;

	lastDist = 0.0f;

}

char* Route::Serialize(unsigned int &size)
{

	size = sizeof(float) * 5 + sizeof(unsigned int) * 2 + 2;

	unsigned int offset = 0;
	char* byteVal = new char[size];
	byteVal[offset] = Resources::SerializableType_ROUTE;
	offset++;
	byteVal[offset] = travelDir;
	offset++;
	memcpy(&byteVal[offset], &qLength, sizeof(float));
	offset += sizeof(float);
	memcpy(&byteVal[offset], &dist, sizeof(float));
	offset += sizeof(float);
	memcpy(&byteVal[offset], &direction.x, sizeof(float));
	offset += sizeof(float);
	memcpy(&byteVal[offset], &direction.y, sizeof(float));
	offset += sizeof(float);
	memcpy(&byteVal[offset], &direction.z, sizeof(float));
	offset += sizeof(unsigned int);

	memcpy(&byteVal[offset], &upnode, sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&byteVal[offset], &downNode, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	return byteVal;

}

char* Route::Unserialize(char* data)
{

	unsigned int offset = 0;
	travelDir = Route::Direction(data[0]);
	offset++;
	memcpy(&qLength, &data[offset], sizeof(float));
	offset += sizeof(float);
	memcpy(&dist, &data[offset], sizeof(float));
	offset += sizeof(float);
	memcpy(&direction.x, &data[offset], sizeof(float));
	offset += sizeof(float);
	memcpy(&direction.y, &data[offset], sizeof(float));
	offset += sizeof(float);
	memcpy(&direction.z, &data[offset], sizeof(float));
	offset += sizeof(unsigned int);

	memcpy(&upnode, &data[offset], sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&downNode, &data[offset], sizeof(unsigned int));
	offset += sizeof(unsigned int);

	return nullptr;

}

Route::Direction Route::GetDirection() const
{

	return travelDir;

}

unsigned int Route::GetNode(Route::Direction dir) const
{

	if (dir == Route::Direction_DOWN)
	{

		return downNode;

	}
	else if (dir == Route::Direction_UP)
	{

		return upnode;

	}

	return 0;

}

float Route::GetQuelength() const
{

	return qLength;

}

unsigned int Route::GetQTime() const
{

	return qTime;

}

float Route::GetQDiff() const
{

	return qDiff;

}

float Route::GetDistance() const
{

	return dist;

}

unsigned int Route::GetId() const
{

	return id;

}

bool Route::CanTravel(Route::Direction dir) const
{

	return this->travelDir == Route::Direction_NA || this->travelDir == dir;

}

unsigned int Route::GetLastObj() const
{

	return lastObject;

}

void Route::SetId(unsigned int id)
{

	this->id = id;

}

void Route::SetNode(unsigned int node, Direction dir)
{

	if (dir == Route::Direction_DOWN)
	{

		downNode = node;

	}
	else if (dir == Route::Direction_UP)
	{

		upnode = node;

	}
}

void Route::Queue(float length, unsigned int time)
{

	if (length < 0.0f)
	{

		qDiff = -length;
		qTime = time;

	}

	qLength += length;
	qLength = max(qLength, 0.0f);

}

void Route::Travel(unsigned int obj, Direction dir)
{

	lastObject = obj;
	travelDir = dir;

}

void Route::Travel(unsigned int obj, float dist, Direction dir)
{

	if (dist > lastDist)
	{

		lastObject = obj;
		lastDist = dist;
		travelDir = dir;

	}
}

void Route::ResetLastDist()
{

	lastDist = 0.0f;

}

void Route::TravelDone(unsigned int obj)
{

	if (obj == lastObject)
	{

		travelDir = Route::Direction_NA;

	}
}

Route::~Route()
{

}