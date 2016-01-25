#include "stdafx.h"
#include "Celestial2DDrawer.h"
#include "CelestialMath.h"
#include "GUITextBox.h"
#include "GUILayout.h"

using namespace Graphics;
using namespace CelestialMath;
using namespace Resources;

std::wstring s2ws2(const std::string& s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;

}

Celestial2DDrawer::Celestial2DDrawer()
{

	maxFonts = 5;
	numberOfFonts = 0;
	maxBrushes = maxFonts;
	numberOfBrushes = 0;
	fonts = new IDWriteTextFormat*[maxFonts];
	brushes = new ID2D1Brush*[maxBrushes];
	
	writeFactory = nullptr;
	d2DFactory = nullptr;
	rT = nullptr;

	for(int i=0;i<maxFonts;i++)
	{

		fonts[i] = nullptr;
		brushes[i] = nullptr;

	}
}

void Celestial2DDrawer::Begin()
{

	rT->BeginDraw();
}

bool Celestial2DDrawer::End()
{

	HRESULT hr = rT->EndDraw();
	return hr == S_OK;

}

HRESULT Celestial2DDrawer::Init(CrossHandlers::TextureResourceObject* target, UINT width, UINT height)
{

	this->width = width;
	this->height = height;
	HRESULT hr = S_OK;


	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2DFactory);

	IDXGISurface* dxgiS;
	hr = target->GetDXT()->GetTexture()->QueryInterface(__uuidof(IDXGISurface), (LPVOID*)&dxgiS);

	FLOAT dpiX;
	FLOAT dpiY;
	d2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX,
		dpiY);

	hr = d2DFactory->CreateDxgiSurfaceRenderTarget(dxgiS, &props, &rT);
	dxgiS->Release();

	return hr;

}

HRESULT Celestial2DDrawer::InitFont(int height, UINT width, FontWeight weight, UINT mipLevels, bool italic, LPCWSTR faceName, int &index)
{

	HRESULT hr = S_OK;

	if(numberOfFonts >= maxFonts)
	{

		extendFontArray(5);

	}

	DWRITE_FONT_WEIGHT fw = transFormFW(weight);
	index = numberOfFonts;

	 hr= writeFactory->CreateTextFormat(faceName, nullptr, fw, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, L"en-us", &fonts[numberOfFonts]);

	numberOfFonts++;
	return hr;

}

HRESULT Celestial2DDrawer::InitSolidBrush(float r, float g, float b, int &index)
{

	HRESULT hr = S_OK;

	if (numberOfBrushes >= maxBrushes)
	{

		extendBrushArray(5);

	}

	index = numberOfBrushes;
	ID2D1SolidColorBrush* temp;

	hr = rT->CreateSolidColorBrush(
		D2D1::ColorF(r,g,b),
		&temp
		);

	brushes[numberOfBrushes] = temp;
	numberOfBrushes++;
	return hr;

}

bool Celestial2DDrawer::DrawTextToTarget(std::wstring text, float x, float y, int font, int brush)
{

	if (fonts[font] != nullptr && brushes[brush] != nullptr)
	{

		RECT rect = RECT();
		BOOL val = SetRect(&rect,x*width,y*height,width+1,height+1);

		D2D1_RECT_F layoutRect = D2D1::RectF(
			x*width,
			y*height,
			width + 1,
			height + 1
			);

		rT->DrawText(text.c_str(), text.length(), fonts[font], layoutRect, brushes[brush]);
		return true;

	}

	return false;

}

void Celestial2DDrawer::DrawGUIObject(Resources::GUIObject* object)
{

	DrawGUIObject(object, Vector2(0, 0), Vector2(width, height));

}


void Celestial2DDrawer::DrawGUIObject(Resources::GUIObject* object, Vector2 parentalAbsPos, Vector2 parentalAbsSize)
{

	if (object->IsEnabled())
	{

		Vector2 topLeft = Vector2(
			object->GetHorizontalSnap() == GUISnap_LEFT ? object->GetPosition().x : object->GetHorizontalSnap() == GUISnap_MIDDLE ? 0.5f - object->GetSize().x*0.5f : object->GetSize().x - object->GetPosition().x,
			object->GetVerticalSnap() == GUISnap_TOP ? object->GetPosition().y : object->GetVerticalSnap() == GUISnap_MIDDLE ? 0.5f - object->GetSize().y*0.5f : object->GetSize().y - object->GetPosition().y);

		Vector2 topLeftAbs = parentalAbsPos + topLeft * parentalAbsSize;
		Vector2 abSize = object->GetSize() * parentalAbsSize;

		D2D1_RECT_F layoutRect = D2D1::RectF(
			topLeftAbs.x,
			topLeftAbs.y,
			topLeftAbs.x + abSize.x,
			topLeftAbs.y + abSize.y
			);


		D2D1::Matrix3x2F wM;
		rT->GetTransform(&wM);
		D2D1_LAYER_PARAMETERS layPar = D2D1_LAYER_PARAMETERS();
		layPar.contentBounds = layoutRect;
		layPar.maskTransform = D2D1::Matrix3x2F::Identity();
		rT->PushAxisAlignedClip(
			layoutRect,
			D2D1_ANTIALIAS_MODE_ALIASED
			);

		if (object->GetType() == GUIObjects_TEXTBOX)
		{

			GUITextBox* tB = (GUITextBox*)object;
			std::string superString = "";

			for (int i = 0; i < tB->GetTextLines(); i++)
			{

				superString += tB->GetText(i);
				superString += "\n";

			}

			IDWriteTextLayout* boxLayout = nullptr;
			
			DWRITE_TEXT_METRICS metrics;
			writeFactory->CreateTextLayout(s2ws2(superString).c_str(), superString.length(), fonts[0], layoutRect.right - layoutRect.left, layoutRect.bottom - layoutRect.top, &boxLayout);
			boxLayout->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
			boxLayout->GetMetrics(&metrics);
			D2D1_POINT_2F textTop = D2D1_POINT_2F();

			textTop.x = layoutRect.left;
			textTop.y = layoutRect.top - max(0, metrics.height - metrics.layoutHeight);

			rT->DrawTextLayout(textTop, boxLayout,brushes[0]);
			boxLayout->Release();

		}

		for (int i = 0; i < object->GetChildren(); i++)
		{

			if (object->GetChild(i) != nullptr)
			{

				DrawGUIObject(object->GetChild(i),topLeftAbs,abSize);

			}
		}

		rT->PopAxisAlignedClip();

	}
}

void Celestial2DDrawer::Clear()
{
	D2D1_COLOR_F col = D2D1_COLOR_F();

	rT->Clear(col);

}

void Celestial2DDrawer::Release()
{

	for(int i=0;i<numberOfFonts;i++)
	{

		fonts[i]->Release();

	}

	for (int i = 0; i<numberOfBrushes; i++)
	{

		brushes[i]->Release();

	}

	if (rT != nullptr)
	{

		rT->Release();

	}

	writeFactory->Release();
	d2DFactory->Release();

}

void Celestial2DDrawer::extendFontArray(int by)
{

	maxFonts += by;
	IDWriteTextFormat** tempFonts = new IDWriteTextFormat*[maxFonts];

	for(int i=0;i<maxFonts-by;i++)
	{

		tempFonts[i] = fonts[i];

	}

	delete[] fonts;
	fonts = tempFonts;

}

void Celestial2DDrawer::extendBrushArray(int by)
{

	maxBrushes += by;
	ID2D1Brush** tempBrushes = new ID2D1Brush*[maxFonts];

	for (int i = 0; i<maxBrushes - by; i++)
	{

		tempBrushes[i] = brushes[i];

	}

	delete[] brushes;
	brushes = tempBrushes;

}

DWRITE_FONT_WEIGHT Celestial2DDrawer::transFormFW(FontWeight weight)
{

	switch(weight)
	{

	case FontWeight_BOLD: return DWRITE_FONT_WEIGHT_BOLD;
	case FontWeight_EXTRABOLD: return DWRITE_FONT_WEIGHT_EXTRA_BOLD;
	case FontWeight_EXTRALIGHT: return DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
	case FontWeight_HEAVY: return DWRITE_FONT_WEIGHT_HEAVY;
	case FontWeight_LIGHT: return DWRITE_FONT_WEIGHT_LIGHT;
	case FontWeight_MEDIUM: return DWRITE_FONT_WEIGHT_MEDIUM;
	case FontWeight_SEMIBOLD: return DWRITE_FONT_WEIGHT_SEMI_BOLD;
	case FontWeight_THIN: return DWRITE_FONT_WEIGHT_THIN;
	default: return DWRITE_FONT_WEIGHT_NORMAL;

	}
}

Celestial2DDrawer::~Celestial2DDrawer()
{
	
	delete[] fonts;
	delete[] brushes;

}