#pragma once

#include "IFileLoader.h"

namespace Resources
{

	class IFileTextureLoader : public IFileLoader
	{

	public:
		///<summary>Loads texture from a file</summary>
		///<param name='text'>[in]The name of the file containing the texture</param>
		///<returns>A pointer to the loaded texture</returns>
		virtual CrossHandlers::TextureResourceObject* LoadTexture(std::string file) = 0;

		virtual ~IFileTextureLoader(){}
	};
}