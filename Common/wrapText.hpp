#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <tuple>

std::vector<std::string> wrapText(const sf::Font& font, const std::string& text, unsigned int characterSize, float maxWidth)
{
    std::vector<std::string> lines;
    std::istringstream words(text);
    std::string word;
    std::string currentLine;

    while (words >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;

        sf::Text testText(sf::String::fromUtf8(testLine.begin(), testLine.end()), font, characterSize);
        float width = testText.getLocalBounds().width;

        if (width <= maxWidth) {
            currentLine = testLine;
        } else {
            if (!currentLine.empty()) {
                lines.push_back(currentLine);
            }
            currentLine = word;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

std::tuple<sf::Text, std::string> createWrappedText(const sf::Font& font, const std::string& text, unsigned int characterSize, float maxWidth, float maxHeight)
{
    auto lines = wrapText(font, text, characterSize, maxWidth);
    std::string resultText = "";
    std::string wrapped = "";

    std::string reaminOfLines = "";

    for (int i = 0; i < lines.size(); i++) {
        if (i > 0) wrapped += "\n";
        wrapped += lines[i];

        sf::Text testText(wrapped, font, characterSize);
        float height = testText.getLocalBounds().height;
        if(height > maxHeight) {
            reaminOfLines += lines[i];
        }
        else {
            resultText = wrapped;
        }
    }
    
    sf::Text result(sf::String::fromUtf8(resultText.begin(), resultText.end()), font, characterSize);
    return std::make_tuple(result, reaminOfLines);
}