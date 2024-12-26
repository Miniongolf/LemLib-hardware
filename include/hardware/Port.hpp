#pragma once

#include <cstdint>

namespace lemlib {

struct DynamicPort {};

constexpr DynamicPort runtime_check_port {};

class SmartPort {
    public:
        consteval SmartPort(std::int64_t port)
            : m_port(port) {
            // XXX: this doesn't actually throw an exception since the ctor
            // is consteval, it halts compilation instead
            if (port < 1 || port > 21) throw "Port out of range!";
        }

        constexpr SmartPort(std::int64_t port, DynamicPort)
            : m_port(port) {
            if (port < 1 || port > 21) m_port = 0;
        }

        constexpr operator std::uint8_t() const {
            return m_port;
        }
    private:
        std::uint8_t m_port;
};

class ReversibleSmartPort {
    public:
        consteval ReversibleSmartPort(std::int64_t port)
            : m_port(port) {
            if (port < 0) port = -port;
            // XXX: this doesn't actually throw an exception since the ctor
            // is consteval, it halts compilation instead
            if (port < 1 || port > 21) throw "Port out of range!";
        }

        constexpr ReversibleSmartPort(SmartPort port): m_port(port) {}

        constexpr ReversibleSmartPort(std::int64_t port, DynamicPort)
            : m_port(port) {
            if (port < 0) port = -port;
            if (port < 1 || port > 21) m_port = 0;
        }

        constexpr ReversibleSmartPort operator-() const  {
            return ReversibleSmartPort{-m_port, runtime_check_port};
        }

        constexpr bool is_reversed() const { return m_port < 0; }

        [[nodiscard("This function does not mutate the original value")]] constexpr ReversibleSmartPort set_reversed(bool reversed) const {
            std::uint8_t port = m_port < 0 ? -m_port : m_port;
            return ReversibleSmartPort{reversed ? -port : port, runtime_check_port};
        }

        constexpr operator std::int8_t() const {
            return m_port;
        }
    private:
        std::int8_t m_port;
};

class ADIPort {
    public:
        consteval ADIPort(std::int64_t port)
            : m_port(0) {
            if (port >= 'a' && port <= 'h') port -= ('a' - 1);
            else if (port >= 'A' && port <= 'H') port -= ('A' - 1);
            // XXX: this doesn't actually throw an exception since the ctor
            // is consteval, it halts compilation instead
            if (port < 1 || port > 8) throw "Port out of range!";
            m_port = port;
        }

        constexpr ADIPort(std::int64_t port, DynamicPort)
            : m_port(0) {
            if (port < 1 || port > 21) m_port = 0;
            if (port >= 'a' && port <= 'h') port -= ('a' - 1);
            else if (port >= 'A' && port <= 'H') port -= ('A' - 1);
            if (port < 1 || port > 8) return;
            m_port = port;
        }

        constexpr operator std::uint8_t() const {
            return m_port;
        }
    private:
        std::uint8_t m_port;
};

} // namespace lemlib
