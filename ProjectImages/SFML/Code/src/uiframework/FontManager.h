#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "iostream"

class FontManager {
public:
    static sf::Font& GetFont() {
        static sf::Font Font;
        static bool loaded = false;
        if (!loaded) {
            if (!Font.loadFromFile("../../resources/fonts/8bit16.ttf")) {
                std::cerr << "Error loading font" << std::endl;
            }
            loaded = true;
        }
        return Font;
    }
};
