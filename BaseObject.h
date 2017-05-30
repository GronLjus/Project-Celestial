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
			unsigned int GetActiveId() const;
			void SetActiveId(unsigned int id);
			unsigned int GetId() const;
			virtual unsigned char* Update(CrossHandlers::Message* mess) = 0;
			unsigned int LastUpdated() const;
			virtual unsigned int GetTargetId() const;
			virtual unsigned int GetParentId() const;
			CrossHandlers::Message** GetKillMessage(unsigned int &kills) const;
			virtual ~BaseObject()
			{
				if (killMessages != nullptr) { 
					for (unsigned int i = 0; i < kills; i++) { 
						if (killMessages[i] != nullptr)
						{
							delete killMessages[i];
						}
					} 
					delete[] killMessages; }
			}

		protected:
			void setLastUpdated(unsigned int time);
			void addKillMessage(CrossHandlers::Message* mess);

		private:
			unsigned int activeId;
			unsigned int id;
			unsigned int lastUpdated;

			CrossHandlers::Message** killMessages;
			unsigned int kills;
			unsigned int maxKills;

			bool save;

	};
}