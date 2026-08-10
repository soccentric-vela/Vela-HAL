#pragma once

#include <cstdint>

namespace vela::pru {

enum class RpcCommand : uint16_t {
    NOP = 0,
    GPIO_SET = 1,
    GPIO_GET = 2,
    PWM_SET = 3,
    CAPTURE_EDGE = 4,
    HEARTBEAT = 5,
    FAULT_INJECT = 6,
    RECOVER = 7,
};

enum class RpcStatus : uint16_t {
    OK = 0,
    BUSY = 1,
    INVALID = 2,
    TIMEOUT = 3,
    FAULT = 4,
};

struct SharedMailbox {
    volatile uint32_t sequence;
    volatile uint32_t command;
    volatile uint32_t arg0;
    volatile uint32_t arg1;
    volatile uint32_t arg2;
    volatile uint32_t status;
    volatile uint32_t result0;
    volatile uint32_t result1;
};

struct RpcFrame {
    uint16_t command;
    uint16_t flags;
    uint32_t correlation_id;
    uint32_t arg0;
    uint32_t arg1;
    uint32_t arg2;
};

constexpr uint32_t kProtocolVersion = 1;
constexpr const char* kDefaultRpmsgDevice = "/dev/rpmsg_pru30";

} // namespace vela::pru
