/**
 * @file ParticleManager.cpp
 * @brief Реализация менеджера систем частиц
 */
#include "ParticleManager.h"

void ParticleManager::createParticles(
    const sf::Vector2f& position, 
    ParticleType type, 
    size_t count
) {
    // Проверка превышения максимального количества частиц
    if (particles.size() + count > MAX_PARTICLES) {
        cleanupParticles();
    }

    // Создание частиц в зависимости от типа
    switch (type) {
        case ParticleType::EXPLOSION:
            createExplosionParticles(position, count);
            break;
        case ParticleType::BULLET_TRAIL:
            createBulletTrailParticles(position, count);
            break;
        default:
            break;
    }
}

void ParticleManager::update(float deltaTime) {
    // Обновление состояния каждой частицы
    for (auto& particle : particles) {
        particle.shape.move(particle.velocity * deltaTime);
        particle.lifetime -= deltaTime;

        // Изменение прозрачности по мере затухания
        sf::Color color = particle.shape.getFillColor();
        color.a = static_cast<sf::Uint8>(255 * (particle.lifetime / 1.0f));
        particle.shape.setFillColor(color);
    }

    // Удаление устаревших частиц
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.lifetime <= 0; }),
        particles.end()
    );
}

void ParticleManager::render(sf::RenderWindow& window) {
    // Отрисовка всех активных частиц
    for (const auto& particle : particles) {
        window.draw(particle.shape);
    }
}

void ParticleManager::createExplosionParticles(
    const sf::Vector2f& position, 
    size_t count
) {
    std::uniform_real_distribution<float> angleDistribution(0, 2 * M_PI);
    std::uniform_real_distribution<float> speedDistribution(50.0f, 200.0f);

    for (size_t i = 0; i < count; ++i) {
        Particle particle;
        particle.shape.setRadius(2.0f);
        particle.shape.setPosition(position);
        
        // Случайное направление и скорость
        float angle = angleDistribution(randomGenerator);
        float speed = speedDistribution(randomGenerator);
        
        particle.velocity = sf::Vector2f(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        );

        // Случайный цвет взрыва
        particle.shape.setFillColor(sf::Color(
            255, 
            std::rand() % 128 + 128, 
            0, 
            255
        ));

        particle.lifetime = 1.0f;
        particle.type = ParticleType::EXPLOSION;

        particles.push_back(particle);
    }
}

void ParticleManager::createBulletTrailParticles(
    const sf::Vector2f& position, 
    size_t count
) {
    for (size_t i = 0; i < count; ++i) {
        Particle particle;
        particle.shape.setRadius(1.0f);
        particle.shape.setPosition(position);
        
        // Небольшой разброс
        particle.velocity = sf::Vector2f(
            (std::rand() % 20 - 10) * 0.5f,
            10.0f
        );

        particle.shape.setFillColor(sf::Color(200, 200, 200, 128));
        particle.lifetime = 0.5f;
        particle.type = ParticleType::BULLET_TRAIL;

        particles.push_back(particle);
    }
}

void ParticleManager::cleanupParticles() {
    // Удаление старых частиц, оставляя новые
    particles.erase(
        particles.begin(), 
        particles.begin() + particles.size() / 2
    );
}