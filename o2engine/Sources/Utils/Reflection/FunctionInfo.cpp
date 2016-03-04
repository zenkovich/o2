#include "FunctionInfo.h"

namespace o2
{
	bool FunctionInfo::operator==(const FunctionInfo& other) const
	{
		return mName == other.mName && mIsContant == other.mIsContant &&
			mReturnType == other.mReturnType && mParameters == other.mParameters;
	}

	Type* FunctionInfo::GetOwnerType() const
	{
		return mOwnerType;
	}

	const String& FunctionInfo::GetName() const
	{
		return mName;
	}

	const Type* FunctionInfo::GetReturnType() const
	{
		return mReturnType;
	}

	bool FunctionInfo::IsConstant() const
	{
		return mIsContant;
	}

	const FunctionInfo::ParametersVec& FunctionInfo::GetParameters() const
	{
		return mParameters;
	}

	bool FunctionInfo::Parameter::operator==(const Parameter& other) const
	{
		return name == other.name && type == other.type && isConstant == other.isConstant && isPointer == other.isPointer &&
			isReference == other.isReference;
	}

}