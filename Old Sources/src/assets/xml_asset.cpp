#include "xml_asset.h"

#include "app/application.h"
#include "assets.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(XmlAsset);

XmlAsset::XmlAsset():
Asset()
{
}

XmlAsset::XmlAsset(const String& location):
Asset(location)
{
	LoadData();
}

XmlAsset::XmlAsset(const FileLocation& location):
Asset(location)
{
	LoadData();
}

XmlAsset::~XmlAsset()
{
}

XmlAsset& XmlAsset::operator=(const XmlAsset& asset)
{
	return *this;
}

pugi::xml_document& XmlAsset::GetXmlDoc()
{
	return mXmlDoc;
}

void XmlAsset::SaveData()
{
	XmlTools::SaveToFile(AppAssets()->GetAssetSourceFullPath(mLocation.mPath), mXmlDoc);
}

void XmlAsset::LoadData()
{
	if (!XmlTools::LoadFromFile(AppAssets()->GetAssetFullPath(mLocation.mPath), mXmlDoc))
	{
		AppAssets()->mLog->Error("Filed to load xml asset: %s", mLocation.mPath.c_str());
	}
}

CLOSE_O2_NAMESPACE