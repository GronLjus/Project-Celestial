#pragma once

namespace Entities
{

	class Road
	{
		public:
			enum Direction{ Direction_UP, Direction_DOWN, Direction_NA};
			Road();
			Direction GetDirection() const;
			unsigned int GetNodes() const;
			unsigned int GetLastObject() const;
			unsigned int GetHeaderNode() const;

			void IncreaseNodes(unsigned int by);
			void DecreaseNodes(unsigned int by);

			void SetLastObject(unsigned int object);
			void SetDirection(Direction dir);
			void SetHeaderNode(unsigned int header);

			~Road();

		private:
			Direction dir;
			unsigned int nodes;
			unsigned int lastObject;
			unsigned int headerNode;

	};
}