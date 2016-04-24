#pragma once
#include <string>
#include "../../../../Graphics/CardHandler/CardHandler.h"

namespace Resources
{

	///<summary>This interfaces provides the basic functionality for all fileloaders</summary>
	class IFileLoader
	{
		public:

			///<summary>Gets an aray of all extensions this reader can handle</summary>
			///<returns>An array of strings</returns>
			virtual std::string* Extension() const = 0;
			///<summary>Gets the amount of extensions this handler can read</summary>
			///<returns>The number of extensions</returns>
			virtual int GetNrExtensions() const = 0;
			///<summary>Initilizes the reader</summary>
			///<param name='card'>[in]The device to base the reader on</param>
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText) = 0;
			virtual ~IFileLoader(){}

	};
}