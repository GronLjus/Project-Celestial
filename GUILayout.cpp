#include "stdafx.h"
#include "GUILayout.h"

using namespace Resources;

GUILayout::GUILayout(GUISnap h, GUISnap v) : GUIObject()
{

	children = new CelestialSlicedList<GUIObject*>(32, nullptr);
	type = GUIObjects_LAYOUT;
	hSnap = h;
	vSnap = v;

}

unsigned char* GUILayout::Update(Message* mess)
{

	if (mess->type == MessageType_OBJECT)
	{

		unsigned int param1;
		bool found = false; 
		unsigned int i;
		GUIObject* obj = nullptr;

		switch (mess->mess)
		{

		case ObjectMess_REMOVECHILD:
			param1 = mess->params[0] | ((int)mess->params[1] << 8) | ((int)mess->params[2] << 16) | ((int)mess->params[3] << 24);
			obj = children->GetValue(param1);

			if (enabled)
			{

				pauseRendering();

			}

			obj->Disable();
			children->Remove(param1);
			resumeRendering();

			break;
		default:
			return GUIObject::Update(mess);

		}
	}

	return nullptr;

}

void GUILayout::Enable()
{

	for (unsigned int i = 0; i < GetChildren() && !enabled; i++)
	{

		GUIObject* obj = children->GetValue(i);

		if (obj != nullptr)
		{

			obj->Enable();

		}
	}

	GUIObject::Enable();

}

void GUILayout::Disable()
{

	for (unsigned int i = 0; i < GetChildren() && enabled; i++)
	{

		GUIObject* obj = children->GetValue(i);

		if (obj != nullptr)
		{

			obj->Disable();

		}

	}

	GUIObject::Disable();

}

unsigned int GUILayout::GetChildren() const
{

	return children->GetFirstEmpty();

}

GUIObject* GUILayout::GetChild(unsigned int child) const
{

	return children->GetValue(child);

}

void GUILayout::SetParent(GUIObject* parent, unsigned int childId)
{

	GUIObject::SetParent(parent, childId); 
	
	for (unsigned int i = 0; i < GetChildren(); i++)
	{

		GUIObject* obj = children->GetValue(i);

		if (obj != nullptr)
		{

			obj->SetLayer(GetLayer()+1+i);

		}
	}
}

void GUILayout::AddChild(GUIObject* gui)
{

	if (enabled)
	{

		gui->Enable();

	}

	int childId = children->Add(gui);
	gui->SetParent(this, childId);

}

GUILayout::~GUILayout()
{

	delete children;

}