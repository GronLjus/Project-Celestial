#pragma once
#include <string>
#include "CelestialVector.h"
#include "IGUIObject.h"

namespace Resources
{

	class IGUITextObject : public IGUIObject
	{

	public:
		///<summary>Gets the number of lines to draw</summary>
		///<returns>The number of lines this object contains</returns>
		virtual int GetTextLines() = 0;
		///<summary>Gets the text the object contains</summary>
		///<param val='line'>[in]The line to get</param>
		///<returns>A string contaning the line</returns>
		virtual std::string GetText(int line) = 0;
		///<summary>Gets the angle the textlines should be drawn in</summary>
		///<returns>An angle in radians</returns>
		virtual float GetAngle() = 0;

		///<summary>Sets a textline</summary>
		///<param val='line'>[in]The line to change</param>
		///<param val='text'>[in]A string of the text to set</param>
		virtual void SetText(int line, std::string text) = 0;
		///<summary>Appends text to a new line</summary>
		///<param val='text'>[in]A string with the text to add</param>
		virtual void AddText(std::string text) = 0;

		///<summary>Clears all text</summary>
		virtual void ClearBox() = 0;
		///<summary>If the box is multiline</summary>
		///<returns>Wether or not the box is a multiline-textbox</returns>
		virtual bool IsMultiLine() = 0;

		virtual ~IGUITextObject(){};

	};
}