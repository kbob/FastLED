
#ifdef FASTLED_STUB_IMPL  // Only use this if explicitly defined.

#include "led_sysdefs_stub.h"

#include <chrono>
#include <thread>

static const auto start_time = std::chrono::system_clock::now();

void pinMode(uint8_t pin, uint8_t mode) {
    // Empty stub as we don't actually ever write anything
}

uint32_t millis() {
    auto current_time = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
}

uint32_t micros() {
    auto current_time = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(current_time - start_time).count();
}

void delay(int ms) {
    std::this_thread::sleep_for (std::chrono::milliseconds(ms));
}

#endif  // FASTLED_STUB_IMPL