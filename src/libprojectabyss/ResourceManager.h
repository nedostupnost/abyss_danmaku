#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include "Logger.h"

/**
 * @file ResourceManager.h
 * @brief Менеджер ресурсов игры
 * 
 * Singleton класс, отвечающий за:
 * - Загрузку и хранение текстур
 * - Загрузку и хранение шрифтов
 * - Кэширование ресурсов
 * - Освобождение ресурсов при завершении работы
 */
class ResourceManager {
public:
    /**
     * @brief Получение единственного экземпляра класса
     * @return Ссылка на экземпляр ResourceManager
     */
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Запрещаем копирование и присваивание
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    /**
     * @brief Загрузка текстуры
     * 
     * @param name Идентификатор текстуры
     * @param filepath Путь к файлу текстуры
     * @return true если загрузка успешна
     */
    bool loadTexture(const std::string& name, const std::string& filepath);

    /**
     * @brief Загрузка шрифта
     * 
     * @param name Идентификатор шрифта
     * @param filepath Путь к файлу шрифта
     * @return true если загрузка успешна
     */
    bool loadFont(const std::string& name, const std::string& filepath);

    /**
     * @brief Получение текстуры
     * 
     * @param name Идентификатор текстуры
     * @return Указатель на текстуру или nullptr, если текстура не найдена
     */
    sf::Texture* getTexture(const std::string& name);

    /**
     * @brief Получение шрифта
     * 
     * @param name Идентификатор шрифта
     * @return Указатель на шрифт или nullptr, если шрифт не найден
     */
    sf::Font* getFont(const std::string& name);

    /**
     * @brief Проверка наличия текстуры
     * 
     * @param name Идентификатор текстуры
     * @return true если текстура загружена
     */
    bool hasTexture(const std::string& name) const;

    /**
     * @brief Проверка наличия шрифта
     * 
     * @param name Идентификатор шрифта
     * @return true если шрифт загружен
     */
    bool hasFont(const std::string& name) const;

    /**
     * @brief Удаление текстуры
     * 
     * @param name Идентификатор текстуры
     */
    void removeTexture(const std::string& name);

    /**
     * @brief Удаление шрифта
     * 
     * @param name Идентификатор шрифта
     */
    void removeFont(const std::string& name);

    /**
     * @brief Очистка всех ресурсов
     */
    void clear();

    /**
     * @brief Инициализация базовых ресурсов игры
     * 
     * Загружает все необходимые текстуры и шрифты
     * @return true если все ресурсы загружены успешно
     */
    bool initializeGameResources();

private:
    ResourceManager() = default;  // Приватный конструктор для паттерна Singleton
    ~ResourceManager();

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures; ///< Хранилище текстур
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;      ///< Хранилище шрифтов

    // Константы путей к ресурсам
    const std::string PLAYER_TEXTURE = "assets/image/reimu.png";
    const std::string ENEMY_TEXTURE = "assets/image/enemy.png";
    const std::string BULLET_TEXTURE = "assets/image/bullet.png";
    const std::string FONT_PATH = "assets/arial.ttf";
};