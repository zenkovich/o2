#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace o2
{
#define Function std::function

	template<class T>
	class FunctionsList
	{
	protected:
		typedef std::vector< Function<T> > FunctionsVec;

		FunctionsVec mFunctions;

	public:
		FunctionsList();
		FunctionsList(const Function<T>& func);

		void Add(const FunctionsList<T>& other);
		int  Add(const Function<T>& func);
		void Remove(int idx);
		void Clear();

		FunctionsList& operator=(const Function<T>& func);
		FunctionsList  operator+(const Function<T>& func) const;
		FunctionsList& operator+=(const Function<T>& func);
		FunctionsList  operator+(const FunctionsList<T>& other) const;
		FunctionsList& operator+=(const FunctionsList<T>& other);

		template<class... Args>
		void operator()(Args... args);
	};

	//implementation
	template<class T>
	FunctionsList<T>::FunctionsList() {}

	template<class T>
	FunctionsList<T>::FunctionsList(const Function<T>& func)
	{
		mFunctions.push_back(func);
	}

	template<class T>
	void FunctionsList<T>::Add(const FunctionsList<T>& other)
	{
		for (auto fn:other.mFunctions)
			mFunctions.push_back(func);
	}

	template<class T>
	int FunctionsList<T>::Add(const Function<T>& func)
	{
		mFunctions.push_back(func);
		return mFunctions.size() - 1;
	}

	template<class T>
	void FunctionsList<T>::Remove(int idx)
	{
		if (idx < 0 || idx >= mFunctions.size())
			return;

		mFunctions.erase(mFunctions.begin() + idx);
	}

	template<class T>
	void FunctionsList<T>::Clear()
	{
		mFunctions.clear();
	}

	template<class T>
	FunctionsList<T>& FunctionsList<T>::operator=(const Function<T>& func)
	{
		mFunctions.clear();
		mFunctions.push_back(func);
		return *this;
	}

	template<class T>
	FunctionsList<T> FunctionsList<T>::operator+(const Function<T>& func) const
	{
		FunctionsList<T> res(*this);
		res.Add(func);
		return res;
	}

	template<class T>
	FunctionsList<T>& FunctionsList<T>::operator+=(const Function<T>& func)
	{
		Add(func);
		return *this;
	}

	template<class T>
	FunctionsList<T> FunctionsList<T>::operator+(const FunctionsList<T>& other) const
	{
		FunctionsList<T> res(*this);
		res.Add(other);
		return res;
	}

	template<class T>
	FunctionsList<T>& FunctionsList<T>::operator+=(const FunctionsList<T>& other)
	{
		Add(other);
		return *this;
	}

	template<class T>
	template<class... Args>
	void FunctionsList<T>::operator()(Args... args)
	{
		for (auto fn:mFunctions)
			fn(args...);
	}
}