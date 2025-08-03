/*
 * Gravr -- Simulating physics with SFML.
 * 
 * MIT License
 * Copyright (c) 2025 Alessandro Chitarrini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Particle.h"
#include <cmath>

const float PIXELS_PER_M = 57.78f;
const float AIR_DENSITY = 1.225f;
const float AIR_VISCOSITY = 1.81e-5f;
const float PI = 3.14159265359f;
const float BALL_RADIUS = 0.04f; // 4cm fixed size
const float DRAG_MULTIPLIER = 8.0f; // Increase to exaggerate drag effect

Particle::Particle(float x, float y, float mass)
    : position(x, y), velocity(0, 0), acceleration(0, 0), mass(mass) {}

void Particle::applyForce(const sf::Vector2f& force) {
    acceleration += force / mass;
}

void Particle::update(float dt) {
    velocity += acceleration * dt;
    position += velocity * dt;
    acceleration = sf::Vector2f(0, 0);
}

float Particle::calculateCrossSection() const {
    return PI * BALL_RADIUS * BALL_RADIUS;
}

float Particle::calculateReynoldsNumber(float speed) const {
    float diameter = 2.0f * BALL_RADIUS;
    return (AIR_DENSITY * speed * diameter) / AIR_VISCOSITY;
}

float Particle::calculateDragCoefficient() const {
    float speedPixels = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speedPixels < 0.01f) return 0.47f;

    float speedMeters = speedPixels / PIXELS_PER_M;
    float reynolds = calculateReynoldsNumber(speedMeters);

    if (reynolds < 0.1f)
        return 24.0f / std::max(reynolds, 0.001f);
    else if (reynolds < 1000.0f)
        return 24.0f / reynolds * (1.0f + 0.15f * std::pow(reynolds, 0.687f));
    else if (reynolds < 300000.0f)
        return 0.44f;
    else
        return 0.1f;
}

void Particle::applyDrag(float deltaTime) {
    float speedPixels = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speedPixels < 0.01f) return;

    float speedMeters = speedPixels / PIXELS_PER_M;
    float Cd = calculateDragCoefficient();
    float A = calculateCrossSection();
    
    float drag = 0.5f * AIR_DENSITY * speedMeters * speedMeters * Cd * A * DRAG_MULTIPLIER;
    sf::Vector2f dragForce = -drag * (velocity / speedPixels) * PIXELS_PER_M;

    applyForce(dragForce);
}
