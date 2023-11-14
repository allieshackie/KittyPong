#pragma once
#include <string>

class COGName
{
public:
	COGName(const char* name) : mName(name)
	{
	}

	const std::string& GetName() const { return mName; }

private:
	std::string mName;
};
