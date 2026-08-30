#pragma once

#include <array>
#include <string>

// Mantiene el progreso desbloqueado y lo serializa en un archivo de texto.
class SaveData
{
public:
    SaveData();
    void Load(const std::string &path);
    void Save(const std::string &path) const;
    bool IsLevelUnlocked(int level) const;
    bool IsAccessoryUnlocked(int accessory) const;
    void UnlockReward(int level);
    void RegisterScore(int level, int score);

    int highestLevel;
    unsigned int accessoryMask;
    int selectedAccessory;
    int selectedColor;
    std::array<int, 10> bestScores;
};
