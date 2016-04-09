#pragma once
#include "DXTextureResource.h"

namespace Graphics
{

	enum MeshType{ MeshType_SOLID, MeshType_WF, MeshType_NA};

	class GraphicalMesh
	{

		public:
			GraphicalMesh();
			GraphicalMesh(CrossHandlers::DXTextureResource* AmbTexture,CrossHandlers::DXTextureResource* DiffTexture,CrossHandlers::DXTextureResource* NormTexture, unsigned int meshIndStart, unsigned int meshIndLength, MeshType type);
			CrossHandlers::DXTextureResource* GetAmbientTexture() const;
			CrossHandlers::DXTextureResource* GetDiffuseTexture() const;
			CrossHandlers::DXTextureResource* GetNormalTexture() const;

			unsigned int GetIndexStart() const;
			unsigned int GetIndexLength() const;
			MeshType GetMeshType() const;
			~GraphicalMesh();

		private:
			CrossHandlers::DXTextureResource* AmbTexture;
			CrossHandlers::DXTextureResource* DiffTexture;
			CrossHandlers::DXTextureResource* NormTexture;

			unsigned int meshIndStart;
			unsigned int meshIndLength;
			MeshType type;

	};
}