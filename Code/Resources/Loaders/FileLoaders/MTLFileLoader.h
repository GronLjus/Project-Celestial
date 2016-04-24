#pragma once
#include "Interfaces/IFileMaterialLoader.h"
#include "../../TextureHandler.h"

namespace Resources
{

	///<summary>This class handles the loading of mtl-files into a meshobject</summary>
	class MTLFileLoader : public IFileMaterialLoader
	{
		public:

			virtual MeshObject::Material** LoadMaterial(std::string file,int &size);
			///<summary>Ges an aray of all extensions this reader can handle</summary>
			///<returns>An array of strings</returns>
			virtual std::string* Extension() const;
			///<summary>Gets the amount of extensions this handler can read</summary>
			///<returns>The number of extensions</returns>
			virtual int GetNrExtensions() const;
			///<summary>Initilizes the reader</summary>
			///<param name='card'>[in]The device to base the reader on</param>
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			virtual ~MTLFileLoader();

		private:

			///<summary>The underlying device</summary>
			Graphics::CardHandler* card;
			///<summary>Gets a word from a string</summary>
			///<param name='which'>[in]Which word to extract</param>
			///<param name='what'>[in]The string to extract the word from</param>
			///<returns>The nth word from a string</returns>
			std::string getWord(int which, std::string what) const;
			///<summary>Get the number of words in a string</summary>
			///<param name='what'>[in]The string to count words in</param>
			///<returns>The number of words in the string</returns>
			int getWords(std::string what) const;

			void createMaterial(MeshObject::Material* obj, std::string dFile, std::string aFile, std::string nFile, UCHAR* diff, UCHAR* amb);
			///<summary>Convert a string to a float</summary>
			///<param name='what'>[in]The string to convert</param>
			///<returns>A float of equal value to the string put in</returns>
			float convertStringToFloat(std::string what) const;
			///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
			std::string* extensions;
			///<summary>The number of extensions</summary>
			int extens;
			///<summary>The loader to use for textures</summary>
			TextureHandler*  textureLoader;

	};
}