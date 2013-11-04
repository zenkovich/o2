#ifndef MAN_FIELD_H
#define MAN_FIELD_H

#include <vector>

#include "util/math/vector2.h"
#include "render_system/camera.h"

OPEN_O2_NAMESPACE

class grMesh;
class cInputMessage;
class cApplication;
class grTexture;

class cMan;
struct cWaypointWeb;

/** Мужикополе. Хранит клетки поля, размер поля, размер клетки, сетку возможных путей прохода, объект мужика. */
class cManField
{
	friend class cMan;

	/** Кластер поля. Хранит в себе координаты кластера и время, необходимое для прохождения этого кластера. */
	struct cFieldCluster
	{
		vec2i     mPosition; /**< Координаты. */ 
		float     mWayTime;  /**< Время прохождения. */
	};
	typedef std::vector<cFieldCluster> FieldClustersVec;
	typedef std::vector<grMesh*> MeshVec;

	cApplication*    mApplication;   /**< Указатель на приложение. */
	cInputMessage*   mInputMessage;  /**< Указатель на структуру ввода. */

	vec2i            mFieldSize;     /**< Размер поля в кластерах. */
	vec2f            mClusterSize;   /**< Размер кластера в пикселях. */
	FieldClustersVec mFieldClusters; /**< Вектор кластеров поля. */

	MeshVec          mFieldMeshes;   /**< Вектор мешей, используемых для отрисовки поля. */

	cWaypointWeb*    mWaypointWeb;   /**< Сеть путей проходов. */
	cMan*            mMan;           /**< Это мужик. */

public:
	/** констр. */
	cManField(cApplication* app);

	/** дестр. */
	~cManField();

	/** Отрисовка поля и мужика. */
	void draw();

	/** Обновление мужика и управления им. */
	void update(float dt);

protected:
	/** Загружает данные и конфиг-файла. */
	void loadConfig();

	/** Инициализирует сеть путей. */
	void initializeWaypointWeb();

	/** Создает меш под заданное кол-во кластеров. */
	grMesh* createMesh(int clustersCount, grTexture* texture);
};

CLOSE_O2_NAMESPACE

#endif //MAN_FIELD_H