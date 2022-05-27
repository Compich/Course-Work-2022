#pragma once
#include <Windows.h>
#include <utility>

using std::pair;

pair<LPVOID, DWORD> loadResource(const int& res);
