#pragma once

#include "IFileLoader.h"
#include "../../../Objects/CelScript/CelScriptCompiled.h"

namespace Resources
{

	class IFileCLLoader : public IFileLoader
	{

	public:
		virtual CelScriptCompiled* Load(std::string path) = 0;
		virtual ~IFileCLLoader(){}

	};
}