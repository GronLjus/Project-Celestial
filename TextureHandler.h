#pragma once
#include "IFileTextureLoader.h"
#include "IMaterialLoader.h"

namespace Resources
{

	///<summary>This class handles the loading of textures</summary>
	class TextureHandler
	{

	private:
		///<summary>The amount of fileloaders this manages for materials</summary>
		int fileMatLoaders;

		///<summary>Gets the extension of a path</summary>
		///<param name='file'>[in]The name of the file to get</param>
		std::string getExtension(std::string file);
		///<summary>An array of pointer to the loaders used to load material from files</summary>
		IFileTextureLoader** matFileLoaders;
		///<summary>An array of pointer to the loaders used to load materials</summary>
		IMaterialLoader* matColourLoader;

	public:

		TextureHandler();
		virtual void Init(Graphics::CardHandler* &card);

		virtual CrossHandlers::TextureResourceObject* LoadTextureFromFile(std::string file);
		virtual CrossHandlers::TextureResourceObject* LoadTextureFromColour(int* colours);

		virtual ~TextureHandler();
	};
}