#pragma once

namespace o2
{
	class IObject;

	class IPtr
	{
		friend class MemoryManager;
		friend struct ObjectInfo;

	protected:
		IObject* mObject; /** Pointer to object. */
		bool     mStatic; /** True, if object is static, false if object is child of another object. */

		/** ctor. */
		IPtr(IObject* object = nullptr);

		/** copy-ctor. */
		IPtr(const IPtr& other);

		/** virtual dtor. */
		virtual ~IPtr();

		/** Copy operator. */
		IPtr& operator=(const IPtr& other);

	public:
		/** Returns true, if mObject is valid. */
		bool IsValid() const;

		/** Releases object. */
		void Release();
	};
}