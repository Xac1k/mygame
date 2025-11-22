#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <Common/buildPath.hpp>

class AudioSystem {
private:
    std::unordered_map<std::string, sf::Music> musicMap;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    
    float masterVolume = 100.0f;
    float musicVolume = 80.0f;
    float soundVolume = 90.0f;

public:
    bool loadMusic(const std::string& name, const std::string& filename) {
        if (musicMap.find(name) != musicMap.end()) {
            return true; 
        }
        
        std::string fullpath = buildFullPath(filename, 1);
        if (!musicMap[name].openFromFile(fullpath)) {
            return false;
        }
        
        musicMap[name].setVolume(musicVolume * (masterVolume / 100.0f));
        return true;
    }
    
    bool loadSound(const std::string& name, const std::string& filename) {
        if (soundBuffers.find(name) != soundBuffers.end()) {
            return true; 
        }
        
        sf::SoundBuffer buffer;
        std::string fullpath = buildFullPath(filename, 1);
        if (!buffer.loadFromFile(fullpath)) {
            return false;
        }
        
        soundBuffers[name] = buffer;
        
        sf::Sound sound;
        sound.setBuffer(soundBuffers[name]);
        sound.setVolume(soundVolume * (masterVolume / 100.0f));
        sounds[name] = sound;
        
        return true;
    }
    
    void playMusic(const std::string& name, bool loop = true) {
        auto it = musicMap.find(name);
        if (it != musicMap.end()) {
            it->second.setLoop(loop);
            it->second.play();
        }
    }
    
    void pauseMusic(const std::string& name) {
        auto it = musicMap.find(name);
        if (it != musicMap.end()) {
            it->second.pause();
        }
    }
    
    void stopMusic(const std::string& name) {
        auto it = musicMap.find(name);
        if (it != musicMap.end()) {
            it->second.stop();
        }
    }
    
    void playSound(const std::string& name) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            it->second.play();
        }
    }
    
    void setMasterVolume(float volume) {
        masterVolume = volume;
        updateVolumes();
    }
    
    void setMusicVolume(float volume) {
        musicVolume = volume;
        updateVolumes();
    }
    
    void setSoundVolume(float volume) {
        soundVolume = volume;
        updateVolumes();
    }
    
private:
    void updateVolumes() {
        for (auto& pair : musicMap) {
            pair.second.setVolume(musicVolume * (masterVolume / 100.0f));
        }
        
        for (auto& pair : sounds) {
            pair.second.setVolume(soundVolume * (masterVolume / 100.0f));
        }
    }
};