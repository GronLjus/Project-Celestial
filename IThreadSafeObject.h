#pragma once

namespace CrossHandlers
{

	///<summary>This interface makes an object threadsafe</summary>
	class IThreadSafeObject
	{

		public:
			///<summary>Locks the object so it doesn't change</summary>
			virtual bool Lock() = 0;
			///<summary>Unlocks the object and applies all changes made while locked</summary>
			virtual void Unlock() = 0;
			virtual ~IThreadSafeObject(){}
	};
}