#include "Result.h"
#include <cassert>

bool Result(const HRESULT& result)
{
	assert(SUCCEEDED(result));
	if (FAILED(result)) return false;
	return true;
}
