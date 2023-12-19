#pragma once

#include <iostream>
#include <string>
#include <unistd.h>

namespace args {

    /**
     *
     */
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 800;
    static const int FRAME_RATE = 128;

    /**
     *
     */
    enum SimulatorType {
        Naive, BarnesHut
    };


    /**
     *
     */
    struct Arguments {

    private:

        // Default values for command line arguments
        const int DEFAULT_NUMBER_OF_PARTICLES = 1000;
        const bool DEFAULT_DRAW_REGION = false;
        const SimulatorType DEFAULT_SIMULATOR_TYPE = BarnesHut;

    public:

        // Command line arguments
        int numberOfParticles{DEFAULT_NUMBER_OF_PARTICLES};
        bool drawRegion{DEFAULT_DRAW_REGION};
        SimulatorType simulatorType{DEFAULT_SIMULATOR_TYPE};
    };

    /**
     *
     * @param argc
     * @param argv
     * @return
     */
    Arguments parse(int argc, char *argv[]);

    /**
     *
     * @param simulatorType
     * @return
     */
    std::string toString(SimulatorType simulatorType);

    /**
     *
     * @param args
     */
    void print(Arguments args);
}
