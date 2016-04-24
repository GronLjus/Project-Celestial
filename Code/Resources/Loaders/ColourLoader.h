#pragma once
#include "FileLoaders/Interfaces/IFileMaterialLoader.h"
#include "FileLoaders/Interfaces/IMaterialLoader.h"

namespace Resources
{

	///<summary>This class handles the loading of colours</summary>
	class ColourLoader : public IMaterialLoader
	{
	public:

		virtual MeshObject::Material* LoadMaterial(int r, int g, int b, std::string name, float df, float af);
		virtual CrossHandlers::TextureResourceObject* LoadTextureFromColour(int* colours);
		virtual void Init(Graphics::CardHandler* &card);
		virtual ~ColourLoader();

	private:

		///<summary>The underlying device</summary>
		Graphics::CardHandler* card;

	};
}