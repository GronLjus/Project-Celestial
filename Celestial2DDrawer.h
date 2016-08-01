#pragma once
#include <dwrite.h>
#include <d2d1.h>
#include <string>
#include "TextureResourceObject.h"
#include "ImageResourceObject.h"
#include "GUIObject.h"

namespace Graphics
{

	///<summary>The weight of the font</summary>
	enum FontWeight{ FontWeight_DONTCARE, FontWeight_THIN, FontWeight_EXTRALIGHT, FontWeight_LIGHT, FontWeight_NORMAL, FontWeight_MEDIUM, FontWeight_SEMIBOLD, FontWeight_BOLD, FontWeight_EXTRABOLD, FontWeight_HEAVY };

	///<summary>This class draws 2d objects to a texture</summary>
	class Celestial2DDrawer
	{
		public:
			Celestial2DDrawer();
			void Begin();
			bool End();
			HRESULT Init(CrossHandlers::TextureResourceObject* target, UINT width, UINT height);
			HRESULT InitFont(int height,UINT width,FontWeight weight,UINT mipLevels,bool italic,LPCWSTR faceName,int &index);
			HRESULT InitSolidBrush(float r, float g, float b, int &index);
			ImageResourceObject* Load2DImage(unsigned char* values, UINT bPC, UINT channels, UINT width, UINT height);
			void DrawGUIObject(Resources::GUIObject* object, unsigned int time);
			void RecycleImageRenderTarget(unsigned int id);
			void Clear();
			void Release();
			void SetBorderBrush(Resources::GUIObject* object, CelestialMath::Vector3 color);
			void SetContentBrush(Resources::GUIObject* object, CelestialMath::Vector3 color);
			~Celestial2DDrawer();

		private:

			///<summary>Extends the internal array containing the fonts</summary>
			///<param name='by'>[in]The amount to increase the array by</param>
			void extendFontArray(int by);
			///<summary>Extends the internal array containing the brushes</summary>
			///<param name='by'>[in]The amount to increase the array by</param>
			void extendBrushArray(int by);
			///<summary>Transforms the FontWeight into DX10FW</summary>
			///<param name='weight'>[in]The FontWeight to transform</param>
			///<returns>A value representing the FW-value</returns>
			DWRITE_FONT_WEIGHT transFormFW(FontWeight weight);

			///<summary>A array containing pointers to all fonts used</summary>
			IDWriteTextFormat** fonts;
			///<summary>The number of usable fonts in the array</summary>
			int numberOfFonts;
			///<summary>The total number of fonts the array can contain</summary>
			int maxFonts;

			///<summary>The factory to use when creating text</summary>
			IDWriteFactory* writeFactory;
			///<summary>The factory to use when building d2-objects</summary>
			ID2D1Factory* d2DFactory;
			///<summary>The rendertarget to use when drawing</summary>
			ID2D1RenderTarget* rT;

			///<summary>An array containing the brushes to use when drawing text</summary>
			ID2D1Brush** brushes;
			///<summary>The number of usable fonts in the array</summary>
			int numberOfBrushes;
			///<summary>The total number of fonts the array can contain</summary>
			int maxBrushes;

			///<summary>The width of the rendertarget</summary>
			UINT width;
			///<summary>The height of the rendertarget</summary>
			UINT height;

			unsigned int caretBlinkInterVal;
			unsigned int caretBlinkShow;
			bool caretBlink;
			unsigned int caretTime;
			unsigned int caretLastTime;


			struct imageRenderStruct
			{

				unsigned int id;
				ID2D1BitmapRenderTarget* renderTarget;

			};

			CrossHandlers::CelestialStack<imageRenderStruct>* imageRenderTargets;
			CrossHandlers::CelestialSlicedList<ID2D1BitmapRenderTarget*>* totalImageRenderTargets;
			unsigned int totalRenderTargets;

	};
}