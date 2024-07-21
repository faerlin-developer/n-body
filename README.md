[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<div align="center">
    <h3 align="center">N-Body Simulation Using MPI</h3>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#performance-on-1540-bodies">Performance on N=1540 Bodies</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#running-the-application">Running the Application</a></li>
      </ul>
    </li>
    <li><a href="#license">License</a></li>
  </ol>
</details>

## About The Project

<br/>
<div align="center">
    <img src="img/screenshot.png" width="700">
    <br/>
    <figcaption>Figure 1. Simulation of 1000 particles. The red bounding boxes in the right screenshot represent the recursive decomposition of space into sub-quadrants by the quadtree data structure.</figcaption>
</div>
<br/>

This application simulates the trajectories of bodies in an N-body system. Its main features can be summarized as
follows:

- __Visualization__: The motion of particles is animated using the [SFML][sfml-url] library.
- __Parallelization__: The application is an [MPI][mpi-url] program that delegates the computation into a configurable
  number of MPI processes. The root process is responsible for updating the screen with the new positions of the
  particles. All processes share an array of objects of type `Particle`, an instance of which holds information such as
  mass, radius, position, velocity, and acceleration. Only one copy of this array exists and is shared among all
  processes using _MPI-3 shared memory programming_. This brings us into an important assumption that is made when
  running this application: __The application must be executed with MPI processes that all belong to the same memory
  domain__. In other words, when running this application in a multi-node cluster, restrict the execution to a single
  node in which all ranks in `MPI_COMM_WORLD` belong to same memory domain. Each process is assigned a contiguous
  segment of the array for updating particle data.
- __Optimization__: The application offers two modes for computing the acceleration of the particles. In the brute-force
  approach, the acceleration of a particle must be adjusted by the gravitational influence from each particle in the
  system. Since this must be done for each particle in the system, this results to a time complexity of O($n^{2}$). The
  second approach is a modification of the Barnes-Hut algorithm which uses a quadtree to group particles that are close
  to each other. When a group is sufficiently far away, it is treated as a single particle with a mass and position
  equal to the group's total mass and centre of mass, respectively. In this application, we only consider groups that
  are leaf nodes in the quadtree.
- __Mechanics__: The dynamics of the system is governed by Newton's law of gravity:
  <br>
  $$F = \frac{Gm_{1}m_{2}}{r^{2}}$$
  <br>
  and Newton's second law of motion $F = ma$. The position of a particle is updated using Euler's integration as
  follows:

  $$a_{n+1} = a_{n} + \Delta a$$
  <br>
  $$v_{n+1} = v_{n} + \Delta a_{n+1} \Delta t$$
  <br>
  $$p_{n+1} = p_{n} + \Delta v_{n+1} \Delta t$$

  where the value of $\Delta t$ is taken as 1 and $\Delta a$ is the vector sum of all acceleration change due to the
  gravitational influence of all other particles in the system.

<div align="center">
  <img class="animated-gif" src="img/n-body-tree.gif" width="400">
</div>

### Performance on N=1540 Bodies

We use the observed frame rate as a metric for the performance of the application. We conducted an experiment of running
the application for 1540 particles across varying number of processor cores. We compare the performance of the two
algorithms for computing the state of the particles: (1) Naive brute-force approach that has a time-complexity of O(
$n^{2}$) and (2) Quadtree approach inspired by the Barnes-Hut algorithm. The result of the experiment is plotted in
Figure 2.

<div align="center">
    <img src="img/plot.png" width="450">
    <br/>
    <figcaption>Figure 2. Frame rate versus number of cores for a problem size of 1540 particles.</figcaption>
</div>

<br/>

A few remarks can be made on the figure above:

- The quadtree approach outperforms the naive approach across any number of cores.
- Only the quadtree approach (when using 4 cores) achieves a frame rate above 24 Hz (the gold standard for film).
- For both the naive and quadtree approach, the frame rate peaks when using 4 cores. Above 4 cores, using more cores
  does not result to an improved performance. Further investigation on the cost of using MPI-3 shared memory programming
  is needed to explain this observation.

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

The application uses SFML for animation and MPI for distributing the workload across multiple processes. You can consult
the official websites of [SFML](sfml-url) and [MPICH](mpich-url) for installation instruction on your preferred OS. If
your local environment is Ubuntu, you can install these dependencies by running:

```bash
$ sudo apt-get install libsfml-dev
$ sudo apt install mpich
```

### Running the Application

Build the application

```bash
$ cd app
$ make
```

There are two ways to run the application:

1. Calling the target `run` of the `Makefile`
2. Launching the application using `mpiexec`

#### Running via the Makefile

The target `run` in the Makefile can take the following parameters:

| Parameter   |   Default   | Description                                                                                                                                                                                                                                                                                                                   |
|-------------|:-----------:|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `simulator` | `BarnesHut` | The algorithm used to compute the state (acceleration, velocity, and position) of the particles. Possible arguments are `Naive` and `BarnesHut`. The `Naive` algorithm has a time complexity of O($n^{2}$). When the argument is to `BarnesHut`, it uses a quadtree and an optimization inspired by the Barnes-Hut algorithm. |  
| `size`      |   `1000`    | The number of particles in the N-body system.                                                                                                                                                                                                                                                                                 |
| `cores`     |     `2`     | The number of MPI processes.                                                                                                                                                                                                                                                                                                  |
| `duration`  |    `0s`     | The argument to the `timeout` command. The default argument of `0s` means no timeout.                                                                                                                                                                                                                                         |

Here are sample run commands using the Makefile:

```bash
$ make run
$ make size=1540 cores=4 run
$ make simulator=BarnesHut duration=10 size=1540 cores=4 run
```

#### Launching via mpiexec

Here is a sample run command using `mpiexec`:

```bash
$ mpiexec -n 4 ./main -s Naive -n 1000
```

which uses 4 cores, the naive brute-force approach for computing the particle states and a problem size of 1000
particles. You can add the `-r` flag to draw the sub-quadrants when using the quadtree approach.

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[license-url]: LICENSE.txt

[linkedin-url]: https://www.linkedin.com/in/faerlin-pulido/

[sfml-url]: https://www.sfml-dev.org/

[mpi-url]: https://www.mpi-forum.org/

[mpich-url]: https://www.mpich.org/

[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555

<!-- ffmpeg -i Screencast\ from\ 2024-07-21\ 03\:49\:39\ PM.webm -pix_fmt rgb24 output.gif -->