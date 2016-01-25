#include "DXTextureResource.h"

namespace CrossHandlers
{
	///<summary>This class handles views seperatly from textures</summary>
	class DXTextureResourceViews : public DXTextureResource
	{

		public:
			DXTextureResourceViews(ID3D11Texture2D* texture,ID3D11Device1* card);
			virtual void Release();
			virtual ~DXTextureResourceViews();

	};
}