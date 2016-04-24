#pragma once
#include "IFileMeshLoader.h"
#include "MTLFileLoader.h"
#include "ColourLoader.h"
#include <vector>

namespace Resources
{

	///<summary>This class handlers the loading of obj-files into a mesh</summary>
	class OBJFileLoader : public IFileMeshLoader
	{
		public:

			OBJFileLoader();
			///<summary>Loads a mesh</summary>
			///<param name='text'>[in]The name of the file containing the mesh</param>
			///<returns>A meshobject containing the mesh-data</returns>
			virtual MeshObject* Load(std::string text);
			///<summary>Ges an aray of all extensions this reader can handle</summary>
			///<returns>An array of strings</returns>
			virtual std::string* Extension() const;
			///<summary>Gets the amount of extensions this handler can read</summary>
			///<returns>The number of extensions</returns>
			virtual int GetNrExtensions() const;
			///<summary>Initilizes the reader</summary>
			///<param name='card'>[in]The device to base the reader on</param>
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			virtual ~OBJFileLoader();

		private:

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
			float convertStringToFloat(std::string what) const;
			///<summary>Convert a string to a int</summary>
			///<param name='what'>[in]The string to convert</param>
			///<returns>A int of equal value to the string put in</returns>
			int convertStringToInt(std::string what) const;
			///<summary>Adds a number of vertices into a vector</summary>
			///<param name='max'>[in/out]The internal dimension fo the vertex</param>
			///<param name='vertices'>[in]The reference to the double vector containing the vertices</param>
			///<param name='line'>[in]The line of text to extract vertexdata from</param>
			void addVertices(int &max, std::vector<std::vector<float>> &vertices, std::string line);
			///<summary>The internal loader to get the mtl-file</summary>
			MTLFileLoader* mtlLoader;
			///<summary>The internal loader to get a colour-texture</summary>
			ColourLoader* colLoader;
			///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
			std::string* extensions;
			///<summary>The number of extensions</summary>
			int extens;

	};
}