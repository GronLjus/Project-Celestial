#pragma once
#include "IFileCLLoader.h"

namespace Resources
{

	///<summary>The filehandler for managing compiled celscripts</summary>
	class CelScriptBinLoader : public IFileCLLoader
	{

	public:
		CelScriptBinLoader();
		virtual std::string* Extension() const;
		virtual int GetNrExtensions() const;
		virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);

		///<summary>Saves compiled celscript to a file</summary>
		///<param val='path'>[in]The path to the file to savein</param>
		///<param val='source'>[in]A pointer to the compiled celscript</param>
		void SaveBinary(std::string path, CelScriptCompiled* source);
		///<summary>Loads a compiled celscript from a file</summary>
		///<param val='path'>[in]The path to the file</param>
		///<returns>A pointer to the loaded celscript</returns>
		virtual CelScriptCompiled* Load(std::string path);
		~CelScriptBinLoader();

	private:
		///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
		std::string* extensions;
		///<summary>The number of extensions</summary>
		int extens;

	};
}