#include "stdafx.h"
#include "SplitManager.h"
#include "PrioQueue.h"

using namespace Entities;
using namespace CrossHandlers;
using namespace Resources;
using namespace CelestialMath;


SplitManager::SplitManager(unsigned int maxClock) : TravelManager(maxClock)
{

	offsetDist = getOffsetDist();

}

SplitManager::objNode SplitManager::shouldIntersect(RouteNodeObject* intersect, RouteNodeObject* from, RouteNodeObject* to, GameTravelObject* obj)
{

	unsigned int interToLocal = intersect->GetLocalId(to->GetId());
	Route* rte = intersect->GetRoute(interToLocal);

	float toDist = rte->GetDistance();

	float goalDistSqr = pow(toDist + offsetDist + obj->GetQueueLength() / 2, 2);
	float goalDistSqr2 = pow(toDist - (offsetDist + obj->GetQueueLength() / 2), 2);

	float objDist = VectorDot(obj->GetPosition() - (to->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));

	if (objDist > goalDistSqr)//Object is beyond the intersection
	{

		return objNode_ABOVE;

	}
	else if (objDist < goalDistSqr2)//Object is closer
	{

		return objNode_BELOW;

	}
	else
	{

		return objNode_ON;

	}
}

void SplitManager::handleSplit(RouteNodeObject* intersect, RouteNodeObject* lower, RouteNodeObject* upper)
{

	prio_queue objectList[objNode_NA];
	unsigned int localUpper = lower->GetLocalId(upper->GetId());
	Route::Direction upDownDir;
	Route* upDown = lower->GetRoute(localUpper, upDownDir);
	RouteNodeObject* goal = upDown->CanTravel(upDownDir) ? upper : lower;
	RouteNodeObject* from = lower == goal ? upper : lower;

	lower->RemoveRoute(upper->GetId());
	upper->RemoveRoute(lower->GetId());

	Vector3 lowerIntDir = intersect->GetPosition() - lower->GetPosition();
	float lID = sqrt(VectorDot(lowerIntDir));
	Vector3 IntersectUpDir = upper->GetPosition() - intersect->GetPosition();
	float iUD = sqrt(VectorDot(IntersectUpDir));

	Route* lowerInt = new Route(lID, lowerIntDir / lID);
	Route* IntUpper = new Route(iUD, IntersectUpDir / iUD);

	roads->Add(lowerInt);
	roads->Add(IntUpper);

	lower->AddRoute(lowerInt);
	upper->AddRoute(IntUpper);

	intersect->AddRoute(lowerInt);
	intersect->AddRoute(IntUpper);

	CelestialListNode<GameTravelObject*>* node = travelObjects->GetFirstNode();

	while (node != nullptr)
	{

		objNode inter = objNode_NA;
		GameTravelObject* obj = node->GetNodeObject();
		float dist = VectorDot(obj->GetPosition() - (goal->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));

		float objUpDist = VectorDot(obj->GetPosition() - upper->GetPosition());
		float objLowDist = VectorDot(obj->GetPosition() - lower->GetPosition());

		if (obj->GetNode() == from->GetId() &&
			obj->GetGoalNode() == goal->GetId())//Object is going on the path
		{

			inter = shouldIntersect(intersect, lower, upper, obj);

		}

		if (inter != objNode_NA)
		{

			prio_queue::nodeVal value;
			value.traveledDist = 0;
			value.targetDist = dist;
			value.index = obj->GetId();

			objectList[inter].push(value);

		}

		node = node->GetNext();

	}

	unsigned int interGoal = intersect->GetLocalId(goal->GetId());
	Route::Direction iGDir;
	Route* iGR = intersect->GetRoute(interGoal, iGDir);
	float iGDist = iGR->GetDistance();

	while (!objectList[objNode_BELOW].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_BELOW].top();
		objectList[objNode_BELOW].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);
		iGR->Travel(obj->GetId(), iGDir);
		obj->SetNode(intersect->GetId());

		if (obj->GetStatus() == TravelStatus_QUEUEING ||
			obj->GetStatus() == TravelStatus_WAITING)
		{

			iGR->Queue(obj->GetQueueLength(), 0);

		}
	}

	unsigned int fromInterLocal = from->GetLocalId(intersect->GetId());
	Route::Direction fIDir;
	Route* fIR = from->GetRoute(fromInterLocal, fIDir);

	while (!objectList[objNode_ON].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_ON].top();
		objectList[objNode_ON].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);

		float distSqrt = sqrt(val.targetDist);
		distSqrt += obj->GetQueueLength() / 2;

		if (distSqrt > iGDist + offsetDist)//The object pokes out from the node
		{

			float overDist = distSqrt - (iGDist + offsetDist);

			fIR->Queue(overDist, 0);
			fIR->Travel(obj->GetId(), fIDir);

			handlePrimedObject(obj);//Renavigate object
			obj->SetStatus(TravelStatus_WAITING);

		}
		else
		{

			intersect->SetObjId(obj->GetId());
			obj->SetNode(intersect->GetId());
			iGR->Travel(obj->GetId(), iGDir);

			if (obj->GetStatus() == TravelStatus_QUEUEING ||
				obj->GetStatus() == TravelStatus_WAITING)
			{

				iGR->Queue(obj->GetQueueLength(), 0);

			}
		}
	}

	while (!objectList[objNode_ABOVE].empty())
	{

		prio_queue::nodeVal val = objectList[objNode_ABOVE].top();
		objectList[objNode_ABOVE].pop();
		GameTravelObject* obj = (GameTravelObject*)gameObjects->GetValue(val.index);

		fIR->Travel(obj->GetId(), fIDir);

		handlePrimedObject(obj);//Renavigate all objects
		float dist = VectorDot(obj->GetPosition() - (intersect->GetPosition() + Vector3(0.0f, obj->GetScale().y / 2, 0.0f)));
		handleQueing(obj, fromInterLocal, fIR, sqrt(dist), 1);

	}
}
