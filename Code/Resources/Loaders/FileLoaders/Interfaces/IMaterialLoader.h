#pragma once
#include "MeshObject.h"
#include <string>

namespace Resources
{

	class IMaterialLoader
	{
		
		public:
			///<summary>Loads material with a colour</summary>
			///<param name='r'>[in]The red of the colour</param>
			///<param name='r'>[in]The green of the colour</param>
			///<param name='r'>[in]The blue of the colour</param>
			///<param name='name'>[in]The name of the material</param>
			///<param name='df'>[in]The diffusefactor of the colour</param>
			///<param name='af'>[in]The ambiantfactor of the colour</param>
			///<returns>A pointer to the material loaded</returns>
			virtual MeshObject::Material* LoadMaterial(int r, int g, int b,std::string name,float df,float af) = 0;
			///<summary>Create a texture from a color</summary>
			///<param name='colours'>[in]A pointer to an array of values, must be of size 3 or more</param>
			///<returns>A pointer to the array of materials loaded</returns>
			virtual CrossHandlers::TextureResourceObject* LoadTextureFromColour(int* colours) = 0;
			///<summary>Initilizes the loader</summary>
			///<param name='card'>[in]The device to base the loader on</param>
			virtual void Init(Graphics::CardHandler* &card) = 0;

			virtual ~IMaterialLoader(){}
	};
}