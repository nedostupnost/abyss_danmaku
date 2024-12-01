/**
 * @file aLibs.h
 * @brief Центральный заголовочный файл для основных компонентов игры
 * 
 * Содержит включения основных классов и утилит проекта
 */
#pragma once

// Системные библиотеки
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>

// Локальные заголовочные файлы
#include "../entities/Bullet.h"
#include "../entities/Enemy.h"
#include "../entities/Player.h"
#include "AnimatedSprite.h"
#include "BoundedSprite.h"

// Пространство имен для общих утилит
namespace abyss {
    /**
     * @brief Генерация случайного числа в заданном диапазоне
     * 
     * @tparam T Тип числа (целочисленный или с плавающей точкой)
     * @param min Минимальное значение
     * @param max Максимальное значение
     * @return T Случайное число
     */
    template<typename T>
    T randomInRange(T min, T max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(gen);
        } else {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(gen);
        }
    }

    /**
     * @brief Проверка пересечения двух прямоугольников
     * 
     * @param rect1 Первый прямоугольник
     * @param rect2 Второй прямоугольник
     * @return true Прямоугольники пересекаются
     * @return false Прямоугольники не пересекаются
     */
    inline bool checkRectIntersection(
        const sf::FloatRect& rect1, 
        const sf::FloatRect& rect2
    ) {
        return rect1.intersects(rect2);
    }
}

// Макросы для упрощения отладки
#ifdef DEBUG
    #define ABYSS_LOG(x) std::cout << x << std::endl
    #define ABYSS_ASSERT(condition) assert(condition)
#else
    #define ABYSS_LOG(x)
    #define ABYSS_ASSERT(condition)
#endif