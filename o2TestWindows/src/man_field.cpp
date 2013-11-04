#include "man_field.h"

//o2
#include "app/application.h"
#include "pugixml/pugixml.hpp"
#include "render_system/mesh.h"
#include "render_system/render_system.h"
#include "render_system/texture.h"
#include "util/log.h"
#include "util/math/color.h"
#include "util/math/math.h"
#include "util/string.h"
#include "util/xml_tools.h"

#include "man.h"
#include "path_web.h"

OPEN_O2_NAMESPACE
	
//cManField class

cManField::cManField(cApplication* app):
	mMan(NULL), mApplication(app), mWaypointWeb(NULL)
{
	mInputMessage = app->getInputMessage();
	loadConfig();
}

cManField::~cManField()
{
	safe_release(mMan);
	safe_release(mWaypointWeb);
	for (MeshVec::iterator it  = mFieldMeshes.begin(); it != mFieldMeshes.end(); ++it)
	{
		safe_release(*it);
	}
}

void cManField::loadConfig()
{
	const std::string configFileName = "config";

	//загр файла
	pugi::xml_document xmlDoc;
	if (!cXmlTools::loadFromFile(configFileName, xmlDoc))
	{
		llog("ERROR: Failed to load man field config file: %s", configFileName.c_str());
		return;
	}

	//загр текстуры
	std::string clusterTexFilename = xmlDoc.child("clusterTexture").attribute("file").value();
	grTexture* texture = mApplication->getRenderSystem()->createTexture(clusterTexFilename);

	//размер поля
	if (pugi::xml_node node = xmlDoc.child("fieldSize"))
		mFieldSize = cXmlTools::node2veci(node);
	else
		mFieldSize = vec2i(50, 50);	

	//размер кластера
	if (pugi::xml_node node = xmlDoc.child("clusterSize"))
		mClusterSize = cXmlTools::node2vecf(xmlDoc.child("clusterSize"));
	else
	{
		if (texture && texture->getSize().x > 1)
			mClusterSize = texture->getSize()/mFieldSize.castTo<float>();
		else
			mClusterSize = vec2f(10.0f, 10.0f);
	}

	//инициализация кластеров, заполнение мешей
	int reqClusters = mFieldSize.x*mFieldSize.y;
	int maxCreatingClusters = 1000;
	int creatingMeshClusters = min(maxCreatingClusters, reqClusters);
	reqClusters -= creatingMeshClusters;
	grMesh* currentMesh = createMesh(creatingMeshClusters, texture);
	int curInitClusters = 0;
	
	pugi::xml_node clustersNode = xmlDoc.child("clusters");
	
	const float wayTimeRandMin = 0.2f;
	const float wayTimeRandMax = 1.2f;
	
	float tuCluster = 1.0f/mFieldSize.x;
	float tvCluster = 1.0f/mFieldSize.y;
	for (int x = 0; x < mFieldSize.x; x++)
	{
		float tu = (float)x*tuCluster;
		for (int y = 0; y < mFieldSize.y; y++)
		{
			float tv = (float)y*tvCluster;


			cFieldCluster newCluster;

			pugi::xml_node curClusterNode = clustersNode.child(format("clust_%i_%i", x, y).c_str());
			if (curClusterNode)
				newCluster.mWayTime = curClusterNode.attribute("wayTime").as_float();
			else
				newCluster.mWayTime = random(wayTimeRandMin, wayTimeRandMax);
			newCluster.mPosition.set(x, y);

			mFieldClusters.push_back(newCluster);

			int v = curInitClusters*4;
			int p = curInitClusters*2*3;
			curInitClusters++;

			float colorCoef = clamp((newCluster.mWayTime - wayTimeRandMin)/(wayTimeRandMax - wayTimeRandMin), 0.0f, 1.0f)*0.5f + 0.5f;
			unsigned long colr = color4(colorCoef, colorCoef, colorCoef, colorCoef).dword();
			
			currentMesh->mVerticies[v]     = vertex2((float)x*mClusterSize.x,       (float)y*mClusterSize.y, colr,       tu,             tv);
			currentMesh->mVerticies[v + 1] = vertex2((float)(x + 1)*mClusterSize.x, (float)y*mClusterSize.y, colr,       tu + tuCluster, tv);
			currentMesh->mVerticies[v + 2] = vertex2((float)(x + 1)*mClusterSize.x, (float)(y + 1)*mClusterSize.y, colr, tu + tuCluster, tv + tvCluster);
			currentMesh->mVerticies[v + 3] = vertex2((float)x*mClusterSize.x,       (float)(y + 1)*mClusterSize.y, colr, tu,             tv + tvCluster);
			
			currentMesh->mIndexes[p]     = v; currentMesh->mIndexes[p + 1] = v + 1; currentMesh->mIndexes[p + 2] = v + 2;
			currentMesh->mIndexes[p + 3] = v; currentMesh->mIndexes[p + 4] = v + 2; currentMesh->mIndexes[p + 5] = v + 3;

			if (curInitClusters == creatingMeshClusters && reqClusters > 0)
			{
				creatingMeshClusters = min(maxCreatingClusters, reqClusters);
				reqClusters -= creatingMeshClusters;
				currentMesh = createMesh(creatingMeshClusters, texture);
				curInitClusters = 0;
			}
		}
	}

	mMan = mnew cMan(this, xmlDoc.child("walkMan"));

	initializeWaypointWeb();
}

void cManField::initializeWaypointWeb()
{
	mWaypointWeb = mnew cWaypointWeb;

	for (FieldClustersVec::iterator it = mFieldClusters.begin(); it != mFieldClusters.end(); ++it)
	{
		mWaypointWeb->mWaypoints.push_back( cWaypointWeb::waypoint( it->mPosition.castTo<float>() ) );
	}

	for (int x = 0; x < mFieldSize.x; x++)
	{
		for (int y = 0; y < mFieldSize.y; y++) 
		{
			int idx = x*mFieldSize.y + y;
			cFieldCluster* currCluster = &mFieldClusters[idx];
			cWaypointWeb::waypoint* currWayPoint = &mWaypointWeb->mWaypoints[idx];

			vec2i offs[4] = { vec2i(-1, 0), vec2i(1, 0), vec2i(0, -1), vec2i(0, 1) };

			for (int i = 0; i < 4; i++)
			{
				int ox = x + offs[i].x;
				int oy = y + offs[i].y;

				if (ox < 0 || oy < 0 || ox >= mFieldSize.x || oy >= mFieldSize.y)
					continue;
				
				int ofIdx = ox*mFieldSize.y + oy;
				cFieldCluster* offsCluster = &mFieldClusters[ofIdx];

				currWayPoint->mLinks.push_back(
					cWaypointWeb::link(ofIdx, (currCluster->mWayTime + offsCluster->mWayTime)*0.5f));
			}
		}
	}
}

grMesh* cManField::createMesh(int clustersCount, grTexture* texture)
{	
	grMesh* newMesh = mnew grMesh(mApplication->getRenderSystem(), texture, 
		                     4*clustersCount, 2*clustersCount);

	newMesh->mVertexCount = newMesh->getMaxVertexCount();
	newMesh->mPolyCount = newMesh->getMaxPolyCount();

	mFieldMeshes.push_back(newMesh);

	return newMesh;
}

void cManField::draw()
{
	for (MeshVec::iterator it  = mFieldMeshes.begin(); it != mFieldMeshes.end(); ++it)
	{
		(*it)->draw();
	}

	mMan->draw();
}

void cManField::update( float dt )
{
	mMan->update(dt);

	if (mInputMessage->isKeyPressed(VK_LBUTTON))
	{
		mMan->setWayPoint(mInputMessage->getCursorPos()/mClusterSize);
	}
}

CLOSE_O2_NAMESPACE
