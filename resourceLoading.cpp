#include "resourceLoading.h"

pair<LPVOID, DWORD> loadResource(const int& res)
{
	HRSRC rsrcData = FindResource(NULL, MAKEINTRESOURCE(res), RT_RCDATA);
	//if (!rsrcData)
	//	throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	//if (rsrcDataSize <= 0)
	//	throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	//if (!grsrcData)
	//	throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	//if (!firstByte)
	//	throw std::runtime_error("Failed to lock resource.");

	return pair<LPVOID, DWORD>(firstByte, rsrcDataSize);
}