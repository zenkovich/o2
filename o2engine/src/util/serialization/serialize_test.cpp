#include "serialization.h"

OPEN_O2_NAMESPACE

struct cTestSerializeObjA:public cSerializableObj
{
	int         a, b;
	float       c, d;
	std::string e, f;
	int         g[16];

	cTestSerializeObjA()
	{
		a = 1; b = 2;
		c = 3; d = 4;
		e = "e"; f = "fff";
		for (int i = 0; i < 16; i++)
			g[i] = i;
	}

	SERIALIZE_METHOD_DECL();
};

SERIALIZE_METHOD_IMPL(cTestSerializeObjA)
{
	SERIALIZE(a);
	SERIALIZE(b);
	SERIALIZE(c);
	SERIALIZE(d);
	SERIALIZE(e);
	SERIALIZE(f);
	SERIALIZE_ARR(g, 16);

	return true;
}

struct cTestSerializeObjB:public cTestSerializeObjA
{
	vec2i       v;
	std::string h;

	cTestSerializeObjB():cTestSerializeObjA()
	{
		v = vec2i(0, 0);
		h = "321";
	}

	SERIALIZE_INHERITED_METHOD_DECL(cTestSerializeObjA, cTestSerializeObjB);
};

SERIALIZE_INHERITED_METHOD_IMPL(cTestSerializeObjB)
{
	SERIALIZE(v);
	SERIALIZE(h);

	return true;
}

void testSerialization()
{
	pugi::xml_document doc;

	cTestSerializeObjB bb;

	bb.serialize(doc, cSerializeType::OUTPUT);

	cSerialization::saveData(doc, "../data/test_serialization.xml");

	cTestSerializeObjB bbCopy;

	pugi::xml_document rdoc;
	cSerialization::loadData(rdoc, "test_serialization", cFileType::FT_CONFIG);

	bbCopy.serialize(rdoc, cSerializeType::INPUT);
}

CLOSE_O2_NAMESPACE