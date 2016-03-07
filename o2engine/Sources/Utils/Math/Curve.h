#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Math/Math.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"
#include "Utils/Serializable.h"

namespace o2
{
	// ------------
	// Bezier curve
	// ------------
	class Curve: public ISerializable
	{
	public:
		class Key;
		typedef Vector<Key> KeysVec;

	public:
		Accessor<float, float> value;         // Value accessor by position
		Accessor<Key, float>   key;           // Key accessor by position
		Property<KeysVec>      keys;          // Keys property
		Getter<float>          length;        // Max position getter
		Function<void()>       onKeysChanged; // Keys changed event

		// Default constructor
		Curve();

		// Constructor by bezier coefficient in 0...1 range (some line as css-bezier)
		Curve(float beginCoef, float beginCoefPosition, float endCoef, float endCoefPosition);

		// Constructor from keys with smoothing
		Curve(Vector<Vec2F> values, float smooth = 1.0f);

		// Copy-constructor
		Curve(const Curve& other);

		// Assign operator
		Curve& operator=(const Curve& other);

		// Returns value by position
		float Evaluate(float position);

		// Adds keys with smoothing
		void AddKeys(Vector<Vec2F> values, float smooth = 1.0f);

		// Adds key
		void AddKey(const Key& key);

		// Adds key
		void AddKey(float position, float value, float leftCoef, float leftCoefPosition,
					float rightCoef, float rightCoefPosition);

		// Adds value at position with smoothing
		void AddKey(float position, float value, float smooth = 1.0f);

		// Returns key at position
		Key GetKey(float position);

		// Removes key at position
		bool RemoveKey(float position);

		// Removes all keys
		void RemoveAllKeys();

		// Returns true if contains key at position
		bool ContainsKey(float position);

		// Returns keys array
		const KeysVec& GetKeys() const;

		// Sets keys
		void SetKeys(const KeysVec& keys);

		// Smooths key at position
		void SmoothKey(float position, float smooth);

		// Returns curve max position
		float Length() const;

		// Returns true if curve is empty
		bool IsEmpty() const;

		// Key access operator by position
		Key operator[](float position);

		// Returns ease in curve
		static Curve EaseIn();

		// Returns ease out curve
		static Curve EaseOut();

		// Returns ease in-out curve
		static Curve EaseInOut();

		// Returns linear curve
		static Curve Linear();

		SERIALIZABLE(Curve);

	public:
		// ---------
		// Curve key
		// ---------
		class Key: public ISerializable
		{
		public:
			float value;             // Value @SERIALIZABLE
			float position;          // Position @SERIALIZABLE
			float leftCoef;          // Left bezier coefficient @SERIALIZABLE
			float leftCoefPosition;  // Left bezier coefficient position (0...1) @SERIALIZABLE
			float rightCoef;		 // Right bezier coefficient @SERIALIZABLE
			float rightCoefPosition; // Right bezier coefficient position (0...1) @SERIALIZABLE

		public:
			//Default constructor
			Key();

			// Constructor from value
			Key(float value);

			// Constructor
			Key(float position, float value, float leftCoef, float leftCoefPosition,
				float rightCoef, float rightCoefPosition);

			// Copy-constructor
			Key(const Key& other);

			// Copy operator
			Key& operator=(const Key& other);

			// Assign from value operator
			Key& operator=(float value);

			// Float cast operator
			operator float() const;

			// Check equals operator
			bool operator==(const Key& other) const;

			SERIALIZABLE(Key);

		public:
			static const int mApproxValuesCount = 10; // Approximation values count
			Vec2F mApproxValues[mApproxValuesCount];  // Approximation values (x - position, y - value)

			friend class Curve;
		};

	protected:
		KeysVec mKeys; // Curve keys @SERIALIZABLE

		// Updates approximation
		void UpdateApproximation();

		// Returns keys (for property)
		KeysVec GetKeysNonContant();

		// Completion deserialization callback
		void OnDeserialized(const DataNode& node);

		// Initializes properties
		void InitializeProperties();
	};
}