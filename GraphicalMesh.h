#pragma once
#include "DXTextureResource.h"

namespace Graphics
{

	class GraphicalMesh
	{

		public:
			GraphicalMesh();
			GraphicalMesh(CrossHandlers::DXTextureResource* AmbTexture,CrossHandlers::DXTextureResource* DiffTexture,CrossHandlers::DXTextureResource* NormTexture, unsigned int meshIndStart, unsigned int meshIndLength);
			CrossHandlers::DXTextureResource* GetAmbientTexture() const;
			CrossHandlers::DXTextureResource* GetDiffuseTexture() const;
			CrossHandlers::DXTextureResource* GetNormalTexture() const;

			unsigned int GetIndexStart() const;
			unsigned int GetIndexLength() const;
			~GraphicalMesh();

		private:
			CrossHandlers::DXTextureResource* AmbTexture;
			CrossHandlers::DXTextureResource* DiffTexture;
			CrossHandlers::DXTextureResource* NormTexture;

			unsigned int meshIndStart;
			unsigned int meshIndLength;

	};
}