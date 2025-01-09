
#include "third_party/espressif/led_strip/src/enabled.h"

#if FASTLED_RMT5

#ifdef FASTLED_RMT_BUILTIN_DRIVER
#warning "FASTLED_RMT_BUILTIN_DRIVER is not supported in RMT5 and will be ignored."
#endif

#include <assert.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "idf5_rmt.h"
#include "third_party/espressif/led_strip/strip_rmt.h"

#include "fl/assert.h"


#define TAG "idf5_rmt.cpp"


namespace {  // anonymous namespace

void convert(int T1, int T2, int T3, uint16_t* T0H, uint16_t* T0L, uint16_t* T1H, uint16_t* T1L) {
    *T0H = T1;
    *T0L = T2 + T3;
    *T1H = T1 + T2;
    *T1L = T3;
}

}  // namespace

RmtController5::RmtController5(int DATA_PIN, int T1, int T2, int T3)
        : mPin(DATA_PIN), mT1(T1), mT2(T2), mT3(T3) {
}

RmtController5::~RmtController5() {
    if (mLedStrip) {
        delete mLedStrip;
    }
}

void RmtController5::loadPixelData(PixelIterator &pixels) {
    const bool is_rgbw = pixels.get_rgbw().active();
    if (!mLedStrip) {
        uint16_t t0h, t0l, t1h, t1l;
        convert(mT1, mT2, mT3, &t0h, &t0l, &t1h, &t1l);
        mLedStrip = IRmtStrip::Create(mPin, pixels.size(), is_rgbw, t0h, t0l, t1h, t1l, 280, IRmtStrip::DMA_AUTO);
        
    } else {
        FASTLED_ASSERT(
            mLedStrip->numPixels() == pixels.size(),
            "mLedStrip->numPixels() (" << mLedStrip->numPixels() << ") != pixels.size() (" << pixels.size() << ")");
    }
    if (is_rgbw) {
        uint8_t r, g, b, w;
        for (uint16_t i = 0; pixels.has(1); i++) {
            pixels.loadAndScaleRGBW(&r, &g, &b, &w);
            mLedStrip->setPixelRGBW(i, r, g, b, w); // Tested to be faster than memcpy of direct bytes.
            pixels.advanceData();
            pixels.stepDithering();
        }
    } else {
        uint8_t r, g, b;
        for (uint16_t i = 0; pixels.has(1); i++) {
            pixels.loadAndScaleRGB(&r, &g, &b);
            mLedStrip->setPixel(i, r, g, b); // Tested to be faster than memcpy of direct bytes.
            pixels.advanceData();
            pixels.stepDithering();
        }
    }

}

void RmtController5::showPixels() {
    // Work around a blip that happens when the rmt_channel is disabled/deallocated.
    // We now set the INPUT_PULLDOWN on the pin to avoid the blip, per the recommendations
    // of espressif. However, but there could still be a momentary blip between the rmt
    // channel being disable and when the INPUT_PULLDOWN is enabled. However the blip will
    // be less than a few microseconds.
    // See espressif bug https://github.com/espressif/esp-idf/issues/15049
    mLedStrip->drawAsync();
}

#endif  // FASTLED_RMT5

