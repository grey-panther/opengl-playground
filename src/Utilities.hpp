#pragma once

#include "iostream"


#define assertTrue(cond) \
do { \
	if (!bool(cond)) { \
		std::cerr <<       \
				"[E] Assertion failed: " #cond " is false\n" \
                "\tFunction: " << __func__ << "\n" \
				"\tLocation: " __FILE__ ":" << std::to_string(__LINE__) << std::endl; \
	} \
} while(false);


#define assertFalse(cond) \
do { \
	if (bool(cond)) { \
		std::cerr <<       \
				"[E] Assertion failed: " #cond " is true\n" \
                "\tFunction: " << __func__ << "\n" \
				"\tLocation: " __FILE__ ":" << std::to_string(__LINE__) << std::endl; \
	} \
} while(false);


#define assertTrueMsg(cond, msg) \
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


#define assertNeverReachHere(msg) \
do { \
	std::cerr <<       \
			"[E] Assertion failed: should never reach here.\n" \
			"\tFunction: " << __func__ << "\n" \
			"\tLocation: " __FILE__ ":" << std::to_string(__LINE__) << "\n" \
			"\tMessage: " << (msg) \
			<< std::endl; \
} while(false);


void debugHandleGLErrors();


#define handleGLErrors() debugHandleGLErrors()


namespace String {
	static const std::string& empty() {
		static const std::string* empty = new std::string {};
		return *empty;
	}
}
