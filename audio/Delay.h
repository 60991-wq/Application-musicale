//
// Created by anini on 03-04-25.
//

#ifndef DELAY_H
#define DELAY_H
#include <vector>


class Delay {
public:
    Delay(double sampleRate);
    void setDelay(double seconds);
    void setMix(float mix);
    void process(float* buffer , int frames);


    private:
    std::vector<float> delayBuffer;
    int bufferSize;
    int writeIndex;
    int readIndex;
    double sampleRate;

    float delayMix;
    double delayTime;

    void updateReadIndex();



};



#endif //DELAY_H
