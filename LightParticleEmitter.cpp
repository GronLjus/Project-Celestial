#include "stdafx.h"
#include "LightParticleEmitter.h"
#include "CelestialMath.h"
#include <time.h>

using namespace Resources;
using namespace CrossHandlers;
using namespace CelestialMath;

LightParticleEmitter::LightParticleEmitter(BaseParticle base, bool allAtOnce, TextureResourceObject* inputTexture) : CommonParticle(base, inputTexture)
{

	totalSize = base.size * 12;

	begins = new int[totalSize];

	for (int i = 0; i<totalSize; i++)
	{

		begins[i] = i;

	}

	if(!allAtOnce)
	{
	
		selectableSize = totalSize;

	}
	else
	{
		
		selectableSize = 1;

	}

	angle = (float)(CELESTIAL_PI / base.size);
	type = ParticleSystem_Light;

}

Particle LightParticleEmitter::GetParticle(int n)
{

	srand(time(nullptr));
	n-=1;
	Vector3 pos = Vector3(0,0,0);
	Vector3 velocity = Vector3(0,0,0);
	Vector3 size = baseSize;
	int startTime = 0;
	int fade = 10000;

	if(n >= 0)
	{

		float angleX = 0;
		float angleY = 0;
		float angleZ = 0;

		fade = 100;
		int circle = (n%2)*-1;

		if(circle == 0)
		{

			circle = 1;

		}

		n /= 2;
		int extreme = n%6;
		int number = n/6;

		if(extreme==0 || extreme == 1)
		{

			pos = Vector3(0,1,0);
			
			if(extreme == 1)
			{
		
				pos = Vector3(0,-1,0);

			}

			angleY = CELESTIAL_PI*0.25f*circle;
			angleZ = angle*number;
			
		}
		else if(extreme == 2 || extreme == 3)
		{

			pos = Vector3(1,0,0);

			if(extreme == 3)
			{

				pos = Vector3(-1,0,0);

			}

			angleX = CELESTIAL_PI*0.25f*circle;
			angleY = angle*number;

		}
		else if(extreme == 4 || extreme == 5)
		{

			pos = Vector3(0,0,1);

			if(extreme == 5)
			{

				pos = Vector3(0,0,-1);

			}

			angleZ = CELESTIAL_PI*0.25f*circle;
			angleX = angle*number;

		}
		
		Matrix xMat = MatrixRotationX(angleX);
		Matrix yMat = MatrixRotationY(angleY);
		Matrix zMat = MatrixRotationZ(angleZ);
		
		Matrix rotMat;

		if(extreme==0 || extreme == 1)
		{

			rotMat = MatrixMultiply(zMat, yMat);
			
		}
		else if(extreme == 2 || extreme == 3)
		{
			
			rotMat = MatrixMultiply(yMat, xMat);

		}
		else if(extreme == 4 || extreme == 5)
		{
			
			rotMat = MatrixMultiply(xMat, zMat);

		}

		Vector4 newPos = VectorTransform(Vector4(pos,1), rotMat);
		Vector3 particlePos = Vector3(newPos.x,newPos.y,newPos.z);
		velocity = particlePos*baseVelocity;
		size = baseSize;

		if(sizeFactor > 0)
		{

			size *= (100+((rand()%(sizeFactor*2))-sizeFactor))*0.01f;

		}

		if(particlePos > 0 && baseVelocity > 0)
		{
		
			velocity = particlePos*(baseVelocity*(100+((rand()%(velFactor*2))-velFactor))*0.01f);

		}

		int start = rand()%selectableSize;
		int sTime = begins[start];

		if(selectableSize > 1)
		{

			begins[start] = begins[selectableSize-1];
			selectableSize--;

		}

		startTime = sTime*(particleLife/(this->totalSize));
		pos = particlePos;
		pos = Vector3(0, 0, 0);

	}

	return Particle(pos,velocity,size,fade,particleLife,-1.0f,startTime);

}

int LightParticleEmitter::GetNrOfParticles() const
{

	return totalSize;

}

LightParticleEmitter::~LightParticleEmitter()
{

	delete[] begins;

}