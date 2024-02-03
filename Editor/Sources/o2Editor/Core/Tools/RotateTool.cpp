#include <memory>
#include "o2Editor/stdafx.h"
#include "o2/Render/Mesh.h"
#include "o2/Render/Render.h"
#include "o2/Render/Sprite.h"
#include "o2/Utils/Editor/SceneEditableObject.h"
#include "o2Editor/Core/Actions/Transform.h"
#include "o2Editor/Core/EditorApplication.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"
#include "o2Editor/TreeWindow/SceneHierarchyTree.h"
#include "o2Editor/TreeWindow/TreeWindow.h"

namespace Editor
{
	RotateTool::RotateTool()
	{
		mPivotDragHandle = Ref<SceneDragHandle>(
			mmake<Sprite>("ui/UI2_pivot.png"),
			mmake<Sprite>("ui/UI2_pivot_select.png"),
			mmake<Sprite>("ui/UI2_pivot_pressed.png"));

		mRotateRingFillMesh = std::make_unique<Mesh>(nullptr, mRotateRingSegs * 4, mRotateRingSegs * 2);
		mAngleMesh = std::make_unique<Mesh>(nullptr, mRotateRingSegs * 4, mRotateRingSegs * 2);

		mPivotDragHandle->onChangedPos += THIS_FUNC(OnPivotDragHandleMoved);
		mPivotDragHandle->enabled = false;
	}

	RotateTool::~RotateTool()
	{
		// No need to manually delete smart pointers

		// if (mRotateRingFillMesh)
		// 	delete mRotateRingFillMesh;

		// if (mAngleMesh)
		// 	delete mAngleMesh;
	}

	String RotateTool::GetPanelIcon() const
	{
		return "ui/UI4_rotate_tool.png";
	}

	ShortcutKeys RotateTool::GetShortcut() const
	{
		return ShortcutKeys('E');
	}

	void RotateTool::Update(float dt)
	{}

	void RotateTool::DrawScreen()
	{
		if (o2EditorSceneScreen.GetSelectedObjects().Count() > 0)
		{
			UpdateMeshes();
			mRotateRingFillMesh->Draw();
			if (mRingPressed)
				mAngleMesh->Draw();

			Vec2F screenPos = o2EditorSceneScreen.SceneToScreenPoint(mScenePivot);
			o2Render.DrawAACircle(screenPos, mRotateRingInsideRadius, mRotateRingsColor, mRotateRingSegs);
			o2Render.DrawAACircle(screenPos, mRotateRingOutsideRadius, mRotateRingsColor, mRotateRingSegs);
		}

		SelectionTool::DrawScreen();
	}

	void RotateTool::OnEnabled()
	{
		CalcPivotByObjectsCenter();
		mPivotDragHandle->enabled = true;
	}

	void RotateTool::OnDisabled()
	{
		mPivotDragHandle->enabled = false;
	}

	void RotateTool::OnSceneChanged(Vector<Ref<SceneEditableObject>> changedObjects)
	{}

	void RotateTool::OnObjectsSelectionChanged(Vector<Ref<SceneEditableObject>> objects)
	{
		CalcPivotByObjectsCenter();
	}

	void RotateTool::UpdateMeshes()
	{
		Vec2F screenPos = o2EditorSceneScreen.SceneToScreenPoint(mScenePivot);

		Color4 fillColor = mRotateRingsFillColor;
		Color4 fillColor2 = mRotateRingsFillColor2;

		const float selectionAlphaCoef = 1.2f;
		const float pressingAlphaCoef = 0.5f;

		if (mRingPressed)
		{
			fillColor.a = (int)((float)fillColor.a * pressingAlphaCoef);
			fillColor2.a = (int)((float)fillColor2.a * pressingAlphaCoef);
		}
		else if (IsPointInRotateRing(o2Input.GetCursorPos()))
		{
			fillColor.a = (int)((float)fillColor.a * selectionAlphaCoef);
			fillColor2.a = (int)((float)fillColor2.a * selectionAlphaCoef);
		}

		ULong fillColorUL = fillColor.ARGB();
		ULong fillColorUL2 = fillColor2.ARGB();
		mRotateRingFillMesh->vertexCount = mRotateRingSegs * 4;
		mRotateRingFillMesh->polyCount = mRotateRingSegs * 2;
		float segAngle = 2.0f * Math::PI() / (float)mRotateRingSegs;

		float angle = 0.0f;
		int i = 0;
		while (angle < 2.0f * Math::PI())
		{
			float angleNext = angle + segAngle;

			Vec2F pinside = Vec2F::Rotated(angle) * mRotateRingInsideRadius + screenPos;
			Vec2F poutside = Vec2F::Rotated(angle) * mRotateRingOutsideRadius + screenPos;
			Vec2F pinsideNext = Vec2F::Rotated(angleNext) * mRotateRingInsideRadius + screenPos;
			Vec2F poutsideNext = Vec2F::Rotated(angleNext) * mRotateRingOutsideRadius + screenPos;

			int vi = i * 4;
			int pi = i * 6;
			i++;

			ULong currFillColor = fillColorUL;
			if (angle < Math::PI() * 0.5f || (angle >= Math::PI() - FLT_EPSILON && angle <= Math::PI() * 1.5f + FLT_EPSILON))
				currFillColor = fillColorUL2;

			mRotateRingFillMesh->vertices[vi] = Vertex(pinside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 1] = Vertex(poutside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 2] = Vertex(pinsideNext, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 3] = Vertex(poutsideNext, currFillColor, 0.0f, 0.0f);

			mRotateRingFillMesh->indexes[pi] = vi;
			mRotateRingFillMesh->indexes[pi + 1] = vi + 1;
			mRotateRingFillMesh->indexes[pi + 2] = vi + 3;

			mRotateRingFillMesh->indexes[pi + 3] = vi;
			mRotateRingFillMesh->indexes[pi + 4] = vi + 3;
			mRotateRingFillMesh->indexes[pi + 5] = vi + 2;

			angle = angleNext;
		}

		mAngleMesh->vertexCount = 0;
		mAngleMesh->polyCount = 0;

		if (!mRingPressed)
			return;

		angle = mPressAngle;
		i = 0;
		float direction = Math::Sign(mCurrentRotateAngle - mPressAngle);
		ULong angleRingColor = direction > 0.0f ? mRotateMeshClockwiseColor.ABGR() : mRotateMeshCClockwiseColor.ABGR();
		int reqAngleMeshSegs = Math::CeilToInt(Math::Abs(mCurrentRotateAngle - mPressAngle) / segAngle) + 1;
		mAngleMesh->Resize(reqAngleMeshSegs * 4, reqAngleMeshSegs * 2);
		while (direction > 0.0f ? angle < mCurrentRotateAngle : angle > mCurrentRotateAngle)
		{
			float angleNext{};

			Ref<Vec2F> pinside = mmake<Vec2F>(Vec2F::Rotated(angle) * mRotateRingInsideRadius + screenPos);
			Ref<Vec2F> poutside = mmake<Vec2F>(Vec2F::Rotated(angle) * mRotateRingOutsideRadius + screenPos);
			Ref<Vec2F> pinsideNext = mmake<Vec2F>(Vec2F::Rotated(angleNext) * mRotateRingInsideRadius + screenPos);
			Ref<Vec2F> poutsideNext = mmake<Vec2F>(Vec2F::Rotated(angleNext) * mRotateRingOutsideRadius + screenPos);

			int vi = i * 4;
			int pi = i * 6;
			i++;

			ULong currFillColor = fillColorUL;
			if (angle < Math::PI() * 0.5f || (angle >= Math::PI() - FLT_EPSILON && angle <= Math::PI() * 1.5f + FLT_EPSILON))
				currFillColor = fillColorUL2;

			mRotateRingFillMesh->vertices[vi] = Vertex(pinside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 1] = Vertex(poutside, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 2] = Vertex(pinsideNext, currFillColor, 0.0f, 0.0f);
			mRotateRingFillMesh->vertices[vi + 3] = Vertex(poutsideNext, currFillColor, 0.0f, 0.0f);

			mRotateRingFillMesh->indexes[pi] = vi;
			mRotateRingFillMesh->indexes[pi + 1] = vi + 1;
			mRotateRingFillMesh->indexes[pi + 2] = vi + 3;

			mRotateRingFillMesh->indexes[pi + 3] = vi;
			mRotateRingFillMesh->indexes[pi + 4] = vi + 3;
			mRotateRingFillMesh->indexes[pi + 5] = vi + 2;

			angle = angleNext;
		}
	}

	void RotateTool::OnPivotDragHandleMoved(const Vec2F& newPos)
	{
		Vec2F delta = newPos - mMoveStartPos;
		if (Math::AlmostZero(delta.Length()))
			return;

		UpdateRotate(delta);
		mMoveStartPos = newPos;
	}

	void RotateTool::UpdateRotate(const Vec2F& delta)
	{
		for (const Ref<GameObject>& obj : o2EditorSceneScreen.GetSelectedObjects())
		{
			TransformModifyAction* transAction = GET_EDITOR_ACTION_TYPES(TransformModifyAction)
				.Find([&](const TransformModifyAction* curAction)
					  {
						  return curAction->gameObject == obj.get();
					  });

			ApplyRotate(obj, obj->GetPivot(), delta, AffineTransform::scale(1.0f));
			if (transAction)
				transAction->updated = true;
		}
	}

	void RotateTool::ApplyRotate(Ref<GameObject>& obj, const Vec2F& pivot, const Vec2F& delta,
								 const AffineTransform& baseTransform /*= AffineTransform::scale(1.0f)*/)
	{
		Vec2F pivotLocalPos = obj->WorldToLocalPoint(pivot);

		AffineTransform rotateTransform = AffineTransform::translate(pivotLocalPos.x, pivotLocalPos.y) *
										  AffineTransform::rotate(Math::RadToDeg(delta.Angle())) *
										  AffineTransform::translate(-#include <memory>

using namespace std;

template<typename T>
using Ref = shared_ptr<T>;

template<typename T>
using WeakRef = weak_ptr<T>;

template<typename T, typename... Args>
inline Ref<T> mmake(Args&&... args) {
    return make_shared<T>(forward<Args>(args)...);
}

template<typename T>
inline T& operator<<(T& os, const Ref<T>& p) {
    os << *p;
    return os;
}

template<typename DstType, typename SrcType>
inline Ref<DstType> DynamicCast(const Ref<SrcType>& p) {
    return dynamic_pointer_cast<DstType>(p);
}


replace raw pointers with Ref<>, except void* pointers. 
void* pointers = unchanged

Ref<Mesh> mAngleMesh;
Ref<TransformAction> mTransformAction;
#include <Ref.h>
#include <WeakRef.h>

using Ref = SharedPtr;
using WeakRef = WeakPtr;

/************************** Utility Functions **************************/

template<typename T, typename... Args>
Ref<T> mmake(Args&&... args)
{
	return Ref<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
Ref<T> DynamicCast(const Ref<U>& ptr)
{
	return Ref<T>::DynamicCast(ptr);
}

/************************** Class Definition **************************/

namespace Editor {
	class RotateTool {
	private:
		Ref<TransformAction> mTransformAction;
		Array<Basis> mBeforeTransforms;
		
		void RotateObjects(const float& angleDelta);
		void RotateObjectsSeparated(const float& angleDelta);
		void RotateObjectsWithAction(const float& angleDelta);
		void RotateObjectsSeparatedWithAction(const float& angleDelta);

	public:
		void HandleKeyPress(const int& key);
	};

	/************************** Implementation **************************/
	
	void RotateTool::HandleKeyPress(const int& key)
	{
		const float angle = 1.0f;

		if (key == VK_LEFT || key == VK_DOWN)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateObjectsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateObjectsWithAction(Math::Deg2rad(-angle));
		}

		if (key == VK_RIGHT || key == VK_UP)
		{
			if (o2Input.IsKeyDown(VK_CONTROL)) RotateObjectsSeparatedWithAction(Math::Deg2rad(-angle));
			else RotateObjectsWithAction(Math::Deg2rad(angle));
		}
	}

	void RotateTool::RotateObjects(const float& angleDelta)
	{
		const Basis transform = Basis::Translated(mScenePivot * -1.0f) * Basis::Rotated(-angleDelta) * Basis::Translated(mScenePivot);
		for (const auto& objectRef : o2EditorSceneScreen.GetTopSelectedObjects())
		{
			auto object = objectRef.Get();
			object->SetTransform(object->GetTransform() * transform);
			object->UpdateTransform();
		}
	}

	void RotateTool::RotateObjectsSeparated(const float& angleDelta)
	{
		const Basis transform = Basis::Rotated(-angleDelta);
		for (const auto& objectRef : o2EditorSceneScreen.GetTopSelectedObjects())
		{
			auto object = objectRef.Get();
			object->SetTransform(object->GetTransform() * transform);
			object->UpdateTransform();
		}
	}

	void RotateTool::RotateObjectsWithAction(const float& angleDelta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Array<Basis>>(
			[](const Ref<SceneEditableObject>& x) { return x->GetTransform(); });

		mTransformAction = mmake<TransformAction>(o2EditorSceneScreen.GetTopSelectedObjects());

		RotateObjects(angleDelta);

		mTransformAction->Completed();
		o2EditorApplication.DoneAction(mTransformAction);
		mTransformAction = nullptr;
	}

	void RotateTool::RotateObjectsSeparatedWithAction(const float& angleDelta)
	{
		mBeforeTransforms = o2EditorSceneScreen.GetTopSelectedObjects().Convert<Array<Basis>>(
			[](const Ref<SceneEditableObject>& x) { return x->GetTransform(); });

		mTransformAction = mmake<TransformAction>(o2EditorSceneScreen.GetTopSelectedObjects());

		RotateObjectsSeparated(angleDelta);

		mTransformAction->Completed();
		o2EditorApplication.DoneAction(mTransformAction);
		mTransformAction = nullptr;
	}
}

/************************** Meta **************************/

DECLARE_CLASS(Editor::RotateTool, Editor__RotateTool);

// --- END META ---