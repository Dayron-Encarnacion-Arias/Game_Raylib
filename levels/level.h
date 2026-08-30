#pragma once

#include <raylib.h>
#include <string>
#include <vector>

// Contiene la configuracion y el mapa procedural de un nivel.
class Level
{
public:
    Level(int id, std::string name, std::string location, std::string reward,
          Color primary, Color secondary, int targetLines, float fallTime);
    void DrawMap(float time, int screenWidth, int screenHeight) const;
    int GetId() const;
    const std::string &GetName() const;
    const std::string &GetLocation() const;
    const std::string &GetReward() const;
    Color GetPrimaryColor() const;
    Color GetSecondaryColor() const;
    int GetTargetLines() const;
    float GetFallTime() const;

private:
    int id;
    std::string name;
    std::string location;
    std::string reward;
    Color primary;
    Color secondary;
    int targetLines;
    float fallTime;
};

// Construye la campaña completa en el orden de desbloqueo.
std::vector<Level> GetAllLevels();
