#pragma once
#include "o2/Scene/SceneLayer.h"

namespace o2
{
	// -----------------
	// Scene layers list
	// -----------------
	class SceneLayersList: public ISerializable
	{
	public:
		// Default constructor
		SceneLayersList();

		// Constructor from layers list
		SceneLayersList(const Vector<SceneLayer*>& layers);

		// Constructor from layers list
		SceneLayersList(const Vector<String>& layers);

		// Copy-constructor
		SceneLayersList(const SceneLayersList& other);

		// Equals operator
		SceneLayersList& operator=(const SceneLayersList& other);

		// Check equals operator
		bool operator==(const SceneLayersList& other) const;

		// Check not equals operator
		bool operator!=(const SceneLayersList& other) const;

		// Checks has layer
		bool HasLayer(SceneLayer* layer) const;

		// Checks has layer
		bool HasLayer(const String& layerName) const;

		// Adds layer
		void AddLayer(SceneLayer* layer);

		// Adds layer
		void AddLayer(const String& layerName);

		// Removes layer
		void RemoveLayer(SceneLayer* layer);

		// Removes layer
		void RemoveLayer(const String& layerName);

		// Sets layers list
		void SetLayers(const Vector<SceneLayer*>& layers);

		// Sets layers list
		void SetLayers(const Vector<String>& layerNames);

		// Sets all exisitng layers. After adding new layers will contain all new layers
		void SetAllLayers();

		// Returns layers list
		Vector<SceneLayer*> GetLayers() const;

		// Returns layers list
		const Vector<String>& GetLayersNames() const;

		SERIALIZABLE(SceneLayersList);

	protected:
		bool mAllLayers = true; // When this is true, the list is always returns all existing layers

		mutable Vector<String> mLayers; // Scene layers names list @SERIALIZABLE
	};
}

CLASS_BASES_META(o2::SceneLayersList)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::SceneLayersList)
{
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mAllLayers);
	FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mLayers);
}
END_META;
CLASS_METHODS_META(o2::SceneLayersList)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<SceneLayer*>&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<String>&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const SceneLayersList&);
	FUNCTION().PUBLIC().SIGNATURE(bool, HasLayer, SceneLayer*);
	FUNCTION().PUBLIC().SIGNATURE(bool, HasLayer, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, AddLayer, SceneLayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, AddLayer, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveLayer, SceneLayer*);
	FUNCTION().PUBLIC().SIGNATURE(void, RemoveLayer, const String&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayers, const Vector<SceneLayer*>&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetLayers, const Vector<String>&);
	FUNCTION().PUBLIC().SIGNATURE(void, SetAllLayers);
	FUNCTION().PUBLIC().SIGNATURE(Vector<SceneLayer*>, GetLayers);
	FUNCTION().PUBLIC().SIGNATURE(const Vector<String>&, GetLayersNames);
}
END_META;
