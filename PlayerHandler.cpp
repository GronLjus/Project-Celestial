#include "stdafx.h"
#include "PlayerHandler.h"
#include "CelestialMath.h"

using namespace Logic;
using namespace Input;
using namespace Resources;
using namespace CelestialMath;
using namespace CrossHandlers;

PlayerHandler::PlayerHandler()
{

	speeds = new float[3];
	speeds[0] = 0.25f;
	speeds[1] = 0;
	speeds[2] = 0.5f;
	mods = new float[3];
	mods[0] = 1;
	mods[1] = 0;
	mods[2] = 1;
	terrain = nullptr;
	boundByGround = true;

}

void PlayerHandler::Init(ResourceObject* ro,ILogicCamera* cam,InputHandler* iH)
{

	playerObject = ro;
	this->cam = cam;
	this->iH = iH;

}

void PlayerHandler::Update()
{

	/*float* movement = new float[3];
	int* moves = iH->GetMovement();
	float* pos = iH->GetMousePos();
	float* oldMouse = iH->GetLastMousePos();
	float xMove = pos[0]-oldMouse[0];
	float yMove = -(pos[1]-oldMouse[1]);
	
	cam->Lock();
	Vector3 side = cam->GetSide(); 
	Vector3 up = cam->GetUp();
	Vector3 direction = cam->GetDirection();
	cam->Unlock();

	if(iH->HasMouseMoved())
	{


		float xAngle = atan(yMove);
		float yAngle = atan(xMove);
		
		Matrix rotationXZ = MatrixRotationAxis(Vector3(side),xAngle);
		Matrix rotationY = MatrixRotationY(yAngle);
		
		Matrix rotation = MatrixMultiply(rotationXZ,rotationY);
		Vector4 temp = VectorTransform(Vector4(side,1),rotationY);
		side.x = temp.x;
		side.y = temp.y;
		side.z = temp.z;
		temp = VectorTransform(Vector4(up, 1), rotation);
		up.x = temp.x;
		up.y = temp.y;
		up.z = temp.z;
		temp = VectorTransform(Vector4(direction, 1), rotation);
		direction.x = temp.x;
		direction.y = temp.y;
		direction.z = temp.z;
		cam->Rotate(rotationXZ,rotationY);
		
	}

	float* sid = new float[3];
	sid[0] = moves[0]*side.x*speeds[0];
	sid[1] = moves[0]*side.y*speeds[0];;
	sid[2] = moves[0]*side.z*speeds[0];;

	float* u = new float[3];
	u[0] = moves[1]*up.x*speeds[1];
	u[1] = moves[1]*up.y*speeds[1];
	u[2] = moves[1]*up.z*speeds[1];

	float* dir = new float[3];
	dir[0] = moves[2]*direction.x*speeds[2];
	dir[1] = moves[2]*direction.y*speeds[2];
	dir[2] = moves[2]*direction.z*speeds[2];

	bool willMove = false;

	for(int i=0;i<3;i++)
	{
		
		movement[i] = (sid[i]+u[i]+dir[i])*mods[i];
		willMove = willMove || movement[i] != 0;

	}

	if(willMove)
	{
	
		Vector3 move = Vector3(movement[0],movement[1],movement[2]);
		cam->Move(move);

		if(terrain != nullptr && boundByGround)
		{

			Vector3 camPos = cam->GetPosition();
			Vector3 newMoveMent = Vector3(0,(terrain->GetHeightAt(camPos.x,camPos.z)-camPos.y)+5,0);
			cam->Move(newMoveMent);

		}
	}

	delete[] dir;
	delete[] sid;
	delete[] u;
	delete[] movement;
	delete[] moves;*/

}

void PlayerHandler::ToggleFreeFlight(bool enabled)
{

	boundByGround = !enabled;

	if(boundByGround)
	{
		
		speeds[0] = 0.25f;
		speeds[1] = 0;
		speeds[2] = 0.5f;
		
		mods[0] = 1;
		mods[1] = 0;
		mods[2] = 1;

	}
	else
	{
		
		speeds[0] = 0.5f;
		speeds[1] = 0.5f;
		speeds[2] = 0.5f;
		
		mods[0] = 1;
		mods[1] = 1;
		mods[2] = 1;

	}
}

void PlayerHandler::SetTerrain(TerrainObject* terrain)
{

	this->terrain = terrain;

}

PlayerHandler::~PlayerHandler()
{

	delete[] speeds;
	delete[] mods;

}