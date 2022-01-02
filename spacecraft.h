#ifndef SPACECRAFT_H
#define SPACECRAFT_H


class Spacecraft
{
public:
    Spacecraft();

    // max acceleration (mm/sec/sec)
    double max_acceleration;

    bool detect_beam_hit();
};

#endif // SPACECRAFT_H
