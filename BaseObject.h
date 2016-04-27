#pragma once
#include "Message.h"
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
			virtual unsigned int GetParentId() const;
			CrossHandlers::Message* GetKillMessage() const;
			virtual ~BaseObject()
			{
			}

		protected:
			void setLastUpdated(unsigned int time);
			CrossHandlers::Message* killMessage;

		private:
			unsigned int id;
			unsigned int lastUpdated;
	};
}