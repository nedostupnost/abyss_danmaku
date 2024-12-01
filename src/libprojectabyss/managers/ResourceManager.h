/**
 * @file ResourceManager.h
 * @brief Менеджер ресурсов для загрузки и кэширования игровых ассетов
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

/**
 * @brief Класс для управления ресурсами игры
 * 
 * Обеспечивает централизованную загрузку, кэширование и освобождение ресурсов:
 * - Текстуры
 * - Шрифты
 * - Звуки
 * - Музыка
 */
class ResourceManager {
private:
    /// Кэш текстур
    std::unordered_map<std::string, sf::Texture> textureCache;

    /// Кэш шрифтов
    std::unordered_map<std::string, sf::Font> fontCache;

    /// Кэш звуковых буферов
    std::unordered_map<std::string, sf::SoundBuffer> soundBufferCache;

    /// Кэш музыкальных треков
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> musicCache;

public:
    /**
     * @brief Загрузка текстуры
     * 
     * @param name Уникальный идентификатор текстуры
     * @param filename Путь к файлу изображения
     * @return sf::Texture& Ссылка на загруженную текстуру
     * @throws std::runtime_error Если текстуру не удалось загрузить
     */
    sf::Texture& loadTexture(const std::string& name, const std::string& filename);

    /**
     * @brief Получение текстуры по имени
     * 
     * @param name Уникальный идентификатор текстуры
     * @return sf::Texture& Ссылка на текстуру
     * @throws std::out_of_range Если текстура не найдена
     */
    sf::Texture& getTexture(const std::string& name);

    /**
     * @brief Загрузка шрифта
     * 
     * @param name Уникальный идентификатор шрифта
     * @param filename Путь к файлу шрифта
     * @return sf::Font& Ссылка на загруженный шрифт
     * @throws std::runtime_error Если шрифт не удалось загрузить
     */
    sf::Font& loadFont(const std::string& name, const std::string& filename);

    /**
     * @brief Получение шрифта по имени
     * 
     * @param name Уникальный идентификатор шрифта
     * @return sf::Font& Ссылка на шрифт
     * @throws std::out_of_range Если шрифт не найден
     */
    sf::Font& getFont(const std::string& name);

    /**
     * @brief Загрузка звукового буфера
     * 
     * @param name Уникальный идентификатор звука
     * @param filename Путь к звуковому файлу
     * @return sf::SoundBuffer& Ссылка на загруженный звуковой буфер
     * @throws std::runtime_error Если звук не удалось загрузить
     */
    sf::SoundBuffer& loadSoundBuffer(const std::string& name, const std::string& filename);

    /**
     * @brief Получение звукового буфера по имени
     * 
     * @param name Уникальный идентификатор звука
     * @return sf::SoundBuffer& Ссылка на звуковой буфер
     * @throws std::out_of_range Если звук не найден
     */
    sf::SoundBuffer& getSoundBuffer(const std::string& name);

    /**
     * @brief Загрузка музыкального трека
     * 
     * @param name Уникальный идентификатор трека
     * @param filename Путь к музыкальному файлу
     * @return sf::Music& Ссылка на музыкальный трек
     * @throws std::runtime_error Если трек не удалось загрузить
     */
    sf::Music& loadMusic(const std::string& name, const std::string& filename);

    /**
     * @brief Получение музыкального трека по имени
     * 
     * @param name Уникальный идентификатор трека
     * @return sf::Music& Ссылка на музыкальный трек
     * @throws std::out_of_range Если трек не найден
     */
    sf::Music& getMusic(const std::string& name);

    /**
     * @brief Очистка всех кэшированных ресурсов
     */
    void clear();
};