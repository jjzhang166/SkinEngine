/* 
 * File:   ElementInput.cpp
 * Author: Inozuma
 * 
 * Created on January 10, 2012, 10:39 AM
 */

#include "Skin/ElementInput.h"

#include <SDL/SDL.h>

using namespace Skin;

ElementInput::ElementInput(Screen& screen, const Vectorf & position, double size) :
Element(screen, position),
mLabel(""),
mSize(size),
mSelect(false)
{
    mBox.setPosition(mPosition.x, mPosition.y);
    mBox.setSize(size, mLabel.getMaxHeight() * 1.25);
    mBox.setBoxColor(Color(255, 255, 255, 0.4));
    mBox.setOutlineColor(Color(255, 255, 255, 1));
}

ElementInput::~ElementInput()
{
}

Text& ElementInput::label()
{
	return mLabel;
}

Box& ElementInput::box()
{
	return mBox;
}

std::string ElementInput::getData(const std::string& name)
{
    if (name == "value")
        return mText;
    return "";
}

void ElementInput::parse(const std::string& key, const std::string& value)
{
	std::string sFont("font-");
	std::string sBox("box-");
	std::string sSize("size-");

	if (key.find(sFont) == 0)
	{
		mLabel.parse(key.substr(sFont.size()), value);
	}
	else if (key.find(sBox) == 0)
	{
		mBox.parse(key.substr(sBox.size()), value);
	}
	else if (key.find(sSize) == 0)
	{
		std::string subkey(key.substr(sSize.size()));

		if (subkey == "length")
			mSize = atof(value.c_str());
	}
}

bool ElementInput::collide(double x, double y)
{

    if (x > mPosition.x && x < mPosition.x + mSize &&
            y > mPosition.y && y < mPosition.y + mLabel.getMaxHeight())
        return true;
    return false;
}

void ElementInput::draw(SDL_Surface* displaySurface)
{
    mLabel.setText(mText + (mSelect ? "|" : ""));
    mLabel.setPosition(mPosition.x + mTranslation.x + mLabel.getMaxHeight() / 4, mPosition.y + mTranslation.y);
    mLabel.setScale(1 + mScale.x, 1 + mScale.y);
    
    mBox.draw(displaySurface);
    mLabel.draw(displaySurface);
}

void ElementInput::event(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN
            && event.key.keysym.sym != SDLK_RETURN
            && event.key.keysym.sym != SDLK_BACKSPACE
            && event.key.keysym.sym != SDLK_DELETE
            && event.key.keysym.unicode != 0
            && event.key.keysym.unicode != '\r'
            && event.key.keysym.unicode != '\n'
            && event.key.keysym.unicode != '\b')
    {
        if (event.key.keysym.sym != SDLK_BACKSPACE)
            mText.push_back(static_cast<char>(event.key.keysym.unicode));
    }
    else if (event.type == SDL_KEYDOWN)
    {
        if ((event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE)
                && !mText.empty())
        {
            mText.resize(mText.size() - 1);
        }
        else
            Element::event(event);
    }
}

void ElementInput::onFocus()
{
    mSelect = true;
    Element::onFocus();
}

void ElementInput::onIdle()
{
    mSelect = false;
    Element::onIdle();
}