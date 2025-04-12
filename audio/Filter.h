#ifndef FILTER_H
#define FILTER_H

class Filter {
public:
    explicit Filter(double sampleRate = 44100.0);

    void setCutoff(double cutoffHz);
    void reset();

    float process(float input);

private:
    double sampleRate;
    double cutoff;
    double alpha;
    float lastOutput;

    void updateAlpha();
};

#endif // FILTER_H
