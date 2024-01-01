[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<div align="center">
    <h3 align="center">N-Body Simulation</h3>
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
    <li><a href="#improvements">Improvements</a></li>
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

This application simulates the trajectories of bodies in an N-body system. Its main features can be summarized into four
categories:

- __Visualization__: The motion of particles is animated using the [SFML][sfml-url] library.
- __Parallelization__: The application is an [MPI][mpi-url] program that delegates the computation into a configurable
  number of processes.
- __Optimization__: The application offers two modes for computing the acceleration of the particles. In the brute-force
  approach, the acceleration of a particle must be adjusted by the gravitational influence from each particle in the
  system. Since this must be done for each particle in the system, this results to a time complexity of O($n^{2}$).

### Performance on N=1540 Bodies

<div align="center">
    <img src="img/plot.png" width="450">
</div>

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

### Running the Application

## Improvements

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[license-url]: LICENSE.txt

[linkedin-url]: https://www.linkedin.com/in/faerlin-pulido/

[sfml-url]: https://www.sfml-dev.org/

[mpi-url]: https://www.mpi-forum.org/

[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge

[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555