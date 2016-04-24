#pragma once
#include "../../CrossHandler-Tools/Messaging/Message.h"
#include <mutex>

namespace Resources
{

	class BaseObject
	{

		public:
			virtual void SetId(unsigned int id);
			unsigned int GetId() const;
			virtual void Update(CrossHandlers::Message* mess) = 0;
			unsigned int LastUpdated() const;
			virtual unsigned int GetTargetId() const;
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