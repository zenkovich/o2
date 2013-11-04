#include "path_web.h"

#include <float.h>

#include "util/math/math.h"

OPEN_O2_NAMESPACE
	
int cWaypointWeb::getNearestWaypoint( const vec2f& point ) const
{
	float minDist = FLT_MAX;
	int res = 0;
	int idx = 0;
	for (WaypointsVec::const_iterator it = mWaypoints.cbegin(); it != mWaypoints.cend(); ++it, idx++)
	{
		float dist = (it->mPoint - point).len();
		if (dist < minDist)
		{
			res = idx;
			minDist = dist;
		}
	}

	return res;
}

bool dijkstraPathSearch( cWaypointWeb& web, WayPointsVec& path, int beginIdx, int endIdx )
{
	path.clear();

	//сброс параметров сетки
	for (cWaypointWeb::WaypointsVec::iterator it = web.mWaypoints.begin(); it != web.mWaypoints.end(); ++it)
	{
		it->mProcessed = false;
		it->mPathG = FLT_MAX;
	}

	//назначаем начальной точке оценку 0
	web.mWaypoints[beginIdx].mPathG = 0;
	web.mWaypoints[beginIdx].mG = 0;
	web.mWaypoints[beginIdx].mParent = &web.mWaypoints[beginIdx];

	//главный цикл алгоритма
	bool stop = false;
	while (!stop)
	{
		//ищем точку с минимальной суммарной оценкой
		cWaypointWeb::waypoint* currentPoint = NULL;
		float minPathG = FLT_MAX;
		for (cWaypointWeb::WaypointsVec::iterator it = web.mWaypoints.begin(); it != web.mWaypoints.end(); ++it)
		{
			if (!it->mProcessed && it->mPathG < minPathG)
			{
				minPathG = it->mPathG;
				currentPoint = &(*it);
			}
		}

		//если така€ точка не найдена, т.е. все вершины пройдены, алгоритм завершает работу с отрицательным результатом
		if (!currentPoint)
			return false;

		//проходимс€ по св€з€м узла
		for (cWaypointWeb::LinksVec::iterator jt = currentPoint->mLinks.begin(); jt != currentPoint->mLinks.end(); ++jt)
		{
			cWaypointWeb::waypoint* linkPoint = &web.mWaypoints[jt->mLinkWPId];

			if (!linkPoint->mProcessed && currentPoint->mPathG + jt->G < linkPoint->mPathG)
			{
				linkPoint->mPathG = currentPoint->mPathG + jt->G;
				linkPoint->mParent = currentPoint;
				linkPoint->mG = jt->G;

				if (jt->mLinkWPId == endIdx)
				{
					stop = true;
					break;
				}
			}
		}

		currentPoint->mProcessed = true;
	}

	//заполн€ем путь, получаем перевернутый путь
	cWaypointWeb::waypoint* currentPoint = &web.mWaypoints[endIdx];
	while (true)
	{
		path.push_back(cWaypoint(currentPoint->mPoint, currentPoint->mG));

		if (currentPoint == currentPoint->mParent)
			break;

		currentPoint = currentPoint->mParent;
	}

	//переворачиваем
	const int pathSize = path.size();
	for (int i = 0; i < pathSize/2; i++)
	{
		tswap(path[i], path[pathSize - 1 - i]);
	}

	return true;
}

CLOSE_O2_NAMESPACE