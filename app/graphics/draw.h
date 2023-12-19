#pragma once

#include <SFML/Graphics.hpp>
#include "quadtree/node.h"
#include "math/particle.h"
#include "math/region.h"
#include "quadtree/node.h"
#include "simulator/data.h"

namespace draw {

    void particles(Particle *particles, int size, sf::RenderWindow *window);

    void drawRegion(Node<PointData, NodeData> *node, sf::RenderWindow *window);

    void drawRectangle(Region::Rectangle &rect, sf::RenderWindow *window);

};