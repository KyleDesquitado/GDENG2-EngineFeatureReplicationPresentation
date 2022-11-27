#include "AGizmoScreen.h"

AGizmoScreen::AGizmoScreen(String name)
{
	this->name = name;
}

AGizmoScreen::~AGizmoScreen()
{
}

String AGizmoScreen::getName()
{
	return this->name;
}
