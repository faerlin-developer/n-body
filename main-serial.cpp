#include <SFML/Graphics.hpp>
#include <mpi.h>
#include <cstdlib>
#include <algorithm>
#include <random>
#include "Particle.h"
#include "State.h"
#include "Vector.h"
#include "Particle.h"

void centerWindow(sf::RenderWindow *window);

void update(int i, Particle *particles);

void updateAcceleration(int i, Particle *particles, int size);

float random(float min, float max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(mt);
}

int main(int argc, char *argv[]) {

    int size;
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm WORLD = MPI_COMM_WORLD;
    const int ROOT = 0;

    if (size != 1) {
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    auto window = sf::RenderWindow(sf::VideoMode(800, 600), "SFML works!");
    centerWindow(&window);

    auto N = 500;
    auto particles = new Particle[N];
    for (int i = 0; i < N; i++) {
        auto mass = random(5, 15);
        auto x = random(-1, 1);
        auto y = random(-1, 1);
        auto pos = Vector(x, y).unit();
        auto vel = pos;
        vel.rotate(M_PI / 2, Vector(0, 0));
        pos.setMagnitude(random(100, 200));
        vel.setMagnitude(random(5, 10));

        particles[i] = Particle(mass, pos.x + 400, pos.y + 300, vel.x, vel.y);
    }

    int frames = 0;
    sf::Clock clock_total;
    sf::Clock clock_update;
    sf::Clock clock_draw;

    window.setFramerateLimit(24);
    while (window.isOpen()) {
        clock_total.restart();

        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        clock_update.restart();
        for (int i = 0; i < N; i++) {
            updateAcceleration(i, particles, N);
        }

        for (int i = 0; i < N; i++) {
            particles[i].vel = particles[i].vel + particles[i].acc;
            particles[i].pos = particles[i].pos + particles[i].vel;
            particles[i].acc.set(0, 0);
        }
        auto update_time = clock_update.getElapsedTime();

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
        window.display();
        auto total_time = clock_total.getElapsedTime();

        if (frames % 24 == 0) {

            auto update_time_s = update_time.asSeconds();
            auto draw_time_s = draw_time.asSeconds();
            auto total_time_s = total_time.asSeconds();
            auto frame_rate = 1.f / total_time_s;

            std::cout << frame_rate << " "
                      << update_time_s << " "
                      << draw_time_s << " "
                      << total_time_s << " "
                      << update_time_s / total_time_s << std::endl;
        }

        frames++;
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

void centerWindow(sf::RenderWindow *window) {
    auto desktop = sf::VideoMode::getDesktopMode();
    auto pos = sf::Vector2i(desktop.width / 2 - window->getSize().x / 2, desktop.height / 2 - window->getSize().y / 2);
    window->setPosition(pos);
}

void updateAcceleration(int i, Particle *particles, int size) {

    auto G = 0.1;
    for (int j = 0; j < size; j++) {
        if (i != j) {
            auto acc = particles[j].pos - particles[i].pos;
            // Needs to constraint minimum value of distance to prevent divergence on the value of the magnitude
            auto distance = std::min(std::max(10.f, acc.magnitudeSquared()), 1000.f);
            auto magnitude = G * particles[j].mass / distance;
            acc.setMagnitude(magnitude);
            particles[i].acc = particles[i].acc + acc;
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
