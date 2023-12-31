
#include "args/args.h"

namespace args {

    Arguments parse(int argc, char *argv[]) {

        Arguments args;

        int c;
        while ((c = getopt(argc, argv, ":n:s:r")) != -1) {
            switch (c) {
                case 'n':
                    args.numberOfParticles = std::stoi(optarg);
                    break;
                case 's':
                    args.simulatorType = toSimulatorType(optarg);
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
                str = "Naive";
                break;
            case args::BarnesHut:
                str = "BarnesHut";
                break;
        }

        return str;
    }

    SimulatorType toSimulatorType(std::string simulator) {

        if (simulator == toString(Naive)) {
            return Naive;
        } else if (simulator == toString(BarnesHut)) {
            return BarnesHut;
        } else {
            throw std::invalid_argument("simulator not supported");
        }
    }

    void print(Arguments args) {
        std::cout << "# " << "number of particles: " << args.numberOfParticles << std::endl;
        std::cout << "# " << "draw region: " << (args.drawRegion ? "true" : "false") << std::endl;
        std::cout << "# " << "simulator type: " << toString(args.simulatorType) << std::endl;
    }
}