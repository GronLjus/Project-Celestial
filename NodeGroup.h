#pragma once

namespace Entities
{

	class NodeGroup
	{

		private:
			unsigned int lockedID;

		public:
			NodeGroup();
			unsigned int GetLocked() const;
			void Lock(unsigned int obj);
			~NodeGroup();

	};
}