#pragma once
#include "../../Graphics/GraphicAPI.h"

namespace CrossHandlers
{

	///<summary>This class contains and encapsulates the uses of a DirectX-Buffer, it can also be used as a linked list</summary>
	class DXBufferObject
	{

		private:
			///<summary>A pointer to the buffer used by this object</summary>
			ID3D10Buffer* internalBuffer;
			///<summary>A pointer to the array mapped to the buffer</summary>
			void** mapped;
			///<summary>A pointer to the next object to be used in a linked list</summary>
			DXBufferObject* next;
			///<summary>The number of elements in the buffer</summary>
			int size;
			///<summary>The companybuffer</summary>
			DXBufferObject* companyBuffer;

		public:
			DXBufferObject();
			///<summary>Sets the buffer of the object, also releases the buffer currently used</summary>
			///<param name='buffer'>[in]A pointer to the new buffer to use</param>
			void SetBuffer(ID3D10Buffer* buffer);
			///<summary>Gets the buffer used by the object</summary>
			///<returns>A pointer to the buffer used by the object, or nullptr</returns>
			ID3D10Buffer* GetBuffer();
			///<summary>Releases the buffer used by this object</summary>
			void Release();
			///<summary>Sets the internal pointer to the mapped pointer</summary>
			///<param name='mapped'>[in]A pointer to the array that's mapped to the buffer</param>
			void Map(void** mapped);
			///<summary>Removes the mapped pointer from the object</summary>
			void UnMap();
			///<summary>Gets the mapped array of the buffer</summary>
			///<returns>A pointer to the mapped array or nullptr</returns>
			void** GetMappedPointer();
			///<summary>Sets the next object in the list</summary>
			///<param name='next'>[in]A pointer to the next dxbuffer to be used</param>
			void SetNext(DXBufferObject* next);
			///<summary>Gets the next object in a linked list</summary>
			///<returns> A pointer to the next object the linkedlist</returns>
			DXBufferObject* GetNext();
			///<summary>Sets the size of the buffer</summary>
			///<param name='size'>[in]The new amount of objects in the buffer</param>
			void SetSize(int size);
			///<summary>Gets the amount of elements in the buffer</summary>
			///<returns>The number of objects in the buffer</returns>
			int GetSize();
			///<summary>Sets the id of the buffer to use as friend</summary>
			///<param name='buffer'>[in]A pointer to the buffer it uses as company</param>
			void SetCompany(DXBufferObject* buffer);
			///<summary>Gets the companybuffer</summary>
			///<returns>A pointer to the companybuffer</returns>
			DXBufferObject* GetCompany();
			~DXBufferObject();

	};
}