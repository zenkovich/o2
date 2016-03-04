#include "SceneDragHandle.h"

#include "Application/Application.h"
#include "Core/WindowsSystem/WindowsManager.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "SceneWindow/SceneWindow.h"

SceneDragHandle::SceneDragHandle():
	regularSprite(nullptr), hoverSprite(nullptr), pressedSprite(nullptr), cursorType(CursorType::Arrow)
{
	if (WindowsManager::IsSingletonInitialzed())
		o2EditorSceneScreen.mDragHandles.Add(this);

	InitializeProperties();
}

SceneDragHandle::SceneDragHandle(Sprite* regular, Sprite* hover /*= nullptr*/, Sprite* pressed /*= nullptr*/):
	regularSprite(regular), hoverSprite(hover), pressedSprite(pressed), cursorType(CursorType::Arrow)
{
	if (WindowsManager::IsSingletonInitialzed())
		o2EditorSceneScreen.mDragHandles.Add(this);

	InitializeProperties();
}

SceneDragHandle::SceneDragHandle(const SceneDragHandle& other):
	regularSprite(nullptr), hoverSprite(nullptr), pressedSprite(nullptr), cursorType(other.cursorType)
{
	if (WindowsManager::IsSingletonInitialzed())
		o2EditorSceneScreen.mDragHandles.Add(this);

	if (other.regularSprite)
		regularSprite = other.regularSprite->Clone();

	if (other.hoverSprite)
		hoverSprite = other.hoverSprite->Clone();

	if (other.pressedSprite)
		pressedSprite = other.pressedSprite->Clone();

	SetPosition(other.mPosition);

	InitializeProperties();
}

SceneDragHandle::~SceneDragHandle()
{
	if (regularSprite) delete regularSprite;
	if (hoverSprite) delete hoverSprite;
	if (pressedSprite) delete pressedSprite;

	if (WindowsManager::IsSingletonInitialzed())
		o2EditorSceneScreen.mDragHandles.Remove(this);
}

void SceneDragHandle::Draw()
{
	if (!mEnabled)
		return;

	Vec2F screenPosition = o2EditorSceneScreen.SceneToScreenPoint(position);
	/*screenPosition.x = Math::Round(screenPosition.x);
	screenPosition.y = Math::Round(screenPosition.y);*/

	if (regularSprite)
	{
		regularSprite->SetPosition(screenPosition);
		regularSprite->Draw();
	}

	if (hoverSprite)
	{
		hoverSprite->SetPosition(screenPosition);
		hoverSprite->Draw();
	}

	if (pressedSprite)
	{
		pressedSprite->SetPosition(screenPosition);
		pressedSprite->Draw();
	}
}

SceneDragHandle& SceneDragHandle::operator=(const SceneDragHandle& other)
{
	if (regularSprite) delete regularSprite;
	if (hoverSprite) delete hoverSprite;
	if (pressedSprite) delete pressedSprite;

	if (other.regularSprite)
		regularSprite = other.regularSprite->Clone();
	else
		regularSprite = nullptr;

	if (other.hoverSprite)
		hoverSprite = other.hoverSprite->Clone();
	else
		hoverSprite = nullptr;

	if (other.pressedSprite)
		pressedSprite = other.pressedSprite->Clone();
	else
		pressedSprite = nullptr;

	cursorType = other.cursorType;

	SetPosition(other.mPosition);

	return *this;
}

bool SceneDragHandle::IsUnderPoint(const Vec2F& point)
{
	if (regularSprite)
		return regularSprite->IsPointInside(point);

	return false;
}

void SceneDragHandle::OnCursorPressed(const Input::Cursor& cursor)
{
	mIsPressed = true;

	if (regularSprite)
		regularSprite->enabled = false;

	if (pressedSprite)
		pressedSprite->enabled = true;

	mPressOffset = mPosition - o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition);
	onPressed();
}

void SceneDragHandle::OnCursorReleased(const Input::Cursor& cursor)
{
	mIsPressed = false;

	if (regularSprite)
		regularSprite->enabled = true;

	if (pressedSprite)
		pressedSprite->enabled = false;

	onReleased();

	if (!IsUnderPoint(cursor.mPosition))
		o2Application.SetCursor(CursorType::Arrow);
}

void SceneDragHandle::OnCursorPressBreak(const Input::Cursor& cursor)
{
	mIsPressed = false;

	if (regularSprite)
		regularSprite->enabled = true;

	if (pressedSprite)
		pressedSprite->enabled = false;

	onReleased();

	if (!IsUnderPoint(cursor.mPosition))
		o2Application.SetCursor(CursorType::Arrow);
}

void SceneDragHandle::OnCursorStillDown(const Input::Cursor& cursor)
{
	if (mIsPressed && cursor.mDelta != Vec2F())
	{
		Vec2F newPos = o2EditorSceneScreen.ScreenToScenePoint(cursor.mPosition) + mPressOffset;
		Vec2F delta = newPos - mPosition;

		SetPosition(newPos);
		onChangedPos(newPos);
		onMoved(delta);
	}
}

void SceneDragHandle::OnCursorEnter(const Input::Cursor& cursor)
{
	if (hoverSprite)
		hoverSprite->enabled = true;

	if (!cursor.mPressed)
		o2Application.SetCursor(cursorType);
}

void SceneDragHandle::OnCursorExit(const Input::Cursor& cursor)
{
	if (hoverSprite)
		hoverSprite->enabled = false;

	if (!IsPressed() && !cursor.mPressed)
		o2Application.SetCursor(CursorType::Arrow);
}

void SceneDragHandle::SetPosition(const Vec2F& position)
{
	mPosition = position;
}

Vec2F SceneDragHandle::GetPosition() const
{
	return mPosition;
}

void SceneDragHandle::SetAngle(float rad)
{
	mAngle = rad;

	if (regularSprite)
		regularSprite->angle = rad;

	if (hoverSprite)
		hoverSprite->angle = rad;

	if (pressedSprite)
		pressedSprite->angle = rad;
}

float SceneDragHandle::GetAngle() const
{
	return mAngle;
}

void SceneDragHandle::SetEnabled(bool enable)
{
	if (enable == mEnabled)
		return;

	mEnabled = enable;

	if (mEnabled)
		o2EditorSceneScreen.mDragHandles.Add(this);
	else
		o2EditorSceneScreen.mDragHandles.Remove(this);

	if (regularSprite)
		regularSprite->enabled = true;

	if (pressedSprite)
		pressedSprite->enabled = false;

	if (hoverSprite)
		hoverSprite->enabled = false;
}

bool SceneDragHandle::IsEnabled() const
{
	return mEnabled;
}

bool SceneDragHandle::IsPressed() const
{
	return mIsPressed;
}

void SceneDragHandle::InitializeProperties()
{
	INITIALIZE_PROPERTY(SceneDragHandle, position, SetPosition, GetPosition);
	INITIALIZE_PROPERTY(SceneDragHandle, enabled, SetEnabled, IsEnabled);
	INITIALIZE_PROPERTY(SceneDragHandle, angle, SetAngle, GetAngle);
}
