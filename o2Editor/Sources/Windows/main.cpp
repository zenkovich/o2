#include "stdafx.h"
#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"

#include "Scene/Actor.h"
#include "Render/Camera.h"
#include "Utils/Timer.h"

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

template<typename T>
class isetter
{
public:
	virtual void Set(const T& value) = 0;
};

template<typename T>
class igetter
{
public:
	virtual T Get() const = 0;
};

template<typename T>
class iproperty: public isetter<T>, public igetter<T>
{
public:
};

template <typename T>
class has_i_am_property
{
	typedef char one;
	typedef long two;

	template <typename C> static one test(decltype(&C::i_am_property));
	template <typename C> static two test(...);

public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

#define _property(classname, type, name, setter, getter)                                                 \
	class name##_property																				 \
	{																									 \
		classname* _this;																				 \
																										 \
	public:																								 \
		name##_property(classname* _this):_this(_this) {}												 \
		operator type() { return _this->getter(); }												     \
		type operator=(type value) { _this->setter(value); return _this->getter(); }	     \
	};																									 \
																										 \
	name##_property name = name##_property(this);	

#define _setter(classname, type, name, setter)                                                 \
	class name##_setter																				 \
	{																									 \
		classname* _this;																				 \
																										 \
	public:																								 \
		name##_setter(classname* _this):_this(_this) {}												 \
		name##_setter& operator=(const type& value) { _this->setter(value); return *this; }	     \
	};																									 \
																										 \
	name##_setter name = name##_setter(this);	

#define _getter(classname, type, name, getter)                                                 \
	class name##_getter																				 \
	{																									 \
		classname* _this;																				 \
																										 \
	public:																								 \
		name##_getter(classname* _this):_this(_this) {}												 \
		operator type() { return _this->getter(); }												     \
	};																									 \
																										 \
	name##_getter name = name##_getter(this);	

class ActorTransformx
{
public:
	_getter(ActorTransformx, Actor*, actor, GetOwnerActor);

	_property(ActorTransformx, Vec2F, position, SetPosition, GetPosition);   
	_property(ActorTransformx, Vec2F, size, SetSize, GetSize);       
	_property(ActorTransformx, float, width, SetWidth, GetWidth);      
	_property(ActorTransformx, float, height, SetHeight, GetHeight);         
	_property(ActorTransformx, Vec2F, scale, SetScale, GetScale);          
	_property(ActorTransformx, Vec2F, pivot, SetPivot, GetPivot);          
	_property(ActorTransformx, Vec2F, szPivot, SetSizePivot, GetSizePivot);        
	_property(ActorTransformx, float, angle, SetAngle, GetAngle);          
	_property(ActorTransformx, float, angleDegree, SetAngleDegrees, GetAngleDegrees);    
	_property(ActorTransformx, float, shear, SetShear, GetShear);          

	_property(ActorTransformx, Basis, basis, SetBasis, GetBasis);    
	_property(ActorTransformx, Basis, nonSizedBasis, SetNonSizedBasis, GetNonSizedBasis);    
	_property(ActorTransformx, RectF, AABB, SetAxisAlignedRect, GetAxisAlignedRect);    
	_property(ActorTransformx, RectF, rect, SetRect, GetRect);    
	_property(ActorTransformx, Vec2F, leftTop, SetLeftTop, GetLeftTop);    
	_property(ActorTransformx, Vec2F, leftBottom, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, rightTop, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, rightBottom, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, center, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, rightDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, leftDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, upDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, downDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, float, right, SetLeft, GetLeft);    
	_property(ActorTransformx, float, left, SetLeft, GetLeft);
	_property(ActorTransformx, float, top, SetLeft, GetLeft);
	_property(ActorTransformx, float, bottom, SetLeft, GetLeft);
	_property(ActorTransformx, Vec2F, worldPosition, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldPivot, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, float, worldAngle, SetLeft, GetLeft);
	_property(ActorTransformx, float, worldAngleDegree, SetLeft, GetLeft);
	_property(ActorTransformx, Basis, worldBasis, SetWorldBasis, GetWorldBasis);    
	_property(ActorTransformx, Basis, worldNonSizedBasis, SetWorldBasis, GetWorldBasis);
	_property(ActorTransformx, Vec2F, worldLeftTop, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldLeftBottom, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldRightTop, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldRightBottom, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldCenter, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldRightDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldLeftDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldUpDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, Vec2F, worldDownDir, SetLeftTop, GetLeftTop);
	_property(ActorTransformx, float, worldRight, SetLeft, GetLeft);
	_property(ActorTransformx, float, worldLeft, SetLeft, GetLeft);
	_property(ActorTransformx, float, worldTop, SetLeft, GetLeft);
	_property(ActorTransformx, float, worldBottom, SetLeft, GetLeft);
	_property(ActorTransformx, RectF, worldRect, SetRect, GetRect);
	_property(ActorTransformx, RectF, worldAABB, SetRect, GetRect);


	// Returns owner actor
	Actor* GetOwnerActor() const { return nullptr; }

	// Sets position
	virtual void SetPosition(const Vec2F& position) { mData->position = position; }

	// Returns position
	Vec2F GetPosition() const { return Vec2F(); }

	// Sets size
	virtual void SetSize(const Vec2F& size) {}

	// Return size
	Vec2F GetSize() const { return Vec2F(); }

	// Sets width
	void SetWidth(float value) {}

	// Return width
	float GetWidth() const { return 0; }

	// Sets height
	void SetHeight(float value) {}

	// Return height
	float GetHeight() const { return 0; }

	// Sets pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
	virtual void SetPivot(const Vec2F& pivot) {}

	// Return pivot, in local space, where (0, 0) - left down corner, (1; 1) - right top
	Vec2F GetPivot() const { return Vec2F(); }

	// Sets size pivot, in local space, where (0, 0) - left down corner, (size.x, size.y) - right top
	void SetSizePivot(const Vec2F& relPivot) {}

	// Returns size pivot, in local space, where (0, 0) - left down corner, (size.x, size.y) - right top
	Vec2F GetSizePivot() const { return Vec2F(); }

	// Sets scale
	void SetScale(const Vec2F& scale) {}

	// Returns scale
	Vec2F GetScale() const { return Vec2F(); }

	// Sets rotation angle, in radians
	void SetAngle(float rad) {}

	// Returns rotation angle in radians
	float GetAngle() const { return 0; }

	// Sets rotation angle, in degrees
	void SetAngleDegrees(float deg) {}

	// Returns rotation angle in degrees
	float GetAngleDegrees() const { return 0; }

	// Sets shear
	void SetShear(float shear) {}

	// Returns shear
	float GetShear() const { return 0; }

	// Sets basis
	virtual void SetBasis(const Basis& basis) {}

	// Returns basis
	Basis GetBasis() const { return Basis(); }

	// Sets basis without size
	virtual void SetNonSizedBasis(const Basis& basis) {}

	// Returns basis without size
	Basis GetNonSizedBasis() const { return Basis(); }

	// Sets rect
	virtual void SetRect(const RectF& rect) {}

	// Returns rect
	virtual RectF GetRect() const { return RectF(); }

	// Sets direction aligned rectangle transformation
	virtual void SetAxisAlignedRect(const RectF& rect) {}

	// Returns direction aligned rectangle transformation
	RectF GetAxisAlignedRect() const { return RectF(); }

	// Sets left top corner position
	void SetLeftTop(const Vec2F& position) {}

	// Returns left top corner position
	Vec2F GetLeftTop() const { return Vec2F(); }

	// Sets right top corner position
	void SetRightTop(const Vec2F& position) {}

	// Returns right top corner position
	Vec2F GetRightTop() const { return Vec2F(); }

	// Sets left down corner position
	void SetLeftBottom(const Vec2F& position) {}

	// Returns left down corner position
	Vec2F GetLeftBottom() const { return Vec2F(); }

	// Sets left right bottom position
	void SetRightBottom(const Vec2F& position) {}

	// Returns right bottom corner position
	Vec2F GetRightBottom() const { return Vec2F(); }

	// Sets center position
	void SetCenter(const Vec2F& position) {}

	// Returns center position
	Vec2F GetCenter() const { return Vec2F(); }

	// Set local right direction
	void SetRightDir(const Vec2F& dir) {}

	// Returns local right direction
	Vec2F GetRightDir() const { return Vec2F(); }

	// Set local left direction
	void SetLeftDir(const Vec2F& dir) {}

	// Returns local left direction
	Vec2F GetLeftDir() const { return Vec2F(); }

	// Set local up direction
	void SetUpDir(const Vec2F& dir) {}

	// Returns local up direction
	Vec2F GetUpDir() const { return Vec2F(); }

	// Set local down direction
	void SetDownDir(const Vec2F& dir) {}

	// Returns local down direction
	Vec2F GetDownDir() const { return Vec2F(); }

	// Set local right border position
	void SetRight(float value) {}

	// Returns local right border position
	float GetRight() const { return 0; }

	// Set local left border position
	void SetLeft(float value) {}

	// Returns local left border position
	float GetLeft() const { return 0; }

	// Set local top border position
	void SetTop(float value) {}

	// Returns local top border position
	float GetTop() const { return 0; }

	// Set local down border position
	void SetBottom(float value) {}

	// Returns local down border position
	float GetBottom() const { return 0; }

	// Transforms point from world space into local
	Vec2F World2LocalPoint(const Vec2F& worldPoint) const { return Vec2F(); }

	// Transforms point from local space into world
	Vec2F Local2WorldPoint(const Vec2F& localPoint) const { return Vec2F(); }

	// Transforms direction from world space into local
	Vec2F World2LocalDir(const Vec2F& worldDir) const { return Vec2F(); }

	// Transforms direction from local space into world
	Vec2F Local2WorldDir(const Vec2F& localDir) const { return Vec2F(); }

	// Returns true when point inside this
	bool IsPointInside(const Vec2F& point) const { return false; }

	// Sets world position
	void SetWorldPosition(const Vec2F& position) {}

	// Returns world position
	Vec2F GetWorldPosition() const { return Vec2F(); }

	// Sets pivot by world coordinates
	void SetWorldPivot(const Vec2F& pivot) {}

	// Returns pivot position in world coordinates
	Vec2F GetWorldPivot() const { return Vec2F(); }

	// Sets world rotation angle, in radians
	void SetWorldAngle(float rad) {}

	// Returns world rotation angle in radians
	float GetWorldAngle() const { return 0; }

	// Sets world rotation angle, in degrees
	void SetWorldAngleDegree(float deg) {}

	// Returns world rotation angle in degrees
	float GetWorldAngleDegree() const { return 0; }

	// Sets world basis
	void SetWorldBasis(const Basis& basis) {}

	// Returns world basis
	Basis GetWorldBasis() const { return Basis(); }

	// Sets world basis without size
	void SetWorldNonSizedBasis(const Basis& basis) {}

	// Returns world basis without size
	Basis GetWorldNonSizedBasis() const { return Basis(); }

	// Sets world rect
	void SetWorldRect(const RectF& rect) {}

	// Returns world rect
	RectF GetWorldRect() const { return RectF(); }

	// Sets world direction aligned rectangle transformation
	void SetWorldAxisAlignedRect(const RectF& rect) {}

	// Returns world direction aligned rectangle transformation
	RectF GetWorldAxisAlignedRect() const { return RectF(); }

	// Sets world left top corner position
	void SetWorldLeftTop(const Vec2F& position) {}

	// Returns world left top corner position
	Vec2F GetWorldLeftTop() const { return Vec2F(); }

	// Sets world right top corner position
	void SetWorldRightTop(const Vec2F& position) {}

	// Returns world right top corner position
	Vec2F GetWorldRightTop() const { return Vec2F(); }

	// Sets world left down corner position
	void SetWorldLeftBottom(const Vec2F& position) {}

	// Returns world left down corner position
	Vec2F GetWorldLeftBottom() const { return Vec2F(); }

	// Sets world left right bottom position
	void SetWorldRightBottom(const Vec2F& position) {}

	// Returns world right bottom corner position
	Vec2F GetWorldRightBottom() const { return Vec2F(); }

	// Sets world center position
	void SetWorldCenter(const Vec2F& position) {}

	// Returns world center position
	Vec2F GetWorldCenter() const { return Vec2F(); }

	// Set World right direction
	void SetWorldRightDir(const Vec2F& dir) {}

	// Returns World right direction
	Vec2F GetWorldRightDir() const { return Vec2F(); }

	// Set World left direction
	void SetWorldLeftDir(const Vec2F& dir) {}

	// Returns World left direction
	Vec2F GetWorldLeftDir() const { return Vec2F(); }

	// Set World up direction
	void SetWorldUpDir(const Vec2F& dir) {}

	// Returns World up direction
	Vec2F GetWorldUpDir() const { return Vec2F(); }

	// Set World down direction
	void SetWorldDownDir(const Vec2F& dir) {}

	// Returns World down direction
	Vec2F GetWorldDownDir() const { return Vec2F(); }

	// Set World right border position
	void SetWorldRight(float value) {}

	// Returns World right border position
	float GetWorldRight() const { return 0; }

	// Set World left border position
	void SetWorldLeft(float value) {}

	// Returns World left border position
	float GetWorldLeft() const { return 0; }

	// Set World top border position
	void SetWorldTop(float value) {}

	// Returns World top border position
	float GetWorldTop() const { return 0; }

	// Set World down border position
	void SetWorldBottom(float value) {}

	// Returns World down border position
	float GetWorldBottom() const { return 0; }

public:
	class Data
	{
	public:
		int    dirtyFrame = 1;                     // Frame index, when layout was marked as dirty
		int    updateFrame = 1;                    // Frame index, when layout was updated

		Vec2F  position;                           // Position @SERIALIZABLE
		Vec2F  size;                               // Size @SERIALIZABLE
		Vec2F  scale = Vec2F(1, 1);                // Scale, (1; 1) is default @SERIALIZABLE
		Vec2F  pivot;                              // Pivot: (0; 0) is left bottom corner - (1; 1) is right top corner @SERIALIZABLE
		float  angle = 0;                          // Rotation angle in radians @SERIALIZABLE
		float  shear = 0;                          // Shear @SERIALIZABLE

		RectF  rectangle;                          // The rectangle in local space
		RectF  parentRectangle;                    // The parent rectangle
		Vec2F  parentRectangePosition;             // The parent rectangle pivot position
		RectF  worldRectangle;                     // The rectangle in world space

		Basis  transform;                          // Final transform basis
		Basis  nonSizedTransform;                  // Final transform basis without size

		Basis  worldNonSizedTransform;             // World transform without size
		Basis  worldTransform;                     // Result world basis

		Basis  parentInvertedTransform;            // Parent world transform inverted
		Basis  parentTransform;                    // Parent world transform
		int    parentInvTransformActualFrame;      // last mParentInvertedTransform actual frame index

		Actor* owner = nullptr;                    // Owner actor @EXCLUDE_POINTER_SEARCH
	};

protected:
	Data* mData = new Data();
};

template<typename T>
struct Test
{
	T val;
public:
	Test() {}
	Test(T value): val(value) {}

	operator T() { return val; }
	Test<T>& operator=(T other) { val = other; return this; }
	T operator+(const T& v) const { return val + v; }
	Test<T>& operator+=(const T& v) { val = val + v; return *this; }
	bool operator==(const T& v) const { return val == v; }
};

template<typename T>
struct Test2
{
	T val;
public:
	Test2() {}
	Test2(T value): val(value) {}

	operator T() { return val; }
	Test2<T>& operator=(T other) { val = other; return this; }
	T operator+(const T& v) const { return val + v; }
	Test2<T>& operator+=(const T& v) { val = val + v; return *this; }
	bool operator==(const T& v) const { return val == v; }
};

struct vv
{
	int x, y;

	vv() {}

	vv(int x, int y):x(x), y(y) {}

	bool operator==(const vv& other) const { return x == other.x; }

	vv operator+(const vv& other) const { vv res; res.x = x + other.x; res.y = y + other.y; return res; }
};

int main()
{
	INITIALIZE_O2;

	vv v1(1, 2), v2(3, 4);
	vv v3 = v1 + v2;

	Test<vv> xx(vv(1, 2));
	Test2<vv> yy(vv(2, 2));

	Test<vv> zz = xx + yy;
	Test2<vv> ww(vv(3, 3));
	zz += ww;

	bool b = xx == yy;

	int iters = 1000;

	ActorTransformx ff;
	bool isProp = has_i_am_property<decltype(ff.position)>::value;

	Timer t1;
	for (int i = 0; i < iters; i++)
	{
		ActorTransformx* xx = new ActorTransformx();
		xx->position = Vec2F(10, 10);
		xx->width = xx->width + 10;
	}
	printf("%f %i\n", t1.GetTime(), sizeof(ActorTransformx));

	Timer t2;
	for (int i = 0; i < iters; i++)
	{
		ActorTransform* xx = new ActorTransform();
		xx->position = Vec2F(10, 10);
	}
	printf("%f %i\n", t2.GetTime(), sizeof(ActorTransform));

	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;

	return 0;
}

