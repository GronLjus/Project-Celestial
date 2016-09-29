#pragma once
#include <string>
#include "IContainable.h"

namespace Resources
{

	///<summary>The class contains the source code of a celscript</summary>
	class KubLingSource : public IContainable
	{

	private:
		enum line{ lineValue, lineSource, lineNa };
			///<summary>The total amount of lines</summary>
			int totalLines;
			///<summary>The array of lines</summary>
			std::string** lines;
			///<summary>The Id of the source</summary>
			int id;

		public:
			///<param val='totalLines'>[in]The total amount of lines</param>
			///<param val='lines'>[in]An array containing all the lines</param>
			KubLingSource(int totalLines,std::string** lines);
			///<summary>Gets a line from the source</summary>
			///<param val='line'>[in]The line to retreive</param>
			///<returns>The line</returns>
			std::string GetLine(int line);
			std::string GetFile(int line);
			///<summary>Get the total amount of lines</summary>
			///<returns>The total amount of lines in the source</returns>
			int GetLines();

			virtual int GetId() const;
			virtual void SetId(int id);
			~KubLingSource();

	};
}