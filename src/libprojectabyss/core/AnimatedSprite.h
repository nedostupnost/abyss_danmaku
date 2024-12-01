/**
 * @file AnimatedSprite.h
 * @brief Класс для создания анимированных спрайтов
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "BoundedSprite.h"

/**
 * @brief Режимы анимации
 */
enum class AnimationMode {
    LOOP,           ///< Зацикленная анимация
    ONCE,           ///< Анимация один раз
    REVERSE_LOOP    ///< Анимация с реверсом
};

class AnimatedSprite : public BoundedSprite {
private:
    sf::IntRect first_bound;       ///< Начальный прямоугольник текстуры
    sf::IntRect curr_bound;        ///< Текущий прямоугольник текстуры
    
    int curr_sprite;               ///< Текущий спрайт в анимации
    int num_sprites;               ///< Общее количество спрайтов
    
    int frame_counter;             ///< Счетчик кадров
    int max_frames;                ///< Максимальное количество кадров между сменой спрайтов
    
    AnimationMode animation_mode;  ///< Режим анимации
    bool animation_finished;       ///< Флаг завершения анимации
    bool reverse_animation;        ///< Флаг реверсивной анимации

    /**
     * @brief Переход к следующему спрайту
     */
    void next_sprite();

public:
    /**
     * @brief Конструктор по умолчанию
     */
    AnimatedSprite();

    /**
     * @brief Инициализация анимированного спрайта
     * 
     * @param orig Начальный прямоугольник текстуры
     * @param curr_sprite Начальный спрайт
     * @param num_sprites Количество спрайтов
     * @param max_frames Количество кадров между сменой спрайтов
     * @param mode Режим анимации
     */
    void init(
        sf::IntRect orig,
        int curr_sprite, 
        int num_sprites,
        int max_frames,
        AnimationMode mode = AnimationMode::LOOP
    );

    /**
     * @brief Обновление анимации
     */
    void tick();

    /**
     * @brief Проверка завершения анимации
     * 
     * @return true Анимация завершена
     * @return false Анимация продолжается
     */
    bool isAnimationFinished() const;

    /**
     * @brief Перезапуск анимации
     */
    void resetAnimation();
};
