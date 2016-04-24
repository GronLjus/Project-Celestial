#pragma once

namespace Resources
{

	class IContainable
	{

		public:
			///<summary>Gets the unique id for this object</summary>
			///<returns>The unique id</returns>
			virtual int GetId() const = 0;
			///<summary>Sets the unique id for this object</summary>
			///<returns>The unique id</returns>
			virtual void SetId(int id) = 0;

			virtual ~IContainable(){}
	};
}