#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>

/**
 * @brief Перечисление уровней логирования
 * 
 * DEBUG - детальная информация для отладки
 * INFO - общая информация о работе программы
 * WARNING - предупреждения, не влияющие на работу программы
 * ERROR - ошибки, влияющие на работу программы
 * FATAL - критические ошибки, требующие немедленного завершения программы
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

/**
 * @brief Класс для логирования событий игры
 * 
 * Singleton класс, обеспечивающий:
 * - Создание лог-файлов с временными метками
 * - Логирование событий разных уровней важности
 * - Автоматическое создание директории для логов
 * - Форматированный вывод с временными метками
 */
class Logger {
private:
    static std::ofstream logFile;           ///< Файловый поток для записи логов
    static std::string currentLogFileName;  ///< Имя текущего лог-файла
    static bool initialized;                ///< Флаг инициализации логгера
    
    /**
     * @brief Получение текущей временной метки
     * @return Строка с текущим временем в формате YYYY-MM-DD HH:MM:SS
     */
    static std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    /**
     * @brief Преобразование уровня логирования в строку
     * @param level Уровень логирования
     * @return Строковое представление уровня логирования
     */
    static std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            case LogLevel::FATAL:   return "FATAL";
            default:                return "UNKNOWN";
        }
    }

    // Приватный конструктор для паттерна Singleton
    Logger() = default;

public:
    /**
     * @brief Инициализация системы логирования
     * 
     * Создает директорию для логов и новый лог-файл с временной меткой
     * @return true если инициализация прошла успешно
     */
    static bool init();

    /**
     * @brief Закрытие системы логирования
     * 
     * Закрывает текущий лог-файл и освобождает ресурсы
     */
    static void close();

    /**
     * @brief Запись сообщения в лог
     * @param level Уровень важности сообщения
     * @param message Текст сообщения
     */
    static void log(LogLevel level, const std::string& message);

    /**
     * @brief Запись сообщения в лог (перегрузка для обратной совместимости)
     * @param levelStr Строковое представление уровня важности
     * @param message Текст сообщения
     */
    static void log(const std::string& levelStr, const std::string& message);

    /**
     * @brief Получение имени текущего лог-файла
     * @return Имя текущего лог-файла
     */
    static const std::string& getCurrentLogFileName() { return currentLogFileName; }
};