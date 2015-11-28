#pragma once

namespace CrossHandlers
{

	///<summary>This class encapsulates all methods required for an object to be cacheble</summary>
	class ICachableObject
	{

		///<summary>Locks an object so it can't be deleted</summary>
		virtual void Lock() = 0;
		///<summary>Unlocks the object so it can be deleted and have the systems permission to die</summary>
		virtual void Unlock() = 0;
		///<summary>Flags the object so it it will perform suicide when it's unlocked</summary>
		virtual void FlagForDeletion() = 0;

	};
}