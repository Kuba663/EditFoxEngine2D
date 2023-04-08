#ifndef __EDITFOXENGINE_ID_H_
#define __EDITFOXENGINE_ID_H_
#include <crc.h>
#include <random>
#include <ctime>
#include <thread>
namespace EditFoxEngine {
	struct efeid {
		unsigned long crc;
		unsigned long long rgn;
		efeid() = default;
		explicit efeid(xstring category);
	};
}
template<>
struct std::hash<EditFoxEngine::efeid> {
	std::size_t operator()(EditFoxEngine::efeid const& id) const noexcept {
		size_t h1 = std::hash<unsigned long>()(id.crc);
		size_t h2 = std::hash<unsigned long long>()(id.rgn);
		return h1 ^ (h2 << 1);
	}
};
#endif
