#pragma once

#include "IGUILoader.h"

namespace Resources
{

	class GUILayoutLoader : public IGUILoader
	{

	public:

		GUILayoutLoader();
		virtual GUIObject* Create(GUISnap hor, GUISnap ver);
		virtual GUIObject* Load(std::string text);
		virtual std::string* Extension() const;
		virtual int GetNrExtensions() const;
		virtual void Init(Graphics::CardHandler* &card, TextContainer* outText);
		virtual ~GUILayoutLoader();

	private:

		///<summary>An array of strings representing the possible file-extensitons this reader can use</summary>
		std::string* extensions;
		///<summary>The number of extensions</summary>
		int extens;
	};
}