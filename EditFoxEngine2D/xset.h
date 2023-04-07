#ifndef _XSET_H
#define _XSET_H

#include "stl_allocator.h"
#include <set>

template<class _Kty,
	class _Pr = std::less<_Kty>,
	class _Alloc = stl_allocator<_Kty> >
	using xset = std::set<_Kty, _Pr, _Alloc>;

/// @see xset
template<class _Kty,
	class _Pr = std::less<_Kty>,
	class _Alloc = stl_allocator<_Kty> >
	using xmultiset = std::multiset<_Kty, _Pr, _Alloc>;

#endif
