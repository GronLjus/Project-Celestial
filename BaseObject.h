#pragma once
#include "Message.h"
#include <mutex>

namespace Resources
{

	class BaseObject
	{

		public:
			void SetId(unsigned int id);
			unsigned int GetId() const;
			virtual void Update(CrossHandlers::Message* mess) = 0;
			unsigned int LastUpdated() const;
			virtual ~BaseObject()
			{
			}

		protected:
			void setLastUpdated(unsigned int time);

		private:
			unsigned int id;
			unsigned int lastUpdated;
	};
}