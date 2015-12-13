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

			ViewObject(CelestialMath::Matrix matrix, unsigned int bufferFlips);
			void Update(CelestialMath::Matrix matrix);
			void IncrementInstances();
			void AddInstanceFragment(unsigned int mesh, unsigned int start, unsigned int length);
			void ResetInstances();
			CrossHandlers::Frustum* GetFrustum() const;
			CrossHandlers::CelestialStack<Fragment>* GetInstanceStack(unsigned int bufferFlip) const;
			~ViewObject();

		private:
			CrossHandlers::CelestialStack<Fragment>** instances;
			CrossHandlers::Frustum* localFrustum;
			unsigned int flips;
			unsigned int flip;
	};
}
