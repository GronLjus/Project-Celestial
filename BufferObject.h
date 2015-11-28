#pragma once
#include "dxbufferobject.h"

namespace CrossHandlers
{
	
	enum BufferTypes{BufferTypes_VERTEX,BufferTypes_INDEX,BufferTypes_INSTANCE,BufferTypes_COUNT};

	///<summary>This class abtracts the usage of the graphics API, it contains a number of underlying buffers</summary>
	class BufferObject
	{

		private:
			///<summary>A pointer to the underlying DXBuffercontainer</summary>
			DXBufferObject* fDxBuffer;
			DXBufferObject* lDxBuffer;
			///<summary>A pointer to the active dxBuffer</summary>
			DXBufferObject* aDxBuffer;

		public:
			BufferObject();
			///<summary>Sets the first underlying dxbuffer the object uses</summary>
			///<param name='object'>[in]A pointer to the object that should be used</param>
			void SetDXBuffer(DXBufferObject* object);
			///<summary>Adds the object to the last of the list, or first if the list is empty</summary>
			///<param name='object'>[in]A pointer to the object that should be used</param>
			void AddDxBuffer(DXBufferObject* object);
			///<summary>Sets the active dxBuffer to use</summary>
			///<param name='object'>[in]A pointer to the dxbuffer to use as active</param>
			void SetActiveDxBuffer(DXBufferObject* object);
			///<summary>Gets a pointer to the first underlying dxbuffer</summary>
			///<returns>A pointer to the first dxbuffer in the list</returns>
			DXBufferObject* GetFirstDXBuffer();
			///<summary>Gets a pointer to the last underlying dxbuffer</summary>
			///<returns>A pointer the last dxBuffer in the list</returns>
			DXBufferObject* GetLastDXBuffer();
			///<summary>Gets the active dxBuffer</summary>
			///<returns>A pointer to the active dxBuffer</returns>
			DXBufferObject* GetActiveDXBuffer();
			~BufferObject();
	};
}