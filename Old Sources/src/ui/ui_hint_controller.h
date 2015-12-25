#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

class UIWidget;
class UILabel;

/** Hint controller. Processing hint widget. */
class UIHintController
{
	UIWidget* mWidget;            /** Main hint widget. */
	UILabel*  mLabel;             /** Label in hint widget. */

	String    mNextPlaceHintText; /** Text for next showing hint. */
	Vec2F     mNextPlacePosition; /** Position for placing hint. */
	bool      mNeedShow;

public:
	/** ctor. */
	UIHintController();

	/** dtor. */
	~UIHintController();

	void SetupWidget(UIWidget* widget, UILabel* label);

	void Update(float dt);
	void Draw();

	void ShowAt(const Vec2F& position, const String& hintString);
	void Hide();

private:
	void WidgetDisappeared();
	void BeginAppearing();
};

CLOSE_O2_NAMESPACE
