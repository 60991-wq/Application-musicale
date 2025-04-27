//
// Created by anini on 25-04-25.
//

#include "AudioParam.h"
#include <mutex>


POD LockedPOD::getCopy() const {
    std::lock_guard<std::mutex> lock(mutex);
    return data;
}
void LockedPOD::setCopy(const POD& newData) {
    std::lock_guard<std::mutex> lock(mutex);
    data = newData;
}
