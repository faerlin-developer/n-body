#include <mpi.h>
#include <cstdlib>
#include <iomanip>
#include "main.h"
#include "args/args.h"
#include "parallel/parallel.h"
#include "simulator/simulator.h"
#include "simulator/init.h"
#include "graphics/screen.h"
#include "graphics/timer.h"

/**
 * Assumptions:
 * 1. All MPI processes are bound to cores belonging to the same memory domain.
 */
int main(int argc, char *argv[]) {

    auto args = args::parse(argc, argv);

    // Initialize MPI environment
    mpi::init(argc, argv);
    auto world_rank = mpi::rank(MPI_COMM_WORLD);

    // We assume that all MPI processes in MPI_COMM_WORLD are part of the same shared memory domain.
    // Hence, only one new communicator should be created when using MPI_COMM_TYPE_SHARED as the split type.
    auto MPI_COMM_SHARED = mpi::split(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, world_rank);
    auto shared_rank = mpi::rank(MPI_COMM_SHARED);
    auto shared_size = mpi::size(MPI_COMM_SHARED);

    int N = args.numberOfParticles;
    bool region_flag = args.drawRegion;
    Simulator *simulator = init::simulator(args.simulatorType);

    auto n = N / shared_size;
    auto start = shared_rank * n;
    auto end = (shared_rank + 1) * n - 1;

    MPI_Win close_window;
    MPI_Win particles_window;

    if (shared_rank == mpi::ROOT) {

        args::print(args);

        int *close_flag;
        Particle *particles;

        particles_window = mpi::allocate(N * sizeof(Particle), sizeof(Particle), MPI_COMM_SHARED, &particles);
        close_window = mpi::allocate(sizeof(int), sizeof(int), MPI_COMM_SHARED, &close_flag);

        init::particles(particles, N);
        init::falsify(close_flag);

        runRoot(simulator, particles, start, end, N, close_flag, region_flag, MPI_COMM_SHARED);

    } else {

        void *empty;
        int *close_flag;
        Particle *particles;

        particles_window = mpi::allocate(0, sizeof(Particle), MPI_COMM_SHARED, &empty);
        close_window = mpi::allocate(0, sizeof(int), MPI_COMM_SHARED, &empty);

        mpi::query<Particle>(particles_window, mpi::ROOT, &particles);
        mpi::query<int>(close_window, mpi::ROOT, &close_flag);

        run(simulator, particles, start, end, N, close_flag, region_flag, MPI_COMM_SHARED);
    }

    delete simulator;

    mpi::barrier(MPI_COMM_SHARED);
    mpi::free(particles_window);
    mpi::free(close_window);
    mpi::finalize();

    return EXIT_SUCCESS;
}

void runRoot(Simulator *simulator,
             Particle *particles, int start, int end, int N,
             int *close_flag, int region_flag,
             MPI_Comm comm) {

    //
    int frames = 0;
    auto timer = Timer();
    auto screen = Screen(args::SCREEN_WIDTH, args::SCREEN_HEIGHT, args::FRAME_RATE);

    //
    while (screen.isOpen()) {

        timer.start("total");

        //
        auto done = screen.pollEvent();
        if (done) {
            break;
        }

        //
        mpi::barrier(comm);
        timer.start("update");
        simulator->updateAcceleration(particles, start, end, N);
        mpi::barrier(comm);
        simulator->updatePosition(particles, start, end, N);
        timer.end("update");
        mpi::barrier(comm);

        //
        screen.clear();
        timer.start("draw");
        simulator->draw(particles, N, screen.window, region_flag);
        timer.end("draw");
        screen.display();

        timer.end("total");

        if (frames % 24 == 0) {
            printInfo(timer);
        }

        frames++;
    }

    toggle(close_flag);
    mpi::barrier(comm);
}

void run(Simulator *simulator,
         Particle *particles, int start, int end, int N,
         int *close_flag,
         int region_flag,
         MPI_Comm comm) {

    while (true) {

        mpi::barrier(comm);

        if (*close_flag) {
            break;
        }

        simulator->updateAcceleration(particles, start, end, N);

        mpi::barrier(comm);

        simulator->updatePosition(particles, start, end, N);

        mpi::barrier(comm);

    }

}

void printInfo(Timer &timer) {

    auto total_time = timer.getElapsed("total");
    auto update_time = timer.getElapsed("update");
    auto draw_time = timer.getElapsed("draw");

    auto frame_rate = 1.0f / total_time;
    auto update_ratio = update_time / total_time;
    auto draw_ratio = draw_time / total_time;
    std::cout << std::left << std::setw(5) << std::setprecision(3)
              << frame_rate << " "
              << update_ratio << " "
              << draw_ratio
              << std::endl;
}

void toggle(int *flag) {
    *flag = (*flag != true);
}