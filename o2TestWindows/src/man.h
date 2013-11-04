#ifndef MAN_H
#define MAN_H

#include "public.h"

#include <vector>
#include <map>

#include "util/xml_tools.h"
#include "util/math/rect.h"

#include "path_web.h"

OPEN_O2_NAMESPACE

class grSprite;
class cManField;

/** Мужик. Содержит текущую позицию в кластерах, анимацию ходьбы и поис пути. */
class cMan
{
	/** Анимация пути. */
	struct WalkAnimation
	{
		typedef std::vector<fRect> RectsList;
		typedef std::map<std::string, vec2i> AnimRangesVec;
		
		grSprite*     mAnimatingSprite; /**< Указатель на анимируемый спрайт. */
		RectsList     mFrames;          /**< Массив фреймов. */
		float         mFramesDelay;     /**< Время между фреймами. */
		AnimRangesVec mAnimations;      /**< Список анимаций. */

		float         mCurrTime;        /**< Текущее время анимации. */
		int           mCurrAnim[2];     /**< Текущий стартовый и конечный кадр анимации. */

		bool          mAnimating;       /**< Истина, если анимация Включена. */

		/** констр. */
		WalkAnimation(grSprite* animatingSprite, pugi::xml_node& xmlNode);

		/** Обновление кадра анимации, обновление анимируемого спрайта. */
		void update(float dt);

		/** Включает проигрывание анимации по названию. */
		void setPlayingAnim(const std::string& id);
	};

	/** Движение мужика. */
	struct WalkPath
	{
		WayPointsVec mWaypoints;       /**< Список точек движения. */
		float        mMovingTime;      /**< Время движения в проходимом waypoint'e .*/
		int          mCurrentWaypoint; /**< Индекс текущего проходимого waypoint'a. */
		cMan*        mMan;             /**< Указател  на идущего мужика. */
		bool         mMoving;          /**< Истина, если мужик в движении. */

		/* констр. */
		WalkPath(cMan* man);

		/** Устанавливает точку, в которую необходимо двигаться. Рассчитывает путь и приступает к движению. */
		void setupWaypoint(const vec2f& point);

		/** Обновление движения. */
		void update(float dt);
	};

	vec2f          mPosition;     /**< Позиция мужика, в клетках. */
	vec2f          mSpriteOffset; /**< Смещение спрайта, необходимо для выравнивания изображения на центр клетки поля. */
	grSprite*      mSprite;       /**< Спрайт для анимации/отрисовки. */
	cManField*     mManField;     /**< Указател на поле. */
	WalkAnimation* mWalkAnim;     /**< Анимация ходьбы. */
	WalkPath*      mWalkPath;     /**< Объект для передвижения мужика. */

public:
	/** констр. */
	cMan(cManField* manField, pugi::xml_node& xmlNode);

	/** дестр. */
	~cMan();

	/** отрисовка. */
	void draw();

	/** Обновление. */
	void update(float dt);

	/** Устанавиливает точку движения. */
	void setWayPoint(const vec2f& point);
};

CLOSE_O2_NAMESPACE

#endif //MAN_H