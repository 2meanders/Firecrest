#pragma once
#include <chrono>
#include <string>

namespace fc::time {

	struct Duration {
		// The length of the duration in milliseconds
		long long value;
		
		inline constexpr long long millis() const {
			return value;
		}
		
		inline double seconds() const {
			return static_cast<double>(value) / 1000.0;
		}
		
	private:
		constexpr Duration(long long ms) : value(ms) {}
	public:
		inline static constexpr Duration fromMillis(long long ms) {
			return { ms };
		}
		inline static Duration fromSeconds(double sec) {
			return { static_cast<long long>(sec * 1000) };
		}
	};
	
	struct Moment {
		// Time since epoch, in milliseconds
		long long value;
		
	private:
		constexpr Moment(long long ms) : value(ms) {}
	public:
		inline static constexpr Moment fromMillis(long long ms) {
			return { ms };
		}
		inline static Moment fromSeconds(double sec) {
			return { static_cast<long long>(sec * 1000) };
		}
	};

	// Creates a moment representing the current time
	inline Moment now() {
		return Moment::fromMillis(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	} 

	inline constexpr Duration operator-(const Moment& lhs, const Moment& rhs) {
		return Duration::fromMillis(lhs.value - rhs.value);
	}

	inline constexpr Moment operator+(const Moment& m, const Duration& d) {
		return Moment::fromMillis(m.value + d.value);
	}

	inline constexpr Moment operator-(const Moment& m, const Duration& d) {
		return Moment::fromMillis(m.value - d.value);
	}

	inline constexpr Duration operator+(const Duration& lhs, const Duration& rhs) {
		return Duration::fromMillis(lhs.value + rhs.value);
	}

	inline constexpr Duration operator-(const Duration& lhs, const Duration& rhs) {
		return Duration::fromMillis(lhs.value - rhs.value);
	}

	inline constexpr bool operator<(const Moment& lhs, const Moment& rhs) {
		return lhs.value < rhs.value;
	}
	inline constexpr bool operator>(const Moment& lhs, const Moment& rhs) {
		return lhs.value > rhs.value;
	}
	inline constexpr bool operator<=(const Moment& lhs, const Moment& rhs) {
		return lhs.value <= rhs.value;
	}
	inline constexpr bool operator>=(const Moment& lhs, const Moment& rhs) {
		return lhs.value >= rhs.value;
	}
	inline constexpr bool operator==(const Moment& lhs, const Moment& rhs) {
		return lhs.value == rhs.value;
	}

	inline constexpr bool operator<(const Duration& lhs, const Duration& rhs) {
		return lhs.value < rhs.value;
	}
	inline constexpr bool operator>(const Duration& lhs, const Duration& rhs) {
		return lhs.value > rhs.value;
	}
	inline constexpr bool operator<=(const Duration& lhs, const Duration& rhs) {
		return lhs.value <= rhs.value;
	}
	inline constexpr bool operator>=(const Duration& lhs, const Duration& rhs) {
		return lhs.value >= rhs.value;
	}
	inline constexpr bool operator==(const Duration& lhs, const Duration& rhs) {
		return lhs.value == rhs.value;
	}
	
}
