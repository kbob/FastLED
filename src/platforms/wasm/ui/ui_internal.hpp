#pragma once

#include "ui_internal.h"

FASTLED_NAMESPACE_BEGIN

inline jsUiInternal::jsUiInternal(const char* name, UpdateFunction updateFunc, ToJsonFunction toJsonFunc)
    : mName(name), mUpdateFunc(updateFunc), mtoJsonFunc(toJsonFunc), mId(nextId()), mMutex() {}

inline const char* jsUiInternal::name() const { return mName; }
inline void jsUiInternal::update(const char* jsonStr) { 
    std::lock_guard<std::mutex> lock(mMutex);
    if (mUpdateFunc) {
        mUpdateFunc(jsonStr);
    }
}
inline void jsUiInternal::toJson(ArduinoJson::JsonObject& json) const {
    std::lock_guard<std::mutex> lock(mMutex);
    if (mtoJsonFunc) {
        mtoJsonFunc(json);
    }
}
inline int jsUiInternal::id() const { return mId; }

inline bool jsUiInternal::clearFunctions() {
    std::lock_guard<std::mutex> lock(mMutex);
    bool wasCleared = (mUpdateFunc.callback != nullptr) || (mtoJsonFunc.callback != nullptr);
    mUpdateFunc = UpdateFunction{nullptr, nullptr};
    mtoJsonFunc = ToJsonFunction{nullptr, nullptr};
    return wasCleared;
}

inline int jsUiInternal::nextId() {
    return sNextId.fetch_add(1, std::memory_order_seq_cst);
}

std::atomic<uint32_t> jsUiInternal::sNextId(0);

FASTLED_NAMESPACE_END
