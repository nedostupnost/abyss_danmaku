#include "ResourceManager.h"

/**
 * @brief Деструктор ResourceManager
 * 
 * Освобождает все загруженные ресурсы
 */
ResourceManager::~ResourceManager() {
    clear();
    Logger::log(LogLevel::INFO, "ResourceManager: Все ресурсы освобождены");
}

/**
 * @brief Загрузка текстуры
 * 
 * Загружает текстуру из файла и сохраняет её в кэше
 * 
 * @param name Идентификатор текстуры
 * @param filepath Путь к файлу текстуры
 * @return true если загрузка успешна
 */
bool ResourceManager::loadTexture(const std::string& name, const std::string& filepath) {
    // Проверяем, не загружена ли уже текстура
    if (hasTexture(name)) {
        Logger::log(LogLevel::WARNING, "ResourceManager: Текстура '" + name + "' уже загружена");
        return true;
    }

    // Создаем новую текстуру
    auto texture = std::make_unique<sf::Texture>();
    
    // Пытаемся загрузить текстуру из файла
    if (!texture->loadFromFile(filepath)) {
        Logger::log(LogLevel::ERROR, "ResourceManager: Не удалось загрузить текстуру '" + name + 
                                   "' из файла: " + filepath);
        return false;
    }

    // Сохраняем текстуру в кэше
    textures[name] = std::move(texture);
    Logger::log(LogLevel::INFO, "ResourceManager: Текстура '" + name + "' успешно загружена");
    return true;
}

/**
 * @brief Загрузка шрифта
 * 
 * Загружает шрифт из файла и сохраняет его в кэше
 * 
 * @param name Идентификатор шрифта
 * @param filepath Путь к файлу шрифта
 * @return true если загрузка успешна
 */
bool ResourceManager::loadFont(const std::string& name, const std::string& filepath) {
    // Проверяем, не загружен ли уже шрифт
    if (hasFont(name)) {
        Logger::log(LogLevel::WARNING, "ResourceManager: Шрифт '" + name + "' уже загружен");
        return true;
    }

    // Создаем новый шрифт
    auto font = std::make_unique<sf::Font>();
    
    // Пытаемся загрузить шрифт из файла
    if (!font->loadFromFile(filepath)) {
        Logger::log(LogLevel::ERROR, "ResourceManager: Не удалось загрузить шрифт '" + name + 
                                   "' из файла: " + filepath);
        return false;
    }

    // Сохраняем шрифт в кэше
    fonts[name] = std::move(font);
    Logger::log(LogLevel::INFO, "ResourceManager: Шрифт '" + name + "' успешно загружен");
    return true;
}

/**
 * @brief Получение текстуры
 * 
 * @param name Идентификатор текстуры
 * @return Указатель на текстуру или nullptr, если текстура не найдена
 */
sf::Texture* ResourceManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second.get();
    }
    
    Logger::log(LogLevel::WARNING, "ResourceManager: Попытка получить несуществующую текстуру '" + name + "'");
    return nullptr;
}

/**
 * @brief Получение шрифта
 * 
 * @param name Идентификатор шрифта
 * @return Указатель на шрифт или nullptr, если шрифт не найден
 */
sf::Font* ResourceManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        return it->second.get();
    }
    
    Logger::log(LogLevel::WARNING, "ResourceManager: Попытка получить несуществующий шрифт '" + name + "'");
    return nullptr;
}

/**
 * @brief Проверка наличия текстуры
 */
bool ResourceManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}

/**
 * @brief Проверка наличия шрифта
 */
bool ResourceManager::hasFont(const std::string& name) const {
    return fonts.find(name) != fonts.end();
}

/**
 * @brief Удаление текстуры
 */
void ResourceManager::removeTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        textures.erase(it);
        Logger::log(LogLevel::INFO, "ResourceManager: Текстура '" + name + "' удалена");
    }
}

/**
 * @brief Удаление шрифта
 */
void ResourceManager::removeFont(const std::string& name) {
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        fonts.erase(it);
        Logger::log(LogLevel::INFO, "ResourceManager: Шрифт '" + name + "' удален");
    }
}

/**
 * @brief Очистка всех ресурсов
 */
void ResourceManager::clear() {
    textures.clear();
    fonts.clear();
    Logger::log(LogLevel::INFO, "ResourceManager: Все ресурсы очищены");
}

/**
 * @brief Инициализация базовых ресурсов игры
 * 
 * Загружает все необходимые текстуры и шрифты для игры
 * @return true если все ресурсы загружены успешно
 */
bool ResourceManager::initializeGameResources() {
    Logger::log(LogLevel::INFO, "ResourceManager: Начало загрузки игровых ресурсов");
    
    bool success = true;

    // Загрузка текстур
    success &= loadTexture("player", PLAYER_TEXTURE);
    success &= loadTexture("enemy", ENEMY_TEXTURE);
    success &= loadTexture("bullet", BULLET_TEXTURE);

    // Загрузка шрифтов
    success &= loadFont("main", FONT_PATH);

    if (success) {
        Logger::log(LogLevel::INFO, "ResourceManager: Все игровые ресурсы успешно загружены");
    } else {
        Logger::log(LogLevel::ERROR, "ResourceManager: Ошибка при загрузке игровых ресурсов");
    }

    return success;
}