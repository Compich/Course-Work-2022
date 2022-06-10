#include "resourceLoading.h"

pair<LPVOID, DWORD> loadResource(const int& res)
{
	HRSRC rsrcData = FindResource(NULL, MAKEINTRESOURCE(res), RT_RCDATA);
	if (!rsrcData)
		throw "Failed to find resource.";

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	if (rsrcDataSize <= 0)
		throw "Size of resource is 0.";

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	if (!grsrcData)
		throw "Failed to load resource.";

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw "Failed to lock resource.";

	return pair<LPVOID, DWORD>(firstByte, rsrcDataSize);
}