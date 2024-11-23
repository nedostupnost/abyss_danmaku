#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation(sf::Texture& texture, const std::vector<int>& frameIndices, float switchTime)
        : m_texture(texture), m_frameIndices(frameIndices), m_switchTime(switchTime), 
          m_currentFrame(0), m_elapsedTime(0) {
        m_sprite.setTexture(m_texture);
        setTextureRect();
    }

    void update(float deltaTime) {
        m_elapsedTime += deltaTime;
        if (m_elapsedTime >= m_switchTime) {
            m_elapsedTime -= m_switchTime;
            m_currentFrame = (m_currentFrame + 1) % m_frameIndices.size();
            setTextureRect();
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_sprite);
    }

private:
    void setTextureRect() {
        int spriteWidth = m_texture.getSize().x / 8; // 8 спрайтов по горизонтали
        int spriteHeight = m_texture.getSize().y / 3; // 3 строки

        int frameIndex = m_frameIndices[m_currentFrame];
        int x = (frameIndex % 8) * spriteWidth; // вычисляем x-координату
        int y = (frameIndex / 8) * spriteHeight; // вычисляем y-координату

        m_sprite.setTextureRect(sf::IntRect(x, y, spriteWidth, spriteHeight));
    }

    sf::Sprite m_sprite;
    sf::Texture& m_texture;
    std::vector<int> m_frameIndices; // Индексы кадров для анимации
    float m_switchTime;
    int m_currentFrame;
    float m_elapsedTime;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animation Example");
    
    sf::Texture texture;
    texture.loadFromFile("spritesheet.png"); // Убедитесь, что путь к файлу правильный

    // Задайте индексы спрайтов, которые хотите использовать в анимации
    std::vector<int> frameIndices = {0, 1, 2, 3, 4, 5, 6, 7}; // Индексы спрайтов с первой строки
    // std::vector<int> frameIndices = {8, 9, 10, 11, 12, 13}; // Индексы спрайтов с второй строки
    // std::vector<int> frameIndices = {16, 17, 18, 19, 20, 21, 22, 23}; // Индексы спрайтов с третьей строки

    Animation animation(texture, frameIndices, 0.1f); // 8 кадров, смена каждые 0.1 секунды

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        animation.update(deltaTime);

        window.clear();
        animation.draw(window);
        window.display();
    }

    return 0;
}

