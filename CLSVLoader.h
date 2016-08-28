#pragma once
#include "IFileLoader.h"

namespace Resources
{

	class CLSVLoader : public IFileLoader
	{

		public:
			CLSVLoader();
			char* Load(std::string path, unsigned int &size);
			void Save(std::string path, char* data, unsigned int size);

			virtual std::string* Extension() const;
			virtual int GetNrExtensions() const;
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			virtual ~CLSVLoader();

		private:
			unsigned int blockSize;
			TextContainer* outText;

			///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
			std::string* extensions;
			///<summary>The number of extensions</summary>
			int extens;
	};
}