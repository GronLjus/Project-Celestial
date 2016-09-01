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
			unsigned int GetLastNode() const;

			void IncreaseNodes(unsigned int by);
			void DecreaseNodes(unsigned int by);

			void SetLastObject(unsigned int object);
			void SetDirection(Direction dir);

			~Road();

		private:
			Direction dir;
			unsigned int nodes;
			unsigned int lastObject;

	};
}