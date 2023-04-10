#pragma once
#include <memory>
#include <Allocator.h>
#include <stl_allocator.h>
#include <xlist.h>
#include <xmap.h>
#include <xqueue.h>
#include <xset.h>
#include <xsstream.h>
#include <xstring.h>
#include "efeid.h"

#pragma region hash specializations
template<>
struct std::hash<xstring> {
	std::size_t operator()(xstring const& str) const noexcept;
};
#pragma endregion
