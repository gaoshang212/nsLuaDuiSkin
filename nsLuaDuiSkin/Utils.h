#pragma once

#include "stdafx.h"

class utils
{
public:
	static void TCharToChar(const TCHAR * tchar, char * _char);
};

class nstring
{
public:
	nstring(int size)
	{
		alloc(size);
	}

	~nstring()
	{
		delete t_buffer;
#ifdef UNICODE
		if (c_buffer != nullptr)
		{
			delete c_buffer;
		}
#endif
	}

	char* getdata()
	{
		char* result = nullptr;
#ifdef UNICODE
		if (c_buffer == nullptr)
		{
			_size = WideCharToMultiByte(CP_OEMCP, 0, t_buffer, -1, 0, 0, NULL, NULL);
			result = c_buffer = new char[_size];
			WideCharToMultiByte(CP_OEMCP, 0, t_buffer, -1, c_buffer, _size, NULL, NULL);
		}
#else

		result = t_buffer;
#endif

		return result;
	}

	LPTSTR gettdata()
	{
		return t_buffer;
	}
#ifdef UNICODE
	size_t size()
	{
		return _size;
	}
#endif

	size_t tsize()
	{
		return wcslen(t_buffer);
	}

	int maxsize()
	{
		return _maxsize;
	}

	nstring& operator =(LPCTSTR lpsz)
	{
		//ASSERT(lpsz == NULL || _IsValidString(lpsz));
		//AssignCopy(SafeStrlen(lpsz), lpsz);
		copy(lpsz);

		return *this;
	}

	//nstring& operator =(const unsigned char* lpsz)
	//{
	//	*this = (LPCSTR)lpsz;
	//	return *this;
	//}

private:
	LPTSTR t_buffer = nullptr;
#ifdef UNICODE
	char* c_buffer = nullptr;
	size_t _size;
#endif

	int _maxsize;

	void copy(LPCTSTR lpsz)
	{
		auto size = (lpsz == NULL) ? 0 : lstrlen(lpsz);
		alloc(size);
		lstrcpy(t_buffer, lpsz);
	}

	void alloc(size_t size)
	{
		t_buffer = new TCHAR[size];
		_size = size;
	}
	
};
