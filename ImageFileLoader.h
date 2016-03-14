#pragma once

#include <wincodec.h>
#include <wincodecsdk.h>

#include "IFileTextureLoader.h"
#include "IFileMaterialLoader.h"

namespace Resources
{

	class ImageFileLoader : public IFileTextureLoader, public IFileMaterialLoader
	{

		public:
			virtual MeshObject::Material** LoadMaterial(std::string file, int &size);
			virtual CrossHandlers::TextureResourceObject* LoadTexture(std::string file);
			virtual CrossHandlers::ImageResourceObject** LoadBitMaps(std::string file,unsigned int &size);
			virtual std::string* Extension() const;
			virtual int GetNrExtensions() const;
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			virtual ~ImageFileLoader();

		private:
			unsigned char* getImage(std::string file, unsigned int &width, unsigned int &height) const;
			///<summary>The underlying device</summary>
			Graphics::CardHandler* card;
			///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
			std::string* extensions;
			///<summary>The number of extensions</summary>
			int extens;
			///<summary>The WIC factory to use when loading images</summary>
			IWICImagingFactory* imFac;

	};
}