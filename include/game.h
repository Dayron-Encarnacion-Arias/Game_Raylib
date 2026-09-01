#pragma once

#include "blocks.h"
#include "character.h"
#include "grid.h"
#include "level.h"
#include "save_data.h"
#include <array>
#include <vector>

enum class GameScreen { Title, LevelSelect, Customize, Playing, Result };
enum class Difficulty { Easy, Normal, Hard };

// Coordina pantallas, reglas, progreso y recursos graficos del juego completo.
class Game
{
public:
    Game(bool smokeTest = false);
    ~Game();
    void Update();
    void Draw();
    bool IsSmokeTestFinished() const;

private:
    void UpdateTitle();
    void UpdateLevelSelect();
    void UpdateCustomize();
    void UpdatePlaying();
    void UpdateResult();
    void HandlePlayingInput();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    void RotateBlock();
    bool IsBlockOutside() const;
    bool BlockFits() const;
    void LockBlock();
    void StartLevel();
    void CompleteLevel();
    void EndGame();
    void ResetBoard();
    void EnterTryHard();
    void ExitTryHard();
    bool IsLevelAvailable(int level) const;
    bool IsAccessoryAvailable(int accessory) const;
    bool IsCharacterAvailable(int character) const;
    int GetExclusiveDropChance() const;
    void UpdateScore(int clearedRows, int dropPoints);
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks() const;

    void DrawScene() const;
    void DrawTitle() const;
    void DrawLevelSelect() const;
    void DrawCustomize() const;
    void DrawPlaying() const;
    void DrawResult() const;
    void DrawPanel(Rectangle rectangle, Color color, bool selected = false) const;
    void DrawCentered(const char *text, int centerX, int y, int size, Color color) const;
    void DrawHeader(const char *title, const char *hint) const;
    void DrawNextBlock(const Block &block, int x, int y) const;
    const char *GetDifficultyName() const;
    float GetDifficultyMultiplier() const;
    void AdvanceSmokeTest();

    static const int screenWidth = 1280;
    static const int screenHeight = 720;
    static const int boardX = 500;
    static const int boardY = 112;
    static const int cellSize = 28;

    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Character character;
    std::vector<Level> levels;
    SaveData saveData;
    GameScreen screen;
    GameScreen customizeReturnScreen;
    Difficulty difficulty;
    int selectedLevel;
    int score;
    int lines;
    int combo;
    float fallTimer;
    float screenTime;
    bool paused;
    bool won;
    bool newUnlock;
    bool characterDropAttempted;
    bool newCharacterUnlock;
    bool tryHardMode;
    bool smokeTest;
    int smokeFrame;
    RenderTexture2D renderTarget;
    std::array<Shader, 10> shaders;
};
