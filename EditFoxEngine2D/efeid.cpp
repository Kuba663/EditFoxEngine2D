#include "stdafx.h"
#include "efeid.h"
#include <cstdio>

EditFoxEngine::efeid::efeid(xstring category)
{
	this->crc = crc32buf(const_cast<char*>(category.data()), strlen(category.c_str()));
	this->rgn = std::mt19937_64(std::hash<time_t>()(std::time(nullptr)) ^ (std::hash<std::thread::id>()(std::this_thread::get_id()) << 1))();
}

bool EditFoxEngine::efeid::operator<(efeid other) const
{
	if (this->crc == other.crc) return this->rgn < other.rgn;
	else return this->crc < other.crc;
}

bool EditFoxEngine::efeid::operator!=(efeid other) const
{
	return !(*this == other);
}

bool EditFoxEngine::efeid::operator==(efeid other) const
{
	return this->crc == other.crc && this->rgn == other.rgn;
}
