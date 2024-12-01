// Исправленный конструктор
Enemy::Enemy(const sf::Texture& texture, Type type, sf::Vector2f startPosition) 
    : AnimatedSprite(), enemyType(type), currentState(State::ENTERING)
{
    // Корректная установка текстуры и позиции
    setTexture(texture);
    setPosition(startPosition);

    // Инициализация анимации в зависимости от типа
    switch (type) {
        case Type::BASIC:
            init(ENEMY_BOUND_IDLE, 0, 4, 10, AnimationMode::LOOP);
            health = 30.0f;
            speed = 100.0f;
            break;
        case Type::SHOOTER:
            init(ENEMY_BOUND_ATTACK, 0, 4, 8, AnimationMode::LOOP);
            health = 50.0f;
            speed = 75.0f;
            break;
        case Type::FAST:
            init(ENEMY_BOUND_IDLE, 0, 4, 15, AnimationMode::LOOP);
            health = 20.0f;
            speed = 200.0f;
            break;
        case Type::BOSS:
            init(ENEMY_BOUND_ATTACK, 0, 4, 5, AnimationMode::LOOP);
            health = 200.0f;
            speed = 50.0f;
            break;
    }
}

// Исправленный метод updateMovement
void Enemy::updateMovement(float deltaTime, const Player& player) {
    sf::Vector2f movement(0.f, 0.f);

    switch (enemyType) {
        case Type::BASIC:
            // Прямое движение вниз
            movement.y += speed * deltaTime;
            break;

        case Type::FAST:
            // Движение по синусоиде
            movement.x = std::sin(getPosition().y * 0.1f) * 50.0f * deltaTime;
            movement.y += speed * deltaTime;
            break;

        case Type::SHOOTER:
            // Движение с огибанием игрока
            {
                sf::Vector2f direction = player.getPosition() - getPosition();
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    direction /= length;
                    movement = direction * speed * deltaTime;
                }
            }
            break;

        case Type::BOSS:
            // Сложное движение
            movement.x = std::sin(getPosition().y * 0.05f) * 100.0f * deltaTime;
            movement.y += speed * deltaTime * 0.5f;
            break;
    }

    // Обновление позиции
    sf::Vector2f newPosition = getPosition() + movement;
    
    // Ограничение движения по экрану
    newPosition.x = std::max(0.f, std::min(newPosition.x, 800.f - getGlobalBounds().width));
    newPosition.y = std::max(0.f, std::min(newPosition.y, 600.f - getGlobalBounds().height));

    setPosition(newPosition);
}

// Исправленный метод shoot
void Enemy::shoot(const Player& player) {
    if (shootCooldown <= 0) {
        switch (enemyType) {
            case Type::SHOOTER:
                // Прямая стрельба в игрока
                bulletManager->createBullet(
                    Bullet::Type::ENEMY,
                    getPosition(),
                    (player.getPosition() - getPosition()),
                    300.0f
                );
                break;

            case Type::BOSS:
                // Веерная стрельба
                for (int i = -2; i <= 2; ++i) {
                    sf::Vector2f direction = player.getPosition() - getPosition();
                    float angle = std::atan2(direction.y, direction.x) + i * 0.2f;
                    bulletManager->createBullet(
                        Bullet::Type::ENEMY,
                        getPosition(),
                        sf::Vector2f(std::cos(angle), std::sin(angle)),
                        250.0f
                    );
                }
                break;

            default:
                break;
        }

        // Сброс кулдауна
        shootCooldown = SHOOT_DELAY;
    }
}