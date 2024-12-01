#include "Logger.h"


// Инициализация статических членов класса
std::ofstream Logger::logFile;
std::string Logger::currentLogFileName;
bool Logger::initialized = false;

/**
 * @brief Инициализация системы логирования
 * 
 * Создает новый лог-файл с уникальным именем, основанным на текущей дате и времени.
 * Если директория logs не существует, она будет создана.
 * 
 * @return true если инициализация прошла успешно, false в случае ошибки
 */
bool Logger::init() {
    if (initialized) {
        return true;  // Предотвращаем повторную инициализацию
    }

    try {
        // Создаем директорию для логов, если её нет
        std::filesystem::create_directory("logs");

        // Генерируем имя файла на основе текущей даты и времени
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << "logs/game_log_" << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S") << ".txt";
        currentLogFileName = ss.str();

        // Открываем файл для записи
        logFile.open(currentLogFileName);
        if (!logFile.is_open()) {
            return false;
        }

        // Записываем заголовок лога
        logFile << "=== Abyss Danmaku Game Log ===" << std::endl;
        logFile << "Started at: " << getTimestamp() << std::endl;
        logFile << "================================" << std::endl;

        initialized = true;
        log(LogLevel::INFO, "Logging system initialized");
        return true;
    }
    catch (const std::exception& e) {
        // В случае ошибки пытаемся записать в стандартный поток ошибок
        std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Закрытие системы логирования
 * 
 * Записывает завершающее сообщение и закрывает файл лога.
 * Этот метод должен вызываться перед завершением программы.
 */
void Logger::close() {
    if (initialized && logFile.is_open()) {
        log(LogLevel::INFO, "Logging system shutting down");
        logFile << "=== End of Log ===" << std::endl;
        logFile.close();
        initialized = false;
    }
}

/**
 * @brief Запись сообщения в лог
 * 
 * Форматирует и записывает сообщение в лог-файл с временной меткой
 * и указанием уровня важности.
 * 
 * @param level Уровень важности сообщения
 * @param message Текст сообщения
 */
void Logger::log(LogLevel level, const std::string& message) {
    if (!initialized || !logFile.is_open()) {
        std::cerr << "Logger not initialized!" << std::endl;
        return;
    }

    try {
        // Формируем строку лога
        std::stringstream logStream;
        logStream << "[" << getTimestamp() << "] "
                 << "[" << levelToString(level) << "] "
                 << message;

        // Записываем в файл
        logFile << logStream.str() << std::endl;
        logFile.flush();  // Принудительная запись буфера

        // Для критических сообщений дублируем вывод в консоль
        if (level >= LogLevel::ERROR) {
            std::cerr << logStream.str() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Logging failed: " << e.what() << std::endl;
    }
}

/**
 * @brief Перегруженная версия метода log для обратной совместимости
 * 
 * Конвертирует строковое представление уровня логирования в enum
 * и вызывает основной метод log.
 * 
 * @param levelStr Строковое представление уровня логирования
 * @param message Текст сообщения
 */
void Logger::log(const std::string& levelStr, const std::string& message) {
    LogLevel level;
    
    // Определяем уровень логирования из строки
    if (levelStr == "DEBUG")       level = LogLevel::DEBUG;
    else if (levelStr == "INFO")   level = LogLevel::INFO;
    else if (levelStr == "WARNING") level = LogLevel::WARNING;
    else if (levelStr == "ERROR")  level = LogLevel::ERROR;
    else if (levelStr == "FATAL")  level = LogLevel::FATAL;
    else level = LogLevel::INFO;  // По умолчанию используем INFO

    log(level, message);
}