#pragma once
#include "Message.h"
#include <mutex>

namespace Resources
{

	class BaseObject
	{

		public:
			BaseObject();
			virtual void SetId(unsigned int id);
			unsigned int GetId() const;
			virtual void Update(CrossHandlers::Message* mess) = 0;
			unsigned int LastUpdated() const;
			virtual unsigned int GetTargetId() const;
			virtual unsigned int GetParentId() const;
			CrossHandlers::Message** GetKillMessage(unsigned int &kills) const;
			virtual ~BaseObject()
			{
				if (killMessages != nullptr) { 
					for (int i = 0; i < kills; i++) { 
						if (killMessages[i] != nullptr)
						{
							delete killMessages[i];
						}
					} 
					delete[] killMessages; }
			}

		protected:
			void setLastUpdated(unsigned int time);
			CrossHandlers::Message** killMessages;
			unsigned int kills;

		private:
			unsigned int id;
			unsigned int lastUpdated;
			bool save;

	};
}