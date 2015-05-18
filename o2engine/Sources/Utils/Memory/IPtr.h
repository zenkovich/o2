#pragma once

namespace o2
{
	class IObject;

	class IPtr
	{
		friend class MemoryManager;

	protected:
		IObject* mObject;
		bool     mStatic;

		IPtr(void* object = nullptr);
		IPtr(const IPtr& other);
		virtual ~IPtr();

		IPtr& operator=(const IPtr& other);

	public:
		bool IsValid() const;
		void Release();
	};
}