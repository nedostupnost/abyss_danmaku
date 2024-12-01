/**
 * @file ResourceManager.cpp
 * @brief Реализация менеджера ресурсов
 */
#include "ResourceManager.h"

sf::Texture& ResourceManager::loadTexture(const std::string& name, const std::string& filename) {
    auto [it, success] = textureCache.try_emplace(name);
    
    if (!it->second.loadFromFile(filename)) {
        textureCache.erase(it);
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    
    return it->second;
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    return textureCache.at(name);
}

sf::Font& ResourceManager::loadFont(const std::string& name, const std::string& filename) {
    auto [it, success] = fontCache.try_emplace(name);
    
    if (!it->second.loadFromFile(filename)) {
        fontCache.erase(it);
        throw std::runtime_error("Failed to load font: " + filename);
    }
    
    return it->second;
}

sf::Font& ResourceManager::getFont(const std::string& name) {
    return fontCache.at(name);
}

sf::SoundBuffer& ResourceManager::loadSoundBuffer(const std::string& name, const std::string& filename) {
    auto [it, success] = soundBufferCache.try_emplace(name);
    
    if (!it->second.loadFromFile(filename)) {
        soundBufferCache.erase(it);
        throw std::runtime_error("Failed to load sound: " + filename);
    }
    
    return it->second;
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) {
    return soundBufferCache.at(name);
}

sf::Music& ResourceManager::loadMusic(const std::string& name, const std::string& filename) {
    auto music = std::make_unique<sf::Music>();
    
    if (!music->openFromFile(filename)) {
        throw std::runtime_error("Failed to load music: " + filename);
    }
    
    musicCache[name] = std::move(music);
    return *musicCache[name];
}

sf::Music& ResourceManager::getMusic(const std::string& name) {
    return *musicCache.at(name);
}

void ResourceManager::clear() {
    textureCache.clear();
    fontCache.clear();
    soundBufferCache.clear();
    musicCache.clear();
}