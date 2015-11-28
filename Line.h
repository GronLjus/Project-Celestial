#pragma once

class Line
{

	private:
		float originX;
		float originY;
		float originZ;

		float vectorX;
		float vectorY;
		float vectorZ;

	public:
		bool finiteLineIntersect(Line l2);
		bool infiniteLineIntersect(Line l2,&int t1,&int t2);

		float* GetOrigin();
		float* GetVector();
};