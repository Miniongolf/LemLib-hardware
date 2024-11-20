namespace lemlib {

struct DynamicPort {};

constexpr DynamicPort runtime_check_port{};

class Port {
    public:
        consteval Port(int64_t port): m_port(port) {
            // XXX: this doesnt actaully throw an exception since the ctor
            // is consteval, it halts compilation instead
            if (port < 1 || port > 21) throw "Port out of range!";
        }
        Port(int64_t port, DynamicPort) : m_port(port) {
            if (port < 1 || port > 21) m_port = 0;
        }
        uint8_t get_port() const {
            return m_port;
        }
    private:
        uint8_t m_port;
};

class ReversiblePort {
    public:
        consteval ReversiblePort(int64_t port): m_port(port < 0 ? -port : port), m_reversed(port < 0) {
            if (port < 0) port = -port;  
            // XXX: this doesnt actaully throw an exception since the ctor
            // is consteval, it halts compilation instead
            if (port < 1 || port > 21) throw "Port out of range!";
        }
        ReversiblePort(int64_t port, DynamicPort): m_port(port < 0 ? -port : port), m_reversed(port < 0) {
            if (port < 0) port = -port;
            if (port < 1 || port > 21) m_port = 0;
        }
        uint8_t get_port() const {
            return m_port;
        }
        bool get_reversed() const {
            return m_reversed;
        }
    private:
        uint8_t m_port;
        bool m_reversed;
};
}
