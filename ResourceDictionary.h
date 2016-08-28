#pragma once
#include "SerializableObject.h"
#include "CelestialSlicedList.h"
#include "Dictionary.h"

namespace Resources
{

	class ResourceDictionary
	{

		public:
			ResourceDictionary();
			Dictionary* Translate(ResourceDictionary* rd);
			void AddResource(unsigned int id, std::string name);
			void RemoveResource(unsigned int id);
			unsigned int FindId(std::string name) const;

			//Serializable Interface
			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			~ResourceDictionary();

		private:
			struct resourceId
			{

				std::string name;
				unsigned int id;

			};

			CrossHandlers::CelestialSlicedList<resourceId*>* resourceDictionary;

	};
}