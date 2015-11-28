#include "stdafx.h"
#include "GUIObject.h"

using namespace Resources;
using namespace CelestialMath;

GUIObject::GUIObject()
{ 

	triggers = new unsigned int[TriggerType_NA]; 
	children = new CelestialSlicedList<GUIObject*>(32, nullptr); 
	childAmount = 0;
	parentGUI = 0;

}

GUISnap GUIObject::GetHorizontalSnap()
{ 

	return hSnap; 

}

GUISnap GUIObject::GetVerticalSnap()
{ 
	
	return vSnap; 

}

void GUIObject::SetHorizontalSnap(GUISnap snap)
{
	
	hSnap = snap; 

}

void GUIObject::SetVerticalSnap(GUISnap snap)
{ 
	
	vSnap = snap; 

}

Vector2 GUIObject::GetPosition()
{
	
	return position; 

}

void GUIObject::SetPosition(Vector2 pos)
{
	
	position = pos; 

}

Vector2 GUIObject::GetSize()
{
	
	return size; 

}

void GUIObject::SetSize(Vector2 size)
{
	
	this->size = size;

}

void GUIObject::Toggle(bool val)
{
	 
	enabled = val; 
 
}

bool GUIObject::IsEnabled() 
{ 
	
	return enabled; 

}

GUIObjects GUIObject::GetType()
{
	
	return type; 

}

unsigned int GUIObject::GetTrigger(TriggerType type)
{
	
	return triggers[type]; 
}

void GUIObject::SetTrigger(TriggerType type, unsigned int trigger)
{ 
	
	triggers[type] = trigger; 
}

unsigned int GUIObject::GetParentID()
{ 
	
	return parentGUI; 

}

void GUIObject::SetParentID(unsigned int parent, unsigned int childId)
{ 
	
	parentGUI = parent; 
	this->childId = childId; 

}

unsigned int GUIObject::GetChildren()
{ 
	
	return childAmount; 

}

GUIObject* GUIObject::GetChild(unsigned int child)
{ 
	
	return children->GetValue(child); 

}

void GUIObject::AddChild(GUIObject* gui)
{

	int childId = children->Add(gui);
	gui->SetParentID(id, childId);
	childAmount++;

}

void GUIObject::ToggleOnScreen(bool isOnScreen)
{

	this->isOnScreen = isOnScreen;

}

bool GUIObject::IsOnScreen()
{

	return isOnScreen;

}

int GUIObject::GetId() const 
{ 
	
	return id; 

}

void GUIObject::SetId(int id) 
{ 
	
	this->id = id; 

}

GUIObject::~GUIObject()
{ 
	
	delete[] triggers; 
	delete children; 
}