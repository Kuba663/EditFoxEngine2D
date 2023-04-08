#include "stdafx.h"
#include "efeid.h"

EditFoxEngine::efeid::efeid(xstring category)
{
	this->crc = crc32buf(const_cast<char*>(category.data()), category.length());
	this->rgn = std::mt19937_64(std::hash<time_t>()(std::time(nullptr)) ^ (std::hash<std::thread::id>()(std::this_thread::get_id()) << 1))();
}
