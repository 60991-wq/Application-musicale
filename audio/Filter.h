//
// Created by anini on 03-04-25.
//

#ifndef FILTER_H
#define FILTER_H
#include <bits/ranges_algo.h>


class Filter {

public:
    explicit Filter(double sampleRate = 44100.0);
        void setCutoff(double cutoff);
        void reset();

        float process(float input);



private:
    double sampleRate;
    double cutoff;
    double alpha;
    float lastOutput;

    void updateAlpha();



};



#endif //FILTER_H
