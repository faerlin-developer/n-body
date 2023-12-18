#pragma once

#include <string>
#include <unistd.h>

namespace args {

    enum SimulatorType {
        Naive, BarnesHut
    };

    // Screen constants
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 800;
    static const int FRAME_RATE = 128;

    // Command line arguments
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

    Arguments parse(int argc, char *argv[]) {

        Arguments args;

        int c;
        while ((c = getopt(argc, argv, ":n:s:r")) != -1) {
            switch (c) {
                case 'n':
                    args.numberOfParticles = std::stoi(optarg);
                    break;
                case 's':
                    args.simulatorType = SimulatorType(std::stoi(optarg));
                    break;
                case 'r':
                    args.drawRegion = true;
                    break;
                case ':':
                    printf("USAGE: -%c requires argument\n", optopt);
                    break;
                case '?':
                    printf("USAGE: unknown arg -%c\n", optopt);
                    break;
            }
        }

        return args;
    }

    std::string toString(SimulatorType simulatorType) {

        std::string str;
        switch (simulatorType) {
            case args::Naive:
                std::cout << "here!" << std::endl;
                str = "Naive";
                break;
            case args::BarnesHut:
                str = "BarnesHut";
                break;
        }

        return str;
    }

    void print(Arguments args) {
        std::cout << std::endl;
        std::cout << "number of particles: " << args.numberOfParticles << std::endl;
        std::cout << "draw region: " << (args.drawRegion == true ? "true" : "false") << std::endl;
        std::cout << "simulator type: " << toString(args.simulatorType) << std::endl;
        std::cout << std::endl;
    }

}
