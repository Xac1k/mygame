#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <tuple>

std::vector<std::string> wrapText( const sf::Font& font, const std::string& text, unsigned int characterSize, float maxWidth)
{
    std::vector<std::string> lines;
    std::string currentLine;
    std::size_t lastSpacePos = std::string::npos;

    sf::Text measure("", font, characterSize);

    for (int i = 0; i < text.size(); ++i) {
        char c = text[i];

        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine.clear();
            lastSpacePos = std::string::npos;
            continue;
        }

        currentLine += c;

        if (c == ' ' || c == '\t')
            lastSpacePos = currentLine.size() - 1;

        measure.setString(sf::String::fromUtf8(
            currentLine.begin(), currentLine.end()));

        if (measure.getLocalBounds().width > maxWidth) {
            if (lastSpacePos != std::string::npos) {
                lines.push_back(currentLine.substr(0, lastSpacePos));
                currentLine = currentLine.substr(lastSpacePos + 1);
            } else {
                lines.push_back(currentLine);
                currentLine.clear();
            }

            lastSpacePos = std::string::npos;
        }
    }

    if (!currentLine.empty())
        lines.push_back(currentLine);

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