#include "save_data.h"
#include <algorithm>
#include <fstream>

// Inicializa una partida nueva con el primer nivel y la opcion sin accesorio.
SaveData::SaveData()
{
    highestLevel = 0;
    accessoryMask = 1u;
    characterMask = 1u;
    selectedAccessory = 0;
    selectedColor = 0;
    selectedCharacter = 0;
    bestScores.fill(0);
}

// Lee el progreso y limita todos los indices para evitar datos invalidos.
void SaveData::Load(const std::string &path)
{
    std::ifstream file(path);
    if (!file) return;
    file >> highestLevel >> accessoryMask >> selectedAccessory >> selectedColor;
    for (int &score : bestScores) file >> score;
    // Los campos nuevos son opcionales para conservar compatibilidad con partidas anteriores.
    if (!(file >> characterMask >> selectedCharacter))
    {
        characterMask = 1u;
        selectedCharacter = 0;
    }
    highestLevel = std::max(0, std::min(9, highestLevel));
    selectedAccessory = std::max(0, std::min(10, selectedAccessory));
    selectedColor = std::max(0, std::min(4, selectedColor));
    accessoryMask |= 1u;
    characterMask |= 1u;
    selectedCharacter = std::max(0, std::min(1, selectedCharacter));
    if (!IsAccessoryUnlocked(selectedAccessory))
        selectedAccessory = 0;
    if (!IsCharacterUnlocked(selectedCharacter))
        selectedCharacter = 0;
}

// Escribe todo el progreso en una representacion de texto pequena y portable.
void SaveData::Save(const std::string &path) const
{
    std::ofstream file(path, std::ios::trunc);
    if (!file) return;
    file << highestLevel << ' ' << accessoryMask << ' '
         << selectedAccessory << ' ' << selectedColor << '\n';
    for (int score : bestScores) file << score << ' ';
    file << '\n' << characterMask << ' ' << selectedCharacter << '\n';
}

// Indica si el jugador ya puede entrar al nivel solicitado.
bool SaveData::IsLevelUnlocked(int level) const
{
    return level >= 0 && level <= highestLevel;
}

// Consulta el bit que representa un accesorio desbloqueado.
bool SaveData::IsAccessoryUnlocked(int accessory) const
{
    return accessory >= 0 && accessory <= 10 && (accessoryMask & (1u << accessory));
}

// Consulta el bit que representa un personaje base o exclusivo desbloqueado.
bool SaveData::IsCharacterUnlocked(int character) const
{
    return character >= 0 && character <= 1 && (characterMask & (1u << character));
}

// Abre el siguiente nivel y el accesorio correspondiente al nivel superado.
void SaveData::UnlockReward(int level)
{
    if (level >= 0 && level < 10) accessoryMask |= (1u << (level + 1));
    if (level < 9) highestLevel = std::max(highestLevel, level + 1);
}

// Conserva permanentemente el personaje exclusivo obtenido por probabilidad.
void SaveData::UnlockCharacter(int character)
{
    if (character >= 0 && character <= 1)
        characterMask |= (1u << character);
}

// Conserva solamente la mayor puntuacion obtenida en cada nivel.
void SaveData::RegisterScore(int level, int score)
{
    if (level >= 0 && level < 10) bestScores[level] = std::max(bestScores[level], score);
}
