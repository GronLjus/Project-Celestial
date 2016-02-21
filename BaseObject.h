#pragma once
#include "Message.h"

namespace Resources
{

	class BaseObject
	{

		public:
			void SetId(unsigned int id);
			unsigned int GetId() const;
			virtual void Update(CrossHandlers::Message* mess) = 0;
			virtual ~BaseObject(){}

		private:
			unsigned int id;
	};
}