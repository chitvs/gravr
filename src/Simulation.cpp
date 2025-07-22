/*
 * Gravr - Simulating physics with SFML
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

#include "Simulation.h"
#include "InputHandler.h"
#include <sstream>
#include <iomanip>
#include <cmath>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float PIXELS_PER_M = 57.78f;
const float GRAVITY = 9.81f * PIXELS_PER_M;
const int PARTICLE_SIZE = 12;
int PARTICLE_PIXELS_HEIGHT = WINDOW_HEIGHT / 2;
const float BASE_Y = WINDOW_HEIGHT - PARTICLE_SIZE;

Simulation::Simulation(sf::RenderWindow& window, const sf::Font& font)
    : window(window), font(font), uiManager(font),
      simulationFinished(false), gameStarted(false), isPaused(false), timeString(""),
      finishedTimeString(""), particle(0, 0, 1.0f),
      hasTouchedGround(false), timeToFirstContact(0.0f) {

    float mass = InputHandler::getMass(window, font);
    float height = InputHandler::getHeight(window, font);

    PARTICLE_PIXELS_HEIGHT = BASE_Y - (height * PIXELS_PER_M);

    particle = Particle(WINDOW_WIDTH / 2, PARTICLE_PIXELS_HEIGHT, mass);

    particleShape = sf::CircleShape(PARTICLE_SIZE);
    particleShape.setFillColor(sf::Color::Red);
    sf::Vector2f particleOrigin(PARTICLE_SIZE, PARTICLE_SIZE);
    particleShape.setOrigin(particleOrigin);
    particleShape.setPosition(sf::Vector2f(WINDOW_WIDTH / 2, PARTICLE_PIXELS_HEIGHT));

    uiManager.setupUI(mass, height);
}

void Simulation::resetSimulation() {
    particle.position = sf::Vector2f(WINDOW_WIDTH / 2, PARTICLE_PIXELS_HEIGHT);
    particle.velocity = sf::Vector2f(0, 0);
    hasTouchedGround = false;
    timeToFirstContact = 0.0f;
    totalClock.restart();
    frameClock.restart();
}

void Simulation::run() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !gameStarted) {
            gameStarted = true;
            resetSimulation();
        }

        if (gameStarted) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) {
                resetSimulation();
                isPaused = false;
                simulationFinished = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace) && !simulationFinished) {
                isPaused = true;
                totalClock.stop();
                frameClock.stop();
            }

            if (isPaused) {
                if (isPaused) {
                window.clear();
                window.draw(particleShape);
                uiManager.drawSimulationUI(window, particle);
                uiManager.drawTime(window, particle, timeString);
                uiManager.drawPauseScreen(window);
                window.display();
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    isPaused = false;
                    totalClock.start();
                    frameClock.start();
                }
            }
            }

            if (!isPaused && !simulationFinished) {
                float deltaTime = frameClock.restart().asSeconds();

                sf::Vector2f gravityForce(0.f, GRAVITY * particle.mass);
                particle.applyForce(gravityForce);
                particle.applyDrag(deltaTime);
                particle.update(deltaTime);

                float time = totalClock.getElapsedTime().asSeconds();
                        std::ostringstream sst;
                        sst << std::fixed << std::setprecision(2) << time << " s";
                        timeString = sst.str();

                if (particle.position.y > BASE_Y) {
                    particle.position.y = BASE_Y;

                    float impactSpeed = std::abs(particle.velocity.y);

                    if (!hasTouchedGround) {
                        hasTouchedGround = true;
                        timeToFirstContact = totalClock.getElapsedTime().asSeconds();
                    }

                    if (impactSpeed > 2.0f) {
                        float COR = 0.7f;
                        particle.velocity.y = -particle.velocity.y * COR;
                    } else {
                        particle.velocity.y = 0.f;
                        simulationFinished = true;

                        float totalTime = totalClock.getElapsedTime().asSeconds();
                        std::ostringstream ss;
                        ss << std::fixed << std::setprecision(2)
                           << "First contact: " << timeToFirstContact << " s | "
                           << "Total time: " << totalTime << " s";
                        finishedTimeString = ss.str();
                    }
                }

                particleShape.setPosition(particle.position);

                window.clear();
                window.draw(particleShape);
                uiManager.drawSimulationUI(window, particle);
                uiManager.drawTime(window, particle, timeString);
                window.display();
            } else if (simulationFinished) {
                window.clear();
                window.draw(particleShape);
                uiManager.drawFinishedScreen(window, particle, finishedTimeString);
                window.display();
            }
        } else {
            uiManager.drawStartScreen(window);
        }
    }
}
