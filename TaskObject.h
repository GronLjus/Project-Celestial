#pragma once
#include "BaseObject.h"
#include "SerializableObject.h"
#include "CelestialStack.h"
#include "TaskEnums.h"

namespace Resources
{

	class TaskObject : public BaseObject, public SerializableObject
	{

		public:
			TaskObject(unsigned int script,
				CrossHandlers::CelestialStack<int>* iParams,
				CrossHandlers::CelestialStack<float>* fParams,
				CrossHandlers::CelestialStack<std::string>* sParams);

			virtual char* Serialize(unsigned int &size);
			virtual char* Unserialize(char* data);

			CrossHandlers::CelestialStack<int>* GetIParams() const;
			CrossHandlers::CelestialStack<float>* GetFParams() const;
			CrossHandlers::CelestialStack<std::string>* GetSParams() const;

			unsigned int GetTime() const;
			unsigned int GetFireTime() const;
			unsigned int GetScript() const;
			TaskClass GetType() const;

			void Queue(TaskClass type, unsigned int fireTime);
			void SetTime(unsigned int time);
			void Kill();
			bool ShouldKill() const;

			virtual unsigned char* Update(CrossHandlers::Message* mess);

			virtual ~TaskObject();

		private:
			unsigned int script;
			CrossHandlers::CelestialStack<int>* iParams;
			CrossHandlers::CelestialStack<float>* fParams;
			CrossHandlers::CelestialStack<std::string>* sParams;

			unsigned int fireTime;
			unsigned int lastFired;

			TaskClass type;

			bool kill;

	};
}