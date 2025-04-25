//
// Created by anini on 25-04-25.
//

#include "AudioParam.h"
#include <mutex>

using Lock = std::lock_guard<std::mutex>;

POD LockedPOD::getCopy() const {
    Lock lock(mutex);
    return data;
}

void LockedPOD::setCopy(const POD& newData) {
    Lock lock(mutex);
    data = newData;
}
