#pragma once
#include "TaskObject.h"
#include "MessageBuffer.h"

namespace Tasking
{

	class ITaskManager
	{

		private:
			CrossHandlers::MessageBuffer* mBuffer;
			unsigned int maxMess;
			unsigned int currentMess;
			void addParam(unsigned int script, int value, unsigned int time);
			void addParam(unsigned int script, float value, unsigned int time);
			void addParam(unsigned int script, std::string value, unsigned int time);
			void runScript(unsigned int script, unsigned int time);

		protected:
			void trigger(Resources::TaskObject* object, unsigned int time);

		public:

			void Init(CrossHandlers::MessageBuffer* mBuffer);

			virtual void AddTask(Resources::TaskObject* object, unsigned int fireTime) = 0;
			virtual void RemoveTask(Resources::TaskObject* object) = 0;
			virtual void Update(unsigned int time) = 0;
			virtual ~ITaskManager() {};

	};
}