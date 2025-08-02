/*
 * Gravr -- Simulating physics with SFML
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

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "Particle.h"

class UIManager {
private:
    const sf::Font& font;
    sf::Text stopText;
    sf::Text resumeText;
    sf::Text massText;
    sf::Text heightStartedText;
    sf::Text startText;

public:
    UIManager(const sf::Font& font);
    void setupUI(float mass, float height);
    void drawStartScreen(sf::RenderWindow& window);
    void drawSimulationUI(sf::RenderWindow& window, const Particle& particle);
    void drawPauseScreen(sf::RenderWindow& window);
    void drawTime(sf::RenderWindow& window, const Particle& particle, const std::string& timeString);
    void drawFinishedScreen(sf::RenderWindow& window, const Particle& particle, const std::string& finishedTimeString);
};

#endif