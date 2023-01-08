#pragma once

#include "iostream"


#define assert(cond) \
do { \
	if (!bool(cond)) { \
		std::cerr <<       \
				"[E] Assertion failed: " #cond "\n" \
                "\tFunction: " << __func__ << "\n" \
				"\tLocation: " __FILE__ ":" << std::to_string(__LINE__) << std::endl; \
	} \
} while(false);

#define assert2(cond, msg) \
do { \
	if (!bool(cond)) { \
		std::cerr <<       \
				"[E] Assertion failed: " #cond "\n" \
                "\tFunction: " << __func__ << "\n" \
				"\tLocation: " __FILE__ ":" << std::to_string(__LINE__) << "\n" \
				"\tMessage: " << (msg) \
				<< std::endl; \
	} \
} while(false);

namespace String {
	static const std::string& empty() {
		static const std::string* empty = new std::string {};
		return *empty;
	}
}
