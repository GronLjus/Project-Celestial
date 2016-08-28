#include "stdafx.h"
#include "Dictionary.h"

using namespace CrossHandlers;

Dictionary::Dictionary()
{

	dictionary = new CelestialSlicedList<unsigned int>(32, 0);

}

unsigned int Dictionary::GetTranslation(unsigned int value) const
{

	return dictionary->GetValue(value);

}

void Dictionary::AddTranslation(unsigned int value, unsigned int translation)
{

	dictionary->Add(translation, value);

}

Dictionary::~Dictionary()
{

	delete dictionary;

}