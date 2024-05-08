#pragma once

namespace android {
	namespace base {

		// Use memcpy for access to unaligned data on targets with alignment
		// restrictions.  The compiler will generate appropriate code to access these
		// structures without generating alignment exceptions.
		template <typename T>
		static inline T get_unaligned(const void* address) {
			T result;
			memcpy(&result, address, sizeof(T));
			return result;
		}

		template <typename T>
		static inline void put_unaligned(void* address, T v) {
			memcpy(address, &v, sizeof(T));
		}

	} // namespace base
} // namespace android