#pragma once
#include <string>
#include "../../CrossHandler-Tools/Texture/TextureResourceObject.h"
#include "../../Resources/Objects/GUIObjects/GUIObject.h"

namespace Graphics
{

	///<summary>The weight of the font</summary>
	enum FontWeight{FontWeight_DONTCARE,FontWeight_THIN,FontWeight_EXTRALIGHT,FontWeight_LIGHT,FontWeight_NORMAL,FontWeight_MEDIUM,FontWeight_SEMIBOLD,FontWeight_BOLD,FontWeight_EXTRABOLD,FontWeight_HEAVY};

	///<summary>This interface handles all outward text operations</summary>
	class I2DDrawer
	{

		public:
			///<summary>Begins drawing</summary>
			virtual void Begin() = 0;
			///<summary>Stops drawing</summary>
			virtual bool End() = 0;
			///<summary>Initalizes the texthandler</summary>
			///<param name='target'>[in]A pointer to the rendertarget that should be used</param>
			///<param name='width'>[in]The width of the rendertarget in logical coordinates</param>
			///<param name='height'>[in]The height of the rendertarget in logical coordinates</param>
			///<returns>A code determining the result</returns>
			virtual HRESULT Init(CrossHandlers::TextureResourceObject* target, UINT width, UINT height) = 0;
			///<summary>Initiate a new font</summary>
			///<param name='height'>[in]The height of the font</param>
			///<param name='width'>[in]The width of the font</param>
			///<param name='weight'>[in]How the font should be weighted</param>
			///<param name='mipLevels'>[in]The amount of miplevels the resulting font will have</param>
			///<param name='italic'>[in]Whether or not the font should be in italic</param>
			///<param name='faceName'>[in]The name of the font to initiate</param>
			///<param name='index'>[out]The index to use when drawing text with this font</param>
			///<returns>Any eventual errors</returns>
			virtual HRESULT InitFont(int height, UINT width, FontWeight weight, UINT mipLevels, bool italic, LPCWSTR faceName, int &index) = 0;
			///<summary>Initiate a new font</summary>
			///<param name='index'>[out]The index to use when drawing text with this brush</param>
			///<param name='r'>[in]The red of the colour to draw as</param>
			///<param name='g'>[in]The green of the colour to draw as</param>
			///<param name='b'>[in]The blue of the colour to draw as</param>
			///<returns>Any eventual errors</returns>
			virtual HRESULT InitSolidBrush(float r, float g, float b, int &index) = 0;
			///<summary>Draws text to the set target</summary>
			///<param name='text'>[in]The text to draw out</param>
			///<param name='x'>[in]The x-coordinate of the target to use([0,1])</param>
			///<param name='y'>[in]The y-coordinate of the target to use([0,1])</param>
			///<param name='font'>[in]The index of the font to draw the text with</param>
			///<param name='brush'>[in]The index of the brush to draw the text with</param>
			///<returns>If the text got drawn properly</returns>
			virtual bool DrawTextToTarget(std::wstring text,float x,float y,int font,int brush) = 0;
			///<summary>Draws a object to the UI</summary>
			///<param val='object'>[in]The object to draw</param>
			virtual void DrawGUIObject(Resources::GUIObject* object, CelestialMath::Vector2 parentalAbsPos, CelestialMath::Vector2 parentalAbsSize) = 0;
			virtual void DrawGUIObject(Resources::GUIObject* object) = 0;
			///<summary>Releases the resources held by this handler</summary>
			virtual void Release() = 0;
			///<summary>Clears the rendertarget</summary>
			virtual void Clear() = 0;
			virtual ~I2DDrawer(){}

	};
}