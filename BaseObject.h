#pragma once

namespace Resources
{

	class BaseObject
	{

		public:
			void SetId(unsigned int id);
			unsigned int GetId() const;
			virtual ~BaseObject(){}

		private:
			unsigned int id;
	};
}