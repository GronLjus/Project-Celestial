#pragma once

#include "IFileLoader.h"
#include "GUIObject.h"
#include "KubLingCompiled.h"

namespace Resources
{

	class IFileCLLoader : public IFileLoader
	{

	public:
		virtual KubLingCompiled* Load(std::string path) = 0;
		virtual ~IFileCLLoader(){}

	};
}