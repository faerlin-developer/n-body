#include <SFML/Graphics.hpp>
#include <mpi.h>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <sstream>
#include "Particle.h"
#include "State.h"
#include "Vector.h"
#include "Particle.h"

void centerWindow(sf::RenderWindow *window);

void updateAcceleration(int i, Particle *particles, int N);

float random(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

int main(int argc, char *argv[]) {

    int world_size;
    int world_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm WORLD = MPI_COMM_WORLD;
    const int ROOT = 0;

    if (world_size != 14) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    MPI_Comm shared_comm;
    int shared_rank;
    int shared_size;
    MPI_Comm_split_type(WORLD, MPI_COMM_TYPE_SHARED, world_rank, MPI_INFO_NULL, &shared_comm);
    MPI_Comm_rank(shared_comm, &shared_rank);
    MPI_Comm_size(shared_comm, &shared_size);

    if (shared_size != world_size) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    auto N = 1260;
    auto size = N / shared_size;
    auto start = shared_rank * size;
    auto end = (shared_rank + 1) * size - 1;

    std::cout << "[" << shared_rank << "] " << start << " " << end << std::endl;

    if (shared_rank == 0) {

        MPI_Win mpi_window;
        Particle *particles;
        auto err = MPI_Win_allocate_shared(N * sizeof(Particle),
                                           sizeof(Particle),
                                           MPI_INFO_NULL,
                                           shared_comm,
                                           &particles,
                                           &mpi_window);

        if (err != MPI_SUCCESS) {
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        auto window = sf::RenderWindow(sf::VideoMode(800, 800), "SFML works!");

        sf::Vector2u size = window.getSize();
        sf::View view(sf::Vector2f(0, 0), sf::Vector2f(size.x, size.y));
        window.setView(view);

        centerWindow(&window);

        for (int i = 0; i < N; i++) {
            auto mass = random(5, 15);
            auto x = random(-1, 1);
            auto y = random(-1, 1);
            auto pos = Vector(x, y).unit();
            auto vel = pos;
            vel.rotate(M_PI / 2, Vector(0, 0));
            pos.setMagnitude(random(100, 200));
            vel.setMagnitude(random(5, 10));

            particles[i] = Particle(mass, pos.x, pos.y, vel.x, vel.y);
        }

        int frames = 0;
        sf::Clock clock_total;
        sf::Clock clock_update;
        sf::Clock clock_draw;

        float frame_rate = 0;
        window.setFramerateLimit(24);
        while (window.isOpen()) {
            clock_total.restart();

            sf::Event event{};
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    MPI_Abort(MPI_COMM_WORLD, EXIT_SUCCESS);
                }
            }

            clock_update.restart();

            MPI_Barrier(shared_comm);
            for (int i = start; i <= end; i++) {
                updateAcceleration(i, particles, N);
            }
            MPI_Barrier(shared_comm);

            for (int i = start; i <= end; i++) {
                particles[i].vel = particles[i].vel + particles[i].acc;
                particles[i].pos = particles[i].pos + particles[i].vel;
                particles[i].acc.set(0, 0);
            }
            MPI_Barrier(shared_comm);

            auto update_time = clock_update.getElapsedTime();

            MPI_Barrier(shared_comm);
            clock_draw.restart();
            window.clear();
            for (int i = 0; i < N; i++) {
                auto r = particles[i].radius;
                sf::CircleShape circle(r);
                circle.setPosition(particles[i].pos.x - r, particles[i].pos.y - r);
                circle.setFillColor(sf::Color{128, 128, 128});
                circle.setOutlineColor(sf::Color::White);
                circle.setOutlineThickness(2.0f);
                window.draw(circle);
            }

            auto draw_time = clock_draw.getElapsedTime();

            sf::Font arial;
            arial.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf");
            std::stringstream s;
            s << "frame rate: " << frame_rate;
            sf::Text text(s.str(), arial, 15);
            text.setPosition(-(size.x / 2.0), -(size.y / 2.0));
            text.setFillColor(sf::Color::White);
            window.draw(text);

            window.display();
            auto total_time = clock_total.getElapsedTime();

            if (frames % 24 == 0) {

                auto update_time_s = update_time.asSeconds();
                auto draw_time_s = draw_time.asSeconds();
                auto total_time_s = total_time.asSeconds();
                frame_rate = 1.f / total_time_s;

                std::cout << frame_rate << " "
                          << update_time_s << " "
                          << draw_time_s << " "
                          << total_time_s << " "
                          << update_time_s / total_time_s << std::endl;
            }

            frames++;
        }

    } else {
        MPI_Win mpi_window;
        Particle *empty;
        auto err = MPI_Win_allocate_shared(0, sizeof(Particle), MPI_INFO_NULL, shared_comm, &empty, &mpi_window);

        if (err != MPI_SUCCESS) {
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        MPI_Aint size;
        int disp;
        Particle *particles;
        MPI_Win_shared_query(mpi_window, 0, &size, &disp, &particles);

        while (true) {

            MPI_Barrier(shared_comm);
            for (int i = start; i <= end; i++) {
                updateAcceleration(i, particles, N);
            }
            MPI_Barrier(shared_comm);

            for (int i = start; i <= end; i++) {
                particles[i].vel = particles[i].vel + particles[i].acc;
                particles[i].pos = particles[i].pos + particles[i].vel;
                particles[i].acc.set(0, 0);
            }
            MPI_Barrier(shared_comm);
        }

    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

void centerWindow(sf::RenderWindow *window) {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto pos = sf::Vector2i(desktop.width / 2 - window->getSize().x / 2, desktop.height / 2 - window->getSize().y / 2);
    window->setPosition(pos);
}

void updateAcceleration(int i, Particle *particles, int N) {

    for (int j = 0; j < N; j++) {
        if (i != j) {
            particles[i].attractedBy(particles[j]);
        }
    }
}

//        Vector v = Vector(100, 100);
//        Vector origin = Vector(30, 100);
//        v.rotate(M_PI * (4.f / 3), origin);
//
//        sf::CircleShape circle(2);
//        circle.setPosition(30 - 2, 100 - 2);
//
//        sf::Vertex line[] = {
//                sf::Vertex(sf::Vector2f(30, 100)),
//                sf::Vertex(sf::Vector2f(v.x, v.y))
//        };
//        window.draw(line, 2, sf::Lines);
//        window.draw(circle);

//     auto M = 10000;
//    float G = 1;
//    auto vMagnitude = std::sqrt(G * M / 200);
// particles[0] = Particle(0.5, 400, 100, vMagnitude, 0);
// particles[1] = Particle(0.5, 400, 500, vMagnitude, 0);
// particles[2] = Particle(M, 400, 300, 0, 0);
