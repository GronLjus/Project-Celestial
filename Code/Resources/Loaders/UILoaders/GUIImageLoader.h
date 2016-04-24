#include "IGUILoader.h"
#include "../ResourceLoader.h"

namespace Resources
{

	class GUIImageLoader : public IGUILoader
	{

		public:

			GUIImageLoader(ResourceLoader* rLoader);
			virtual GUIObject* Create(GUISnap hor, GUISnap ver, std::string file);
			virtual GUIObject* Load(std::string text);
			virtual std::string* Extension() const;
			virtual int GetNrExtensions() const;
			virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
			virtual ~GUIImageLoader();

		private:
			ResourceLoader* resourceLoader;
			///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
			std::string* extensions;
			///<summary>The number of extensions</summary>
			int extens;
	};
}