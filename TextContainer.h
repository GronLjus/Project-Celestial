#pragma once
#include <string>

namespace CrossHandlers
{

	class TextContainer
	{

	public:
		///<param val='maxLines'>The maximum number of lines</param>
		TextContainer();
		///<summary>Adds a row of text</summary>
		///<param val='text'>[in]The text to add</param>
		void AddTextLine(std::string text);
		void AppendText(std::string text);
		void AppendText(char text);
		void SetText(std::string text);
		void EraseText(unsigned int by);
		void MoveCaret(int rows, int columns);
		unsigned int GetCaretPos() const;
		std::string GetText() const;
		///<summary>Clear all text</summary>
		virtual void Clear();
		~TextContainer();

	protected:
		///<summary>An array with the text</summary>
		std::string text;
		///<summary>The number of lines</summary>
		int lines;
		unsigned int caretPos;
		unsigned int caretLine;
		unsigned int caretChar;
		unsigned int maxLines;
		unsigned int* chars;

	};
}