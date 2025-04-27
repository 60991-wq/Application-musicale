#ifndef FILTER_H
#define FILTER_H

class Filter {
public:
    explicit Filter(double sampleRate);

    void setCutoff(double cutoffHz);
    void setResonance(double res);
    void reset();

    float process(float input);

private:
    void updateAlpha();

    double sampleRate;
    double cutoff;
    double resonance {0.0};
    float lastOutput;
    float alpha;
};

#endif // FILTER_H