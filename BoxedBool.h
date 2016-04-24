#pragma once

namespace CrossHandlers
{
	///<summary>This class encapsulats a bool so it retains a value</summary>
	class BoxedBool
	{
		public:
			///<param name='val'>[in]The object value</param>
			BoxedBool(bool val);
			///<summary>Compare this object to a bool</summary>
			///<param name='value'>[in]The value to compare to</param>
			///<returns>If this object is equal to the comparisionvalue</returns>
			bool operator==(const bool &value);
			///<summary>Gets this objects internal value</summary>
			///<returns>The object value</returns>
			bool GetValue();
			///<summary>Sets this objects internal value</summary>
			///<param name='val'>[in]The object value</param>
			void SetValue(bool val);
		private:
			///<summary>The internal value of this object</summary>
			bool value;
	};
}