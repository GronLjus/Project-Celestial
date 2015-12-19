#pragma once
#include "Frustum.h"
#include "BufferObj.h"
#include "BufferPoints.h"
#include "CelestialStack.h"

namespace Entities
{

	class ViewObject
	{

		public:
			struct Fragment
			{
				unsigned int mesh;
				unsigned int start;
				unsigned int length;

				Fragment(){ mesh = 0; start = 0; length = 0; }
				Fragment(unsigned int mesh, unsigned int start, unsigned int length) : mesh(mesh), start(start), length(length){}

			};

			struct ViewPort
			{
				unsigned int height;
				unsigned int width;
				float maxDepth;
				float minDepth;
				unsigned int topX;
				unsigned int topY;

			};

			ViewObject(CelestialMath::Vector3 pos,CelestialMath::Vector3 sidePoint, CelestialMath::Vector3 lookAtPoint, CelestialMath::Vector3 up, float fov, unsigned int bufferFlips, ViewPort port);
			void Update(CelestialMath::Matrix transform);
			void IncrementInstances();
			void AddInstanceFragment(unsigned int mesh, unsigned int start, unsigned int length);
			void ResetInstances();
			CrossHandlers::Frustum* GetFrustum() const;
			CrossHandlers::CelestialStack<Fragment>* GetInstanceStack(unsigned int bufferFlip) const;

			unsigned int GetFlip() const;
			CelestialMath::Matrix GetView(unsigned int flip) const;
			CelestialMath::Matrix GetProjection(unsigned int flip) const;
			CelestialMath::Matrix GetViewProjection(unsigned int flip) const;
			CelestialMath::Matrix GetInverseViewProjection(unsigned int flip) const;

			CelestialMath::Vector3 GetPosition(unsigned int flip) const;
			float GetFov() const;

			ViewPort GetPort() const;
			~ViewObject();

		private:
			CrossHandlers::CelestialStack<Fragment>** instances;
			CrossHandlers::Frustum* localFrustum;
			unsigned int flips;
			unsigned int flip;
			ViewPort vp;

			Entities::ViewObject* theView;

			CelestialMath::Matrix* views;
			CelestialMath::Matrix* projections;
			CelestialMath::Matrix* viewProjections;
			CelestialMath::Matrix* invViewProjections;

			CelestialMath::Vector3 sidePoint;
			CelestialMath::Vector3 lookAtPoint;
			CelestialMath::Vector3 up;
			CelestialMath::Vector3* pos;

			float fov;
	};
}
