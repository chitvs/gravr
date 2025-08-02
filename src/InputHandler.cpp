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

#include "InputHandler.h"
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float PADDING = 10.0f;

float InputHandler::getMass(sf::RenderWindow& window, const sf::Font& font){

    float mass;
    bool inputDone = false;

    // Prompt
    sf::String promptString = "Enter the ball mass* (kg) and press Enter: ";
    sf::Text promptText(font, promptString);
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptTextBounds = promptText.getLocalBounds();
    sf::Vector2f promptTextOrigin(promptTextBounds.position.x + promptTextBounds.size.x/2, promptTextBounds.position.y + promptTextBounds.size.y/2);
    promptText.setOrigin(promptTextOrigin);
    sf::Vector2f promptTextPosition = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    promptText.setPosition(promptTextPosition);

    // PromptNote
    sf::String promptNoteString = "*Note: mass must be between 0.001 kg and 100 kg.";
    sf::Text promptNoteText(font, promptNoteString);
    promptNoteText.setCharacterSize(14);
    promptNoteText.setFillColor(sf::Color::White);
    sf::FloatRect promptNoteTextBounds = promptNoteText.getLocalBounds();
    sf::Vector2f promptNoteTextOrigin(promptNoteTextBounds.position.x, promptNoteTextBounds.position.y + promptNoteTextBounds.size.y);
    promptNoteText.setOrigin(promptNoteTextOrigin);
    sf::Vector2f promptNoteTextPosition = sf::Vector2f(PADDING, WINDOW_HEIGHT-PADDING);
    promptNoteText.setPosition(promptNoteTextPosition);

    // UserInput
    sf::String userInputString;
    sf::Text userInputText(font, userInputString);
    userInputText.setCharacterSize(24);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setOrigin(promptTextOrigin);
    sf::Vector2f userInputTextPosition = promptTextPosition + sf::Vector2f(promptTextBounds.size.x, 0);
    userInputText.setPosition(userInputTextPosition);

    while (!inputDone && window.isOpen())
    {
        window.clear();
        window.draw(promptText);
        window.draw(promptNoteText);
        window.draw(userInputText);
        window.display();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
                return 0.056; // Tennis ball mass
            }

            if (event->is<sf::Event::TextEntered>())
            {
                const auto* textEntered = event->getIf<sf::Event::TextEntered>();
                if (textEntered)
                {
                    char32_t unicode = textEntered->unicode;
                    if (unicode == 8 && !userInputString.isEmpty()) {
                        userInputString.erase(userInputString.getSize() - 1);
                    }
                    else if (unicode == '\r' || unicode == '\n') {
                        if (!userInputString.isEmpty()) {
                            try {
                                mass = std::stof(userInputString.toAnsiString());
                                if (mass >= 0.001f && mass <= 100.0f)
                                    inputDone = true;
                            } catch (...) {
                            }
                        }
                    }
                    else if ((unicode >= '0' && unicode <= '9') || unicode == '.') {
                        userInputString += unicode;
                    }
                    userInputText.setString(userInputString);
                }
            }
        }
    }

    if (!inputDone) {
        mass = 0.056f;
        std::cerr << "Invalid or incomplete input, using default mass (0.056 kg - tennis ball)\n";
    }    

    return mass;
}

float InputHandler::getHeight(sf::RenderWindow& window, const sf::Font& font){

    float height;
    bool inputDone = false;

    // Prompt
    sf::String promptString = "Enter the drop height* (m) and press Enter: ";
    sf::Text promptText(font, promptString);
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    sf::FloatRect promptTextBounds = promptText.getLocalBounds();
    sf::Vector2f promptTextOrigin(promptTextBounds.position.x + promptTextBounds.size.x/2, promptTextBounds.position.y + promptTextBounds.size.y/2);
    promptText.setOrigin(promptTextOrigin);
    sf::Vector2f promptTextPosition = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    promptText.setPosition(promptTextPosition);

    // PromptNote
    sf::String promptNoteString = "*Note: height must be between 1 m and 10 m for realistic simulation.";
    sf::Text promptNoteText(font, promptNoteString);
    promptNoteText.setCharacterSize(14);
    promptNoteText.setFillColor(sf::Color::White);
    sf::FloatRect promptNoteTextBounds = promptNoteText.getLocalBounds();
    sf::Vector2f promptNoteTextOrigin(promptNoteTextBounds.position.x, promptNoteTextBounds.position.y + promptNoteTextBounds.size.y);
    promptNoteText.setOrigin(promptNoteTextOrigin);
    sf::Vector2f promptNoteTextPosition = sf::Vector2f(PADDING, WINDOW_HEIGHT-PADDING);
    promptNoteText.setPosition(promptNoteTextPosition);

    // UserInput
    sf::String userInputString;
    sf::Text userInputText(font, userInputString);
    userInputText.setCharacterSize(24);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setOrigin(promptTextOrigin);
    sf::Vector2f userInputTextPosition = promptTextPosition + sf::Vector2f(promptTextBounds.size.x, 0);
    userInputText.setPosition(userInputTextPosition);

    while (!inputDone && window.isOpen())
    {
        window.clear();
        window.draw(promptText);
        window.draw(promptNoteText);
        window.draw(userInputText);
        window.display();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
                return 2.0f;
            }

            if (event->is<sf::Event::TextEntered>())
            {
                const auto* textEntered = event->getIf<sf::Event::TextEntered>();
                if (textEntered)
                {
                    char32_t unicode = textEntered->unicode;
                    if (unicode == 8 && !userInputString.isEmpty()) {
                        userInputString.erase(userInputString.getSize() - 1);
                    }
                    else if (unicode == '\r' || unicode == '\n') {
                        if (!userInputString.isEmpty()) {
                            try {
                                height = std::stof(userInputString.toAnsiString());
                                if (height >= 1.0f && height <= 10.0f)
                                    inputDone = true;
                            } catch (...) {
                            }
                        }
                    }
                    else if ((unicode >= '0' && unicode <= '9') || unicode == '.') {
                        userInputString += unicode;
                    }
                    userInputText.setString(userInputString);
                }
            }
        }
    }

    if (!inputDone) {
        height = 2.0f;
        std::cerr << "Invalid or incomplete input, using default height (2 m)\n";
    }    

    return height;
}
