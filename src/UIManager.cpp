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

#include "UIManager.h"
#include "Simulation.h"
#include <sstream>
#include <iomanip>
#include <cmath>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float PIXELS_PER_M = 57.78f;
const float PADDING = 10.0f;
const int PARTICLE_SIZE = 12;
const float BASE_Y = WINDOW_HEIGHT - PARTICLE_SIZE;

UIManager::UIManager(const sf::Font& font) : font(font),
    stopText(font, "Press Backspace to stop the simulation, 0 to reset, Esc to quit"),
    resumeText(font, "Press Enter to resume the simulation"),
    massText(font, ""),
    heightStartedText(font, ""),
    startText(font, "Press Enter to start the simulation") {
    
    // Stop
    stopText.setCharacterSize(20);
    stopText.setFillColor(sf::Color::White);
    sf::FloatRect stopTextBounds = stopText.getLocalBounds();
    sf::Vector2f stopTextOrigin(stopTextBounds.position.x + stopTextBounds.size.x/2, stopTextBounds.position.y);
    stopText.setOrigin(stopTextOrigin);
    sf::Vector2f stopTextPosition = sf::Vector2f(WINDOW_WIDTH/2, PADDING);
    stopText.setPosition(stopTextPosition);

    // Resume
    resumeText.setCharacterSize(24);
    resumeText.setFillColor(sf::Color::White);
    sf::FloatRect resumeTextBounds = resumeText.getLocalBounds();
    sf::Vector2f resumeTextOrigin(resumeTextBounds.position.x + resumeTextBounds.size.x/2, resumeTextBounds.position.y + resumeTextBounds.size.y/2);
    resumeText.setOrigin(resumeTextOrigin);
    sf::Vector2f resumeTextPosition = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    resumeText.setPosition(resumeTextPosition);

    // Start
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color::White);
    sf::FloatRect startTextBounds = startText.getLocalBounds();
    sf::Vector2f startTextOrigin(startTextBounds.position.x + startTextBounds.size.x/2, startTextBounds.position.y + startTextBounds.size.y/2);
    startText.setOrigin(startTextOrigin);
    sf::Vector2f startTextPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    startText.setPosition(startTextPosition);
}

void UIManager::setupUI(float mass, float height) {
    // Mass and ball info
    std::ostringstream massStream;
    massStream << std::fixed << std::setprecision(3) << mass;
    std::string ballType;
    if (mass <= 0.003f) // ~2.7g for a standard ping pong ball
        ballType = " (ping pong)";
    else if (mass <= 0.05f) // ~40g for a standard racquet ball
        ballType = " (racquet ball)";
    else if (mass <= 0.065f) // ~56-59.4g for a standard tennis ball
        ballType = " (tennis ball)";
    else if (mass <= 0.16f) // ~142-149g for a standard baseball
        ballType = " (baseball)";
    else if (mass <= 0.5f) // ~410-450g for a standard size 5 soccer ball
        ballType = " (soccer ball)";
    else if (mass <= 0.7f) // ~567-650g for a standard size 7 basketball
        ballType = " (basketball)";
    else if (mass <= 8.0f) // Up to ~7.26kg for a heavy bowling ball or shot put
        ballType = " (bowling ball)";
    else if (mass <= 20.0f) // Common range for heavier medicine balls
        ballType = " (light medicine ball)";
    else if (mass <= 50.0f) // Range for very heavy medicine balls or specialized training balls
        ballType = " (heavy medicine ball)";
    else if (mass <= 100.0f) // Up to 100kg for extremely heavy training balls or industrial spherical objects
        ballType = " (industrial ball)";
    else // For any mass greater than 100kg
        ballType = " (extremely heavy / undefined ball)";
    
    std::string massString = "Mass: " + massStream.str() + " kg" + ballType;
    massText = sf::Text(font, massString);
    massText.setCharacterSize(20);
    massText.setFillColor(sf::Color::White);
    sf::FloatRect massTextBounds = massText.getLocalBounds();
    sf::Vector2f massTextOrigin(massTextBounds.position.x, massTextBounds.position.y + massTextBounds.size.y);
    massText.setOrigin(massTextOrigin);
    sf::Vector2f massTextPosition = sf::Vector2f(PADDING, WINDOW_HEIGHT - PADDING);
    massText.setPosition(massTextPosition);

    std::ostringstream heightStartedStream;
    heightStartedStream << std::fixed << std::setprecision(3) << height;

    std::string heightStartedString = "Height: " + heightStartedStream.str() + " m";
    heightStartedText = sf::Text(font, heightStartedString);
    heightStartedText.setCharacterSize(20);
    heightStartedText.setFillColor(sf::Color::White);
    sf::FloatRect heightStartedTextBounds = heightStartedText.getLocalBounds();
    sf::Vector2f heightStartedTextOrigin(heightStartedTextBounds.position.x, heightStartedTextBounds.position.y + heightStartedTextBounds.size.y);
    heightStartedText.setOrigin(heightStartedTextOrigin);
    sf::Vector2f heightStartedTextPosition = sf::Vector2f(PADDING, WINDOW_HEIGHT - PADDING*4);
    heightStartedText.setPosition(heightStartedTextPosition);
}

void UIManager::drawStartScreen(sf::RenderWindow& window) {
    window.clear();
    window.draw(startText);
    window.display();
}

void UIManager::drawSimulationUI(sf::RenderWindow& window, const Particle& particle) {
    int digitsAfterComma = 2;

    // Velocity
    std::stringstream ssVelocity;
    ssVelocity << std::fixed << std::setprecision(digitsAfterComma) << (particle.velocity.y / PIXELS_PER_M);
    std::string velocityString = ssVelocity.str() + " m/s";
    sf::Text velocityText(font, velocityString);
    velocityText.setCharacterSize(24);
    velocityText.setFillColor(sf::Color::Red);
    sf::Vector2f velocityTextPosition = particle.position + sf::Vector2f(PARTICLE_SIZE, -PARTICLE_SIZE);
    velocityText.setPosition(velocityTextPosition);

    // Height
    std::stringstream ssHeight;
    float height = -((particle.position.y - BASE_Y) / PIXELS_PER_M);
    ssHeight << std::fixed << std::setprecision(digitsAfterComma) << (height);
    std::string heightString = ssHeight.str() + " m";
    sf::Text heightText(font, heightString);
    heightText.setCharacterSize(24);
    heightText.setFillColor(sf::Color::Red);
    sf::Vector2f heightTextPosition = particle.position + sf::Vector2f(PARTICLE_SIZE, -PARTICLE_SIZE-20);
    heightText.setPosition(heightTextPosition);

    window.draw(velocityText);
    window.draw(heightText);
    window.draw(stopText);
    window.draw(massText);
    window.draw(heightStartedText);
}

void UIManager::drawPauseScreen(sf::RenderWindow& window) {
    window.draw(resumeText);
    window.display();
}

void UIManager::drawTime(sf::RenderWindow& window, const Particle& particle, const std::string& timeString) {
    sf::Text timeText(font, timeString);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::Green);
    sf::FloatRect timeTextBounds = timeText.getLocalBounds();
    sf::Vector2f timeTextOrigin(timeTextBounds.position.x, timeTextBounds.position.y);
    timeText.setOrigin(timeTextOrigin);
    sf::Vector2f timeTextPosition = sf::Vector2f(PADDING, PADDING);
    timeText.setPosition(timeTextPosition);

    window.draw(timeText);
}

void UIManager::drawFinishedScreen(sf::RenderWindow& window, const Particle& particle, const std::string& finishedTimeString) {
    sf::Text finishedTimeText(font, finishedTimeString);
    finishedTimeText.setCharacterSize(24);
    finishedTimeText.setFillColor(sf::Color::Green);
    sf::FloatRect finishedTimeTextBounds = finishedTimeText.getLocalBounds();
    sf::Vector2f finishedTimeTextOrigin(finishedTimeTextBounds.position.x + finishedTimeTextBounds.size.x/2, finishedTimeTextBounds.position.y + finishedTimeTextBounds.size.y/2);
    finishedTimeText.setOrigin(finishedTimeTextOrigin);
    sf::Vector2f finishedTimeTextPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    finishedTimeText.setPosition(finishedTimeTextPosition);

    window.draw(stopText);
    window.draw(massText);
    window.draw(heightStartedText);
    window.draw(finishedTimeText);
}