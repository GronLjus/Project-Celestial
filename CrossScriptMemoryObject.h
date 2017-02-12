#pragma once
#include "BaseObject.h"
#include <string>

namespace Resources
{

	class CrossScriptMemoryObject : public BaseObject
	{

		private:
			struct MemObject
			{
				MemObject(){ value = nullptr; }
				std::string name;
				unsigned char* value;
				unsigned int valLength;
				unsigned int totalLength;

				friend bool operator<(const MemObject& mem1, const MemObject& mem2)
				{

					return mem1.name.compare(mem2.name) < 0;

				}
				~MemObject(){ if (value != nullptr){ delete[] value; } }

			};

			MemObject* objects;
			unsigned int totalObjects;
			unsigned int maxObjects;

			unsigned int getIndex(std::string val, unsigned int start, unsigned int end);
			void addMem(MemObject object, unsigned int index);

		public:
			CrossScriptMemoryObject();
			virtual unsigned char* Update(CrossHandlers::Message* mess) { return nullptr; };
			void SetMem(std::string name, unsigned char* value, unsigned int valLength);
			unsigned char* GetMem(std::string name, unsigned int &length);
			~CrossScriptMemoryObject();

	};
}