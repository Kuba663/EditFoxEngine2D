#include "stdafx.h"

std::size_t std::hash<xstring>::operator()(xstring const& str) const noexcept
{
	return std::hash<std::string>()(std::string(str.c_str()));
}