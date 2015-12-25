#pragma once

#include "asset.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

class XmlAsset: public Asset
{
	DEFINE_TYPE(XmlAsset);

protected:
	pugi::xml_document mXmlDoc;

public:
	XmlAsset();
	XmlAsset(const String& location);
	XmlAsset(const FileLocation& location);
	~XmlAsset();

	pugi::xml_document& GetXmlDoc();

private:
	XmlAsset& operator=(const XmlAsset& asset);

protected:
	void LoadData();
	void SaveData();
};

CLOSE_O2_NAMESPACE
