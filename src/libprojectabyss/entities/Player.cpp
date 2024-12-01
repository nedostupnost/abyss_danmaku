// Исправленный конструктор
Player::Player(const sf::Texture& texture, sf::Vector2f startPosition) 
    : AnimatedSprite()
{
    // Корректная установка текстуры и позиции
    setTexture(texture);
    setPosition(startPosition);

    // Инициализация анимации
    init(BOUND_IDLE, 0, 4, 10, AnimationMode::LOOP);
}

// Исправленный метод updateMovement
void Player::updateMovement(float deltaTime) {
    sf::Vector2f movement(0.f, 0.f);

    switch (currentState) {
        case State::MOVE_LEFT:
            movement.x -= speed * deltaTime;
            break;
        case State::MOVE_RIGHT:
            movement.x += speed * deltaTime;
            break;
        default:
            break;
    }

    // Обновление позиции с учетом границ экрана
    sf::Vector2f newPosition = getPosition() + movement;
    
    // Ограничение движения по горизонтали
    newPosition.x = std::max(0.f, std::min(newPosition.x, 800.f - getGlobalBounds().width));

    setPosition(newPosition);
}

// Исправленный метод handleInput
void Player::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
                currentState = State::MOVE_LEFT;
                break;
            case sf::Keyboard::Right:
                currentState = State::MOVE_RIGHT;
                break;
            case sf::Keyboard::Space:
                shoot();
                break;
            default:
                break;
        }
    }

    if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::Right:
                currentState = State::IDLE;
                break;
            default:
                break;
        }
    }
}

// Исправленный метод shoot
void Player::shoot() {
    if (shootCooldown <= 0) {
        // Создание пули с корректными параметрами
        bulletManager->createBullet(
            Bullet::Type::PLAYER,
            getPosition() + sf::Vector2f(getGlobalBounds().width / 2, 0),
            sf::Vector2f(0, -1),  // Направление вверх
            500.0f  // Скорость пули
        );

        // Сброс кулдауна
        shootCooldown = SHOOT_DELAY;
    }
}