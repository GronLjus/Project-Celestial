#pragma once
#include "CelestialVector.h"
#include "BaseObject.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace Resources
{

	class CelMesh : public BaseObject
	{
	
		public: 
			struct Vertex
			{

				CelestialMath::Vector3 pos;
				CelestialMath::Vector2 uv;
				CelestialMath::Vector3 norm;

			};
			
			CelMesh(unsigned int indices, unsigned int* indexBuffer, unsigned int vertices, Vertex* vertexBuffer, bool wireframe, CelestialMath::Vector3 size);
			virtual void Update(CrossHandlers::Message* mess);
			unsigned int* GetIndexBuffer(unsigned int &indices) const;
			Vertex* GetVertexBuffer(unsigned int &vertices) const;
			CrossHandlers::IBounding* GetBoundingObjectCopy(CrossHandlers::Shape shape) const;
			bool IsWireframe() const;
			CelestialMath::Vector3 GetScale() const;
			virtual ~CelMesh();

		private:
			unsigned int indices;
			unsigned int* indexBuffer;

			unsigned int vertices;
			Vertex* vertexBuffer;
			bool wireFrame;

			CelestialMath::Vector3 size;
			CrossHandlers::BoundingBox* box;
			CrossHandlers::BoundingSphere* sphere;

	};
}
