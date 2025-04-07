//
// Created by anini on 03-04-25.
//

#include "Envelope.h"

Envelope::Envelope(double sampleRate)
    : sampleRate(sampleRate),
      envelopeValue(0.0),
      attackTime(0.1),  
      releaseTime(0.5),
      state(IDLE) {
    updateIncrements();
}
