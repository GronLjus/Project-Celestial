#pragma once
#include "CelestialSlicedList.h"

namespace CrossHandlers
{

	class Dictionary
	{

	public:
		Dictionary();
		unsigned int GetTranslation(unsigned int value) const;
		void AddTranslation(unsigned int value, unsigned int translation);
		~Dictionary();

	private:
		CelestialSlicedList<unsigned int>* dictionary;
	};
}
