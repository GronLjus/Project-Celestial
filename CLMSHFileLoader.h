#pragma once
#include "IFileMeshLoader.h"

namespace Resources
{

	///<summary>This class handlers the loading of obj-files into a mesh</summary>
	class CLMSHFileLoader : public IFileMeshLoader
	{
	public:

		CLMSHFileLoader();
		///<summary>Loads a mesh</summary>
		///<param name='text'>[in]The name of the file containing the mesh</param>
		///<returns>A meshobject containing the mesh-data</returns>
		virtual CelMesh* Load(std::string text);
		///<summary>Ges an aray of all extensions this reader can handle</summary>
		///<returns>An array of strings</returns>
		virtual std::string* Extension() const;
		///<summary>Gets the amount of extensions this handler can read</summary>
		///<returns>The number of extensions</returns>
		virtual int GetNrExtensions() const;
		///<summary>Initilizes the reader</summary>
		///<param name='card'>[in]The device to base the reader on</param>
		virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
		virtual ~CLMSHFileLoader();

	private:

		char* fileBuffer;

		char getByte(unsigned int offset);
		float getFloat(unsigned int offset);
		unsigned int getUInt(unsigned int offset);
		
		TextContainer* outText;

		///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
		std::string* extensions;
		///<summary>The number of extensions</summary>
		int extens;

	};
}