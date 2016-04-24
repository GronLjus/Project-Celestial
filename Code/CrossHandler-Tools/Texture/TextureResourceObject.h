#pragma once
#include "DX/DXTextureResource.h"

namespace CrossHandlers
{
	///<summary>Contains the data needed to handle textures,can also be used as a linkedlist</summary>
	class TextureResourceObject
	{

		public:
			TextureResourceObject();
			///<summary>Gets the resource as DX</summary>
			///<returns>A pointer to the underlying object</returns>
			DXTextureResource* GetDXT();
			///<summary>Sets the resource as DX</summary>
			///<param name='dxt'>[in]A pointer to the object to set as</param>
			void SetDXT(DXTextureResource* dxt);
			///<summary>Gets the next object in the list</summary>
			///<returns>A pointer to the next object in the list, or nullptr if last</returns>
			TextureResourceObject* GetNext();
			///<summary>Sets the next pointer in the list</summary>
			///<param name='next'>[in]A pointer to the object to be the next</param>
			void SetNext(TextureResourceObject* next);
			~TextureResourceObject();

		private:
			///<summary>A pointer to the internal shaderresource</summary>
			DXTextureResource* dxt;
			///<summary>A pointer to the next object in the linked list, or nullptr</summary>
			TextureResourceObject* next;

	};
}