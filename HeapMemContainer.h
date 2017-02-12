#pragma once
#include "BaseObject.h"
#include "HeapMemory.h"

namespace Resources
{

	class HeapMemContainer : public BaseObject
	{

		private:
			Logic::HeapMemory* heap;

		public:
			HeapMemContainer(Logic::HeapMemory* heap);
			Logic::HeapMemory* GetHeap() const;
			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; }
			virtual ~HeapMemContainer();

	};
}
