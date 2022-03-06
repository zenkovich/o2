#include "o2/stdafx.h"
#include "FunctionInfo.h"

namespace o2
{
	bool FunctionInfoBase::operator==(const FunctionInfoBase& other) const
	{
		return mName == other.mName &&
			mReturnType == other.mReturnType && mParameters == other.mParameters;
	}

	FunctionInfoBase* FunctionInfoBase::Clone() const
	{
		return mnew FunctionInfoBase(*this);
	}

	FunctionInfoBase* FunctionInfo::Clone() const
	{
		return mnew FunctionInfo(*this);
	}

	bool FunctionInfo::operator==(const FunctionInfo& other) const
	{
		return FunctionInfoBase::operator==(other) && mIsContant == other.mIsContant;
	}

	Type* FunctionInfoBase::GetOwnerType() const
	{
		return mOwnerType;
	}

	const String& FunctionInfoBase::GetName() const
	{
		return mName;
	}

	const Type* FunctionInfoBase::GetReturnType() const
	{
		return mReturnType;
	}

	bool FunctionInfo::IsConstant() const
	{
		return mIsContant;
	}

	ProtectSection FunctionInfoBase::GetProtectionSection() const
	{
		return mProtectSection;
	}

	const Vector<IAttribute*>& FunctionInfoBase::GetAttributes() const
	{
		return mAttributes;
	}

	bool FunctionInfoBase::Parameter::operator==(const Parameter& other) const
	{
		return name == other.name && type == other.type && isConstant == other.isConstant && isPointer == other.isPointer &&
			isReference == other.isReference;
	}

	const Vector<FunctionInfoBase::Parameter>& FunctionInfoBase::GetParameters() const
	{
		return mParameters;
	}

	FunctionInfoBase* StaticFunctionInfo::Clone() const
	{
		return mnew StaticFunctionInfo(*this);
	}
}
