#pragma once
#include <string>

namespace CrossHandlers
{

	class TextContainer
	{

	public:
		///<param val='maxLines'>The maximum number of lines</param>
		TextContainer(int maxLines);
		///<summary>Adds a row of text</summary>
		///<param val='text'>[in]The text to add</param>
		virtual void AddText(std::string text);
		virtual void AppendText(std::string text);
		virtual void EraseText(unsigned int by);
		virtual std::string GetText(int line);
		///<summary>Clear all text</summary>
		virtual void Clear();
		~TextContainer();

	protected:
		///<summary>An array with the text</summary>
		std::string* textLines;
		///<summary>The number of lines</summary>
		int lines;
		///<summary>The maximum number of lines</summary>
		int maxLines;

	};
}