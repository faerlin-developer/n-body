#include <SFML/Graphics.hpp>
#include <mpi.h>
#include <cstdlib>
#include <random>
#include <iomanip>
#include "simulator/simulator.h"
#include "simulator/barneshut.h"
#include "simulator/naive.h"
#include "graphics/screen.h"
#include "graphics/timer.h"
#include "graphics/draw.h"
#include "simulator/init.h"

const int ROOT = 0;

void printInfo(Timer &timer);

void root(Simulator *simulator, Particle *particles, int size, int start, int end, MPI_Comm comm);

/**
 * Assumptions:
 * 1. The MPI processes are bound within one node that can form a memory shared segment.
 */
int main(int argc, char *argv[]) {

    // Initialize MPI environment
    int world_comm_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_comm_rank);
    MPI_Comm WORLD = MPI_COMM_WORLD;

    // We assume that all MPI processes in MPI_COMM_WORLD are part of the same shared memory domain.
    // Hence, only one new communicator should be created when using MPI_COMM_TYPE_SHARED as the split type.
    MPI_Comm MPI_COMM_SHARED;
    int shared_comm_rank;
    int shared_comm_size;
    MPI_Comm_split_type(WORLD, MPI_COMM_TYPE_SHARED, world_comm_rank, MPI_INFO_NULL, &MPI_COMM_SHARED);
    MPI_Comm_rank(MPI_COMM_SHARED, &shared_comm_rank);
    MPI_Comm_size(MPI_COMM_SHARED, &shared_comm_size);

    int global_size = 1000;

    MPI_Win particles_window;
    MPI_Win close_window;
    Simulator *simulator = new BarnesHut();
    // Simulator *simulator = new Naive();

    auto local_size = global_size / shared_comm_size;
    auto start = shared_comm_rank * local_size;
    auto end = (shared_comm_rank + 1) * local_size - 1;

    if (shared_comm_rank == ROOT) {

        Particle *particles;
        int size = global_size * sizeof(Particle);
        int disp_unit = sizeof(Particle);
        MPI_Win_allocate_shared(size, disp_unit, MPI_INFO_NULL, MPI_COMM_SHARED, &particles, &particles_window);

        int *flag;
        MPI_Win_allocate_shared(sizeof(int), sizeof(int), MPI_INFO_NULL, MPI_COMM_SHARED, &flag, &close_window);

        flag[0] = 0;

        root(simulator, particles, global_size, start, end, MPI_COMM_SHARED);

        flag[0] = 1;
        MPI_Barrier(MPI_COMM_SHARED);

    } else {
        MPI_Aint size;
        int disp_unit;
        Particle *temp_particles;
        Particle *particles;
        MPI_Win_allocate_shared(0, sizeof(Particle), MPI_INFO_NULL, MPI_COMM_SHARED, &temp_particles,
                                &particles_window);
        MPI_Win_shared_query(particles_window, ROOT, &size, &disp_unit, &particles);

        int *temp_flag;
        int *flag;
        MPI_Win_allocate_shared(0, sizeof(int), MPI_INFO_NULL, MPI_COMM_SHARED, &temp_flag, &close_window);
        MPI_Win_shared_query(close_window, ROOT, &size, &disp_unit, &flag);

        flag[0] = 0;

        while (true) {

            MPI_Barrier(MPI_COMM_SHARED);

            if (flag[0]) {
                break;
            }

            simulator->updateAcceleration(particles, global_size, start, end);

            MPI_Barrier(MPI_COMM_SHARED);

            simulator->updatePosition(particles, global_size, start, end);

            MPI_Barrier(MPI_COMM_SHARED);


        }
    }

    delete simulator;

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Win_free(&particles_window);
    MPI_Win_free(&close_window);
    MPI_Finalize();

    return EXIT_SUCCESS;
}

void root(Simulator *simulator, Particle *particles, int size, int start, int end, MPI_Comm comm) {

    //
    init::particles(particles, size);

    //
    int frames = 0;
    auto timer = Timer();
    auto screen = Screen(800, 800, 128);

    //
    while (screen.isOpen()) {

        timer.start("total");

        //
        auto done = screen.pollEvent();
        if (done) {
            break;
        }

        //
        MPI_Barrier(comm);
        timer.start("update");
        simulator->updateAcceleration(particles, size, start, end);
        MPI_Barrier(comm);
        simulator->updatePosition(particles, size, start, end);
        timer.end("update");
        MPI_Barrier(comm);

        //
        screen.clear();
        timer.start("draw");
        draw::particles(particles, size, screen.window);
        timer.end("draw");
        screen.display();

        timer.end("total");

        if (frames % 24 == 0) {
            printInfo(timer);
        }

        frames++;
    }

}

void printInfo(Timer &timer) {

    auto total_time = timer.getElapsed("total");
    auto update_time = timer.getElapsed("update");
    auto draw_time = timer.getElapsed("draw");

    auto frame_rate = 1.0f / total_time;
    auto update_ratio = update_time / total_time;
    auto draw_ratio = draw_time / total_time;
    std::cout << std::setprecision(3) << frame_rate << " " << update_ratio << " " << draw_ratio << std::endl;
}