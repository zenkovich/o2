#pragma once

namespace o2
{
	class IObject;

	class IPtr
	{
		friend class MemoryManager;
		friend struct ObjectInfo;

	protected:
		IObject* mObject;
		bool     mStatic;

		IPtr(IObject* object = nullptr);
		IPtr(const IPtr& other);
		virtual ~IPtr();

		IPtr& operator=(const IPtr& other);

	public:
		bool IsValid() const;
		void Release();
	};
}