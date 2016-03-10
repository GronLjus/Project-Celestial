#include "stdafx.h"
#include "GUILayout.h"

using namespace Resources;

GUILayout::GUILayout(GUISnap h, GUISnap v) : GUIObject()
{

	children = new CelestialSlicedList<GUIObject*>(32, nullptr);
	childAmount = 0;
	type = GUIObjects_LAYOUT;
	hSnap = h;
	vSnap = v;

}

unsigned int GUILayout::GetChildren() const
{

	return childAmount;

}

GUIObject* GUILayout::GetChild(unsigned int child) const
{

	return children->GetValue(child);

}

void GUILayout::AddChild(GUIObject* gui)
{

	int childId = children->Add(gui);
	gui->SetParentID(GetId(), childId);
	childAmount++;

}

GUILayout::~GUILayout()
{

	delete children;

}