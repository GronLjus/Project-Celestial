#include "stdafx.h"
#include "KeyCodes.h"

unsigned int Input::ConvertCode(Input::CelestialKeyCategories cat, unsigned char key)
{

	unsigned int retVal = key;
	retVal += cat == Input::CelestialKeyCategories_MOD ? Input::CelestialCharKeyCodes_NA :
		cat == Input::CelestialKeyCategories_SPEC ? Input::CelestialCharKeyCodes_NA + Input::CelestialModKeyCodes_NA :
		0;
	return retVal;

}

Input::CelestialKeyCategories Input::GetCat(unsigned int code)
{

	if (code >= Input::CelestialCharKeyCodes_NA + Input::CelestialModKeyCodes_NA)
	{

		return  Input::CelestialKeyCategories_SPEC;

	}

	if (code >= Input::CelestialCharKeyCodes_NA)
	{

		return  Input::CelestialKeyCategories_MOD;

	}

	return Input::CelestialKeyCategories_CHAR;

}

unsigned char Input::GetCode(unsigned int code, Input::CelestialKeyCategories cat)
{

	if (cat == Input::CelestialKeyCategories_SPEC)
	{

		return code - (Input::CelestialCharKeyCodes_NA + Input::CelestialModKeyCodes_NA);

	}
	else if (cat == Input::CelestialKeyCategories_MOD)
	{
		
		return code - (Input::CelestialCharKeyCodes_NA);

	}
	
	return code;

}