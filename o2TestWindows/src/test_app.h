#ifndef TEST_APP_H
#define TEST_APP_H

#include "app/application.h"
#include "util/singleton.h"
#include "util/math/basis.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

class cManField;
class cStretchRect;

/** Тестовое приложение. Обрабатывает ввод, выводит графику и т.п. */
class TestApp:public cApplication, public cSingleton<TestApp>
{
	cManField*    mManField; /** Указатель на экземпляр обекта мужикополя. */

	vec2f         mLastCursorPos;
	cStretchRect* mStretchRect;
	bool          mDragRect;
	bool          mResizeRect;

public:	
	/** констр. */
	TestApp();

	/** дестр. */
	~TestApp();

	/** Вызывается при обновлении кадра. */
	void onUpdate(float dt);

	/** Вызывается при отрисовке кадра. */
	void onDraw();

	/** Обрабатывает сообщения приложения. */
	void processMessage(cApplacationMessage::type message);

public:
	/** Вызывается в сообщении инициализации приложения. */
	void onInitialized();
};

CLOSE_O2_NAMESPACE

#endif //TEST_APP_H