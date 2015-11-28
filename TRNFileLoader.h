#pragma once
#include "IFileTerrainLoader.h"
#include "MTLFileLoader.h"

namespace Resources
{

	///<summary>This class handles the loading of files with the .trn-extension</summary>
	class TRNFileLoader : public IFileTerrainLoader
	{
	
		public:
		
			TRNFileLoader();
			///<summary>Gets an aray of all extensions this reader can handle</summary>
			///<returns>An array of strings</returns>
			virtual std::string* Extension() const;
			///<summary>Gets the amount of extensions this handler can read</summary>
			///<returns>The number of extensions</returns>
			virtual int GetNrExtensions() const;
			///<summary>Initilizes the reader</summary>
			///<param name='card'>[in]The device to base the reader on</param>
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			///<summary>Loads a terrain</summary>
			///<param name='text'>[in]The name of the file containing the terrain-data</param>
			///<returns>A pointer to the terrainobject containing the loaded data</returns>
			virtual TerrainObject* Load(std::string text);
			~TRNFileLoader();

		private:
			///<summary>The underlying loader to handle accomponing material-file</summary>
			MTLFileLoader* mLoader;
			///<summary>An array of file-extensions this loader can handle</summary>
			std::string* extensions;
			///<summary>The amoun of extensions</summary>
			int nrExtensions;

			///<summary>Gets a word from a string</summary>
			///<param name='which'>[in]Which word to extract</param>
			///<param name='what'>[in]The string to extract the word from</param>
			///<returns>The nth word from a string</returns>
			std::string getWord(int which,std::string what) const;
			///<summary>Get the number of words in a string</summary>
			///<param name='what'>[in]The string to count words in</param>
			///<returns>The number of words in the string</returns>
			int getWords(std::string what) const;
			///<summary>Convert a string to a float</summary>
			///<param name='what'>[in]The string to convert</param>
			///<returns>A float of equal value to the string put in</returns>
			int convertStringToInt(std::string what) const;

	};

}