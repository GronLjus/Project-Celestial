#pragma once
#include "IFileCLLoader.h"
#include "CelScriptCompiler.h"

namespace Resources
{

	///<summary>Handles the loading and saving of colncript-source files</summary>
	class CelScriptSourceLoader : public IFileCLLoader
	{

	public:
		CelScriptSourceLoader();
		virtual std::string* Extension() const;
		virtual int GetNrExtensions() const;
		virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);

		///<summary>Save a script to file</summary>
		///<param val='path'>[in]The path to the file to save</param>
		///<param val='source'>[in]A pointer to the source to save</param>
		void SaveSource(std::string path, CelScriptSource* source);
		///<summary>Load a script</summary>
		///<param val='path'>[in]The path to the file to load from</param>
		///<returns>A pointer to the loaded source</returns>
		CelScriptSource* LoadSource(std::string path);
		virtual CelScriptCompiled* Load(std::string path);
		~CelScriptSourceLoader();

	private:
		///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
		std::string* extensions;
		///<summary>The number of extensions</summary>
		int extens;
		CrossHandlers::CelestialList<std::string*>* putFileIntoList(std::string path);

		///<summary>The compiler for compiling celscripts</summary>
		Logic::CelScriptCompiler* celCompiler;

	};
}