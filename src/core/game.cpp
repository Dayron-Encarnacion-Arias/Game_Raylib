#include "game.h"
#include "colors.h"
#include <algorithm>
#include <cstdlib>

// Carga campaña, progreso, bloques y recursos que necesitan una ventana activa.
Game::Game(bool smokeTest)
{
    this->smokeTest = smokeTest;
    smokeFrame = 0;
    screen = GameScreen::Title;
    customizeReturnScreen = GameScreen::Title;
    difficulty = Difficulty::Normal;
    selectedLevel = 0;
    score = 0;
    lines = 0;
    combo = -1;
    fallTimer = 0.0f;
    screenTime = 0.0f;
    paused = false;
    won = false;
    newUnlock = false;
    characterDropAttempted = false;
    newCharacterUnlock = false;
    tryHardMode = false;
    levels = GetAllLevels();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    saveData.Load("saves/progress.dat");
    character.SetBodyColor(saveData.selectedColor);
    character.SetAccessory(saveData.selectedAccessory);
    character.SetType(saveData.selectedCharacter);
    for (int index = 0; index < 10; index++)
        shaders[index] = LoadShader(nullptr, TextFormat("shaders/level_%02i.fs", index + 1));
    renderTarget = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(renderTarget.texture, TEXTURE_FILTER_BILINEAR);
}

// Libera los shaders y la textura antes de que main cierre la ventana de Raylib.
Game::~Game()
{
    // TryHard es temporal: nunca copia su apariencia administrativa al guardado normal.
    if (!tryHardMode)
    {
        saveData.selectedColor = character.GetBodyColor();
        saveData.selectedAccessory = character.GetAccessory();
        saveData.selectedCharacter = character.GetType();
    }
    if (!smokeTest)
        saveData.Save("saves/progress.dat");
    for (Shader shader : shaders)
        UnloadShader(shader);
    UnloadRenderTexture(renderTarget);
}

// Actualiza tiempo y delega la entrada a la pantalla que se encuentra activa.
void Game::Update()
{
    const float deltaTime = std::min(GetFrameTime(), 0.05f);
    screenTime += deltaTime;
    if (screen == GameScreen::Title)
        UpdateTitle();
    else if (screen == GameScreen::LevelSelect)
        UpdateLevelSelect();
    else if (screen == GameScreen::Customize)
        UpdateCustomize();
    else if (screen == GameScreen::Playing)
        UpdatePlaying();
    else
        UpdateResult();
    if (smokeTest)
        AdvanceSmokeTest();
}

// Renderiza primero a una textura y despues aplica el shader del nivel elegido.
void Game::Draw()
{
    BeginTextureMode(renderTarget);
    DrawScene();
    EndTextureMode();

    const int shaderIndex = (screen == GameScreen::Title || screen == GameScreen::Customize)
                                ? 0
                                : selectedLevel;
    const int timeLocation = GetShaderLocation(shaders[shaderIndex], "time");
    if (timeLocation >= 0)
        SetShaderValue(shaders[shaderIndex], timeLocation, &screenTime, SHADER_UNIFORM_FLOAT);
    Vector2 resolution = {static_cast<float>(screenWidth), static_cast<float>(screenHeight)};
    const int resolutionLocation = GetShaderLocation(shaders[shaderIndex], "resolution");
    if (resolutionLocation >= 0)
        SetShaderValue(shaders[shaderIndex], resolutionLocation, &resolution, SHADER_UNIFORM_VEC2);

    BeginDrawing();
    ClearBackground(BLACK);
    BeginShaderMode(shaders[shaderIndex]);
    DrawTextureRec(renderTarget.texture,
                   {0, 0, static_cast<float>(renderTarget.texture.width),
                    -static_cast<float>(renderTarget.texture.height)},
                   {0, 0}, WHITE);
    EndShaderMode();
    EndDrawing();
    if (smokeTest && smokeFrame == 3)
        TakeScreenshot("assets/preview.png");
    if (smokeTest && smokeFrame == 11)
        TakeScreenshot("build/tryhard_preview.png");
    if (smokeTest && smokeFrame == 12)
        TakeScreenshot("build/exclusive_preview.png");
}

// Informa a main cuando la prueba automatica ya recorrio todas las pantallas.
bool Game::IsSmokeTestFinished() const
{
    return smokeTest && smokeFrame >= 14;
}

// Abre juego normal, personalizacion o el modo TryHard desde la portada.
void Game::UpdateTitle()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        screen = GameScreen::LevelSelect;
        screenTime = 0;
        return;
    }
    if (IsKeyPressed(KEY_T))
    {
        if (tryHardMode)
            ExitTryHard();
        else
            EnterTryHard();
        return;
    }
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    const Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, {550, 350, 250, 54}))
    {
        screen = GameScreen::LevelSelect;
    }
    else if (CheckCollisionPointRec(mouse, {550, 420, 250, 54}))
    {
        customizeReturnScreen = GameScreen::Title;
        screen = GameScreen::Customize;
    }
    else if (CheckCollisionPointRec(mouse, {550, 490, 250, 64}))
    {
        if (tryHardMode)
            ExitTryHard();
        else
            EnterTryHard();
        return;
    }
    else
        return;
    screenTime = 0;
}

// Navega las diez tarjetas, cambia dificultad e inicia un nivel desbloqueado.
void Game::UpdateLevelSelect()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        screen = GameScreen::Title;
        screenTime = 0;
        return;
    }
    if (IsKeyPressed(KEY_C))
    {
        customizeReturnScreen = GameScreen::LevelSelect;
        screen = GameScreen::Customize;
        screenTime = 0;
        return;
    }
    if (IsKeyPressed(KEY_RIGHT))
        selectedLevel = (selectedLevel + 1) % 10;
    if (IsKeyPressed(KEY_LEFT))
        selectedLevel = (selectedLevel + 9) % 10;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))
        selectedLevel = (selectedLevel + 5) % 10;
    if (IsKeyPressed(KEY_ONE))
        difficulty = Difficulty::Easy;
    if (IsKeyPressed(KEY_TWO))
        difficulty = Difficulty::Normal;
    if (IsKeyPressed(KEY_THREE))
        difficulty = Difficulty::Hard;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        const Vector2 mouse = GetMousePosition();
        for (int index = 0; index < 10; index++)
            if (CheckCollisionPointRec(mouse, {39.0f + (index % 5) * 240, 103.0f + (index / 5) * 109, 222, 94}))
                selectedLevel = index;
        for (int index = 0; index < 3; index++)
            if (CheckCollisionPointRec(mouse, {469.0f + index * 155, 408, 137, 52}))
                difficulty = static_cast<Difficulty>(index);
    }
    if (IsKeyPressed(KEY_ENTER) && IsLevelAvailable(selectedLevel))
        StartLevel();
}

// Cambia color o equipa solamente accesorios que el jugador ya desbloqueo.
void Game::UpdateCustomize()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (!tryHardMode)
        {
            saveData.selectedColor = character.GetBodyColor();
            saveData.selectedAccessory = character.GetAccessory();
            saveData.selectedCharacter = character.GetType();
            saveData.Save("saves/progress.dat");
        }
        screen = customizeReturnScreen;
        screenTime = 0;
        return;
    }
    if (character.GetType() == 0 && IsKeyPressed(KEY_LEFT))
        character.SetBodyColor((character.GetBodyColor() + 4) % 5);
    if (character.GetType() == 0 && IsKeyPressed(KEY_RIGHT))
        character.SetBodyColor((character.GetBodyColor() + 1) % 5);
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    const Vector2 mouse = GetMousePosition();
    for (int index = 0; index < 2; index++)
        if (CheckCollisionPointRec(mouse, {525.0f + index * 280, 151, 250, 44}) &&
            IsCharacterAvailable(index))
            character.SetType(index);
    for (int index = 0; index < 5; index++)
        if (character.GetType() == 0 && CheckCollisionPointCircle(mouse, {565.0f + index * 125, 258}, 34))
            character.SetBodyColor(index);
    for (int index = 0; index < 11; index++)
        if (character.GetType() == 0 &&
            CheckCollisionPointRec(mouse, {525.0f + (index % 3) * 225, 367.0f + (index / 3) * 64, 207, 48}) &&
            IsAccessoryAvailable(index))
            character.SetAccessory(index);
}

// Procesa pausa, entrada y descenso automatico segun la dificultad elegida.
void Game::UpdatePlaying()
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        paused = !paused;
        return;
    }
    if (paused)
    {
        if (IsKeyPressed(KEY_Q))
        {
            screen = GameScreen::LevelSelect;
            paused = false;
        }
        return;
    }
    HandlePlayingInput();
    fallTimer += GetFrameTime();
    const float interval = levels[selectedLevel].GetFallTime() * GetDifficultyMultiplier();
    if (fallTimer >= interval)
    {
        fallTimer = 0;
        MoveBlockDown();
    }
}

// Permite repetir, personalizar o volver al selector desde el resultado.
void Game::UpdateResult()
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
        screen = GameScreen::LevelSelect;
    if (IsKeyPressed(KEY_R))
        StartLevel();
    if (IsKeyPressed(KEY_C))
    {
        customizeReturnScreen = GameScreen::LevelSelect;
        screen = GameScreen::Customize;
    }
}

// Traduce las teclas de juego a operaciones privadas sobre el bloque actual.
void Game::HandlePlayingInput()
{
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        MoveBlockLeft();
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        MoveBlockRight();
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_X))
        RotateBlock();
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        MoveBlockDown();
        UpdateScore(0, 1);
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        int distance = 0;
        while (screen == GameScreen::Playing)
        {
            currentBlock.Move(1, 0);
            if (IsBlockOutside() || !BlockFits())
            {
                currentBlock.Move(-1, 0);
                break;
            }
            distance++;
        }
        UpdateScore(0, distance * 2);
        LockBlock();
    }
}

// Mueve a la izquierda y revierte el paso si produce una colision.
void Game::MoveBlockLeft()
{
    currentBlock.Move(0, -1);
    if (IsBlockOutside() || !BlockFits())
        currentBlock.Move(0, 1);
}

// Mueve a la derecha y revierte el paso si produce una colision.
void Game::MoveBlockRight()
{
    currentBlock.Move(0, 1);
    if (IsBlockOutside() || !BlockFits())
        currentBlock.Move(0, -1);
}

// Baja una fila o fija la pieza cuando ya no existe espacio debajo.
void Game::MoveBlockDown()
{
    currentBlock.Move(1, 0);
    if (IsBlockOutside() || !BlockFits())
    {
        currentBlock.Move(-1, 0);
        LockBlock();
    }
}

// Gira la pieza y deshace la rotacion si queda fuera o se superpone.
void Game::RotateBlock()
{
    currentBlock.Rotate();
    if (IsBlockOutside() || !BlockFits())
        currentBlock.UndoRotation();
}

// Comprueba limites horizontales e inferior permitiendo filas negativas al aparecer.
bool Game::IsBlockOutside() const
{
    for (const Position &tile : currentBlock.GetCellPositions())
        if (tile.column < 0 || tile.column >= 10 || tile.row >= 20)
            return true;
    return false;
}

// Comprueba que cada celda visible del bloque ocupe un espacio vacio.
bool Game::BlockFits() const
{
    for (const Position &tile : currentBlock.GetCellPositions())
        if (tile.row >= 0 && !grid.IsCellEmpty(tile.row, tile.column))
            return false;
    return true;
}

// Copia la pieza a Grid, limpia filas y prepara el siguiente bloque.
void Game::LockBlock()
{
    const std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (const Position &tile : tiles)
        if (tile.row < 0)
        {
            EndGame();
            return;
        }
    grid.LockCells(tiles, currentBlock.id);
    const int cleared = grid.ClearFullRows();
    UpdateScore(cleared, 0);
    lines += cleared;
    if (lines >= levels[selectedLevel].GetTargetLines())
    {
        CompleteLevel();
        return;
    }
    currentBlock = nextBlock;
    nextBlock = GetRandomBlock();
    if (!BlockFits())
        EndGame();
}

// Reinicia tablero y contadores usando la configuracion del nivel seleccionado.
void Game::StartLevel()
{
    ResetBoard();
    screen = GameScreen::Playing;
    paused = false;
    screenTime = 0;
}

// Entrega recompensa, abre la siguiente fase y muestra la pantalla de victoria.
void Game::CompleteLevel()
{
    won = true;
    newUnlock = false;
    characterDropAttempted = false;
    newCharacterUnlock = false;
    if (!tryHardMode)
    {
        newUnlock = !saveData.IsAccessoryUnlocked(selectedLevel + 1);
        saveData.UnlockReward(selectedLevel);
        if (selectedLevel == 6 && !saveData.IsCharacterUnlocked(1))
        {
            characterDropAttempted = true;
            newCharacterUnlock = GetRandomValue(1, 100) <= GetExclusiveDropChance();
            if (newCharacterUnlock)
                saveData.UnlockCharacter(1);
        }
        saveData.RegisterScore(selectedLevel, score);
        saveData.Save("saves/progress.dat");
    }
    screen = GameScreen::Result;
}

// Registra la puntuacion alcanzada y muestra el resultado de derrota.
void Game::EndGame()
{
    won = false;
    newUnlock = false;
    characterDropAttempted = false;
    newCharacterUnlock = false;
    if (!tryHardMode)
    {
        saveData.RegisterScore(selectedLevel, score);
        saveData.Save("saves/progress.dat");
    }
    screen = GameScreen::Result;
}

// Vacía Grid y obtiene una pareja nueva de bloques desde la bolsa.
void Game::ResetBoard()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
    lines = 0;
    combo = -1;
    fallTimer = 0;
    characterDropAttempted = false;
    newCharacterUnlock = false;
}

// Activa el interruptor administrativo sin abandonar la pantalla de inicio.
void Game::EnterTryHard()
{
    tryHardMode = true;
    screenTime = 0;
}

// Sale de TryHard y recupera la apariencia guardada de la partida normal.
void Game::ExitTryHard()
{
    character.SetType(0);
    character.SetBodyColor(saveData.selectedColor);
    character.SetAccessory(saveData.selectedAccessory);
    character.SetType(saveData.selectedCharacter);
    tryHardMode = false;
    screenTime = 0;
}

// Considera todos los niveles disponibles mientras TryHard esta activo.
bool Game::IsLevelAvailable(int level) const
{
    return tryHardMode || saveData.IsLevelUnlocked(level);
}

// Considera todos los accesorios disponibles mientras TryHard esta activo.
bool Game::IsAccessoryAvailable(int accessory) const
{
    return tryHardMode || saveData.IsAccessoryUnlocked(accessory);
}

// Considera disponibles los personajes exclusivos al activar TryHard.
bool Game::IsCharacterAvailable(int characterIndex) const
{
    return tryHardMode || saveData.IsCharacterUnlocked(characterIndex);
}

// Devuelve la probabilidad del personaje exclusivo segun la dificultad elegida.
int Game::GetExclusiveDropChance() const
{
    if (difficulty == Difficulty::Easy)
        return 15;
    if (difficulty == Difficulty::Hard)
        return 50;
    return 30;
}

// Suma puntos por filas y por descenso manual, incluyendo bonus de combo.
void Game::UpdateScore(int clearedRows, int dropPoints)
{
    static const int rowScores[5] = {0, 100, 300, 500, 800};
    score += dropPoints;
    if (clearedRows > 0)
    {
        combo++;
        score += rowScores[clearedRows] + std::max(0, combo) * 50;
    }
    else if (dropPoints == 0)
        combo = -1;
}

// Extrae una pieza aleatoria y repone la bolsa cuando queda vacia.
Block Game::GetRandomBlock()
{
    if (blocks.empty())
        blocks = GetAllBlocks();
    const int randomIndex = std::rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

// Crea una bolsa que contiene exactamente una instancia de cada tetromino.
std::vector<Block> Game::GetAllBlocks() const
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

// Selecciona el metodo de dibujo correspondiente al estado actual.
void Game::DrawScene() const
{
    if (screen == GameScreen::Title)
        DrawTitle();
    else if (screen == GameScreen::LevelSelect)
        DrawLevelSelect();
    else if (screen == GameScreen::Customize)
        DrawCustomize();
    else if (screen == GameScreen::Playing)
        DrawPlaying();
    else
        DrawResult();
}

// Dibuja la portada roja y negra con juego, personaje y acceso TryHard.
void Game::DrawTitle() const
{
    levels[9].DrawMap(screenTime, screenWidth, screenHeight);
    DrawCentered("RED SHIFT", screenWidth / 2, 113, 84, WHITE);
    DrawCentered("T E T R I S", screenWidth / 2, 205, 29, lightRed);
    DrawCentered("CONSTRUYE / SOBREVIVE / DESBLOQUEA", screenWidth / 2, 254, 17, mutedText);
    character.Draw({360, 430}, 2.15f, 1, screenTime);
    DrawPanel({520, 315, 310, 300}, mainRed);
    Rectangle play = {550, 350, 250, 54};
    Rectangle customize = {550, 420, 250, 54};
    Rectangle tryHard = {550, 490, 250, 64};
    DrawRectangleRounded(play, 0.18f, 8, CheckCollisionPointRec(GetMousePosition(), play) ? mainRed : Color{37, 25, 31, 255});
    DrawRectangleRounded(customize, 0.18f, 8, CheckCollisionPointRec(GetMousePosition(), customize) ? lightRed : Color{37, 25, 31, 255});
    DrawRectangleRounded(tryHard, 0.18f, 8, Color{37, 25, 31, 255});
    DrawRectangleRoundedLinesEx(tryHard, 0.18f, 8, 2, tryHardMode ? GOLD : mutedText);
    DrawCentered("JUGAR", 675, 367, 20, WHITE);
    DrawCentered("PERSONAJE", 675, 437, 20, WHITE);
    DrawText("TRYHARD", 568, 512, 18, tryHardMode ? GOLD : WHITE);
    Rectangle switchTrack = {716, 507, 62, 30};
    DrawRectangleRounded(switchTrack, 1.0f, 12, tryHardMode ? GOLD : Color{77, 67, 72, 255});
    DrawCircle(tryHardMode ? 762 : 732, 522, 11, tryHardMode ? BLACK : WHITE);
    DrawCentered(tryHardMode ? "ACTIVO: todo disponible" : "INACTIVO: progreso normal", 675, 572, 13,
                 tryHardMode ? GOLD : mutedText);
}

// Dibuja diez tarjetas con un escenario de pose para el personaje en cada una.
void Game::DrawLevelSelect() const
{
    levels[selectedLevel].DrawMap(screenTime, screenWidth, screenHeight);
    DrawHeader(tryHardMode ? "TRYHARD / NIVELES" : "SELECCION DE NIVEL",
               "ESC: inicio | Flechas: navegar | C: personaje | ENTER: jugar");
    for (int index = 0; index < 10; index++)
    {
        Rectangle card = {39.0f + (index % 5) * 240, 103.0f + (index / 5) * 109, 222, 94};
        bool unlocked = IsLevelAvailable(index);
        DrawPanel(card, levels[index].GetPrimaryColor(), selectedLevel == index);
        DrawText(levels[index].GetName().c_str(), card.x + 13, card.y + 13, 17, unlocked ? WHITE : mutedText);
        DrawText(unlocked ? levels[index].GetLocation().c_str() : "BLOQUEADO", card.x + 13, card.y + 40, 13,
                 unlocked ? levels[index].GetPrimaryColor() : mutedText);
        DrawText(TextFormat("MEJOR %06i", saveData.bestScores[index]), card.x + 13, card.y + 67, 11, mutedText);
        DrawRectangleRounded({card.x + 164, card.y + 9, 48, 76}, 0.25f, 5, {12, 10, 13, 210});
        character.Draw({card.x + 188, card.y + 47}, 0.64f, index, screenTime, !unlocked);
        if (!unlocked)
            DrawText("X", card.x + 184, card.y + 36, 20, mainRed);
    }
    const Level &level = levels[selectedLevel];
    DrawPanel({39, 340, 1162, 300}, level.GetPrimaryColor(), true);
    DrawText(level.GetName().c_str(), 73, 369, 31, WHITE);
    DrawText(level.GetLocation().c_str(), 73, 410, 18, mutedText);
    DrawText("RECOMPENSA", 73, 465, 14, level.GetPrimaryColor());
    DrawText(level.GetReward().c_str(), 73, 490, 23, WHITE);
    DrawText(TextFormat("META: %i LINEAS", level.GetTargetLines()), 73, 545, 17, mutedText);
    if (selectedLevel == 6 && !IsCharacterAvailable(1))
        DrawText(TextFormat("CADETE CARMESI: %i%%", GetExclusiveDropChance()), 73, 579, 14, GOLD);
    DrawText("DIFICULTAD DEL NIVEL", 469, 371, 16, mutedText);
    for (int index = 0; index < 3; index++)
    {
        Difficulty option = static_cast<Difficulty>(index);
        Rectangle button = {469.0f + index * 155, 408, 137, 52};
        DrawPanel(button, difficulty == option ? level.GetPrimaryColor() : mutedText, difficulty == option);
        const char *name = option == Difficulty::Easy ? "FACIL" : option == Difficulty::Hard ? "DIFICIL"
                                                                                             : "NORMAL";
        DrawCentered(name, button.x + button.width / 2, 424, 16, difficulty == option ? WHITE : mutedText);
    }
    character.Draw({1048, 468}, 1.62f, selectedLevel, screenTime, !IsLevelAvailable(selectedLevel));
    DrawCentered(IsLevelAvailable(selectedLevel) ? "ENTER PARA INICIAR" : "NIVEL BLOQUEADO", 670, 589, 18,
                 IsLevelAvailable(selectedLevel) ? level.GetPrimaryColor() : mutedText);
}

// Dibuja selector de personaje, colores y accesorios compatibles.
void Game::DrawCustomize() const
{
    levels[2].DrawMap(screenTime, screenWidth, screenHeight);
    DrawHeader(tryHardMode ? "TRYHARD / TALLER" : "TALLER DE PERSONAJE",
               tryHardMode ? "ESC: volver sin guardar" : "ESC: guardar y volver");
    DrawPanel({55, 110, 410, 535}, mainRed, true);
    DrawCentered("VISTA PREVIA", 260, 139, 18, mutedText);
    character.Draw({260, 340}, 3.9f, 1, screenTime);
    static const char *names[11] = {"Sin accesorio", "Visor rojo", "Gorra rebelde", "Audifonos", "Antena", "Corona", "Capa", "Llave mecanica", "Halo", "Cuernos", "Mascara legendaria"};
    DrawCentered(character.GetType() == 1 ? "CADETE CARMESI" : names[character.GetAccessory()], 260, 568, 24, WHITE);
    DrawPanel({495, 110, 730, 184}, lightRed);
    DrawText("PERSONAJE", 525, 124, 17, WHITE);
    static const char *characterNames[2] = {"Operador", "Cadete Carmesi"};
    for (int index = 0; index < 2; index++)
    {
        Rectangle option = {525.0f + index * 280, 151, 250, 44};
        const bool available = IsCharacterAvailable(index);
        const bool selected = character.GetType() == index;
        DrawPanel(option, selected ? GOLD : lightRed, selected);
        DrawCentered(available ? characterNames[index] : "??? / NIVEL 07", option.x + option.width / 2,
                     option.y + 13, 15, available ? (selected ? WHITE : mutedText) : Color{74, 65, 70, 255});
    }
    DrawText(character.GetType() == 0 ? "COLOR DEL CUERPO" : "COLOR FIJO DEL EXCLUSIVO", 525, 211, 16,
             character.GetType() == 0 ? WHITE : mutedText);
    const std::vector<Color> colors = GetCharacterColors();
    for (int index = 0; index < 5; index++)
    {
        Vector2 point = {565.0f + index * 125, 258};
        const bool editable = character.GetType() == 0;
        DrawCircleV(point, editable && character.GetBodyColor() == index ? 29 : 25,
                    editable && character.GetBodyColor() == index ? WHITE : Fade(colors[index], editable ? 1.0f : 0.25f));
        DrawCircleV(point, 21, Fade(colors[index], editable ? 1.0f : 0.25f));
    }
    DrawPanel({495, 312, 730, 333}, mainRed);
    DrawText(character.GetType() == 0 ? "ACCESORIOS" : "ACCESORIOS PERMANENTES / NO COMPATIBLE", 525, 331, 17,
             character.GetType() == 0 ? WHITE : GOLD);
    for (int index = 0; index < 11; index++)
    {
        Rectangle item = {525.0f + (index % 3) * 225, 367.0f + (index / 3) * 64, 207, 48};
        bool unlocked = IsAccessoryAvailable(index) && character.GetType() == 0;
        bool selected = character.GetType() == 0 && character.GetAccessory() == index;
        DrawPanel(item, selected ? lightRed : darkRed, selected);
        const char *label = character.GetType() == 1 ? names[index] : (unlocked ? names[index] : TextFormat("Nivel %02i", index));
        DrawText(label, item.x + 12, item.y + 15, 15,
                 unlocked ? (selected ? WHITE : mutedText) : Color{74, 65, 70, 255});
        if (!unlocked && character.GetType() == 0)
            DrawText("X", item.x + 178, item.y + 14, 17, mainRed);
    }
}

// Dibuja tablero, pieza actual, siguiente pieza, progreso y recompensa.
void Game::DrawPlaying() const
{
    const Level &level = levels[selectedLevel];
    level.DrawMap(screenTime, screenWidth, screenHeight);
    DrawHeader(level.GetName().c_str(),
               TextFormat("%s%s | ESC: pausa", tryHardMode ? "TRYHARD | " : "", GetDifficultyName()));
    DrawPanel({54, 112, 360, 560}, level.GetPrimaryColor());
    character.Draw({234, 250}, 2.1f, selectedLevel, screenTime);
    DrawCentered(character.GetType() == 1 ? "CADETE CARMESI" : "OPERADOR", 234, 335, 15, mutedText);
    DrawText("PUNTUACION", 86, 383, 14, mutedText);
    DrawText(TextFormat("%07i", score), 86, 408, 35, WHITE);
    DrawText("LINEAS", 86, 475, 14, mutedText);
    DrawText(TextFormat("%02i / %02i", lines, level.GetTargetLines()), 86, 500, 30, level.GetPrimaryColor());
    float progress = std::min(1.0f, lines / static_cast<float>(level.GetTargetLines()));
    DrawRectangleRounded({86, 544, 292, 15}, 0.5f, 6, {49, 39, 44, 255});
    DrawRectangleRounded({86, 544, 292 * progress, 15}, 0.5f, 6, level.GetPrimaryColor());
    DrawText("A/D mover | X girar", 86, 615, 13, mutedText);
    DrawText("ESPACIO bajar", 86, 640, 13, mutedText);
    DrawRectangleRounded({boardX - 12.0f, boardY - 12.0f, 304, 584}, 0.04f, 6, {7, 7, 9, 245});
    DrawRectangleRoundedLinesEx({boardX - 12.0f, boardY - 12.0f, 304, 584}, 0.04f, 6, 3, level.GetPrimaryColor());
    grid.Draw(boardX, boardY, cellSize);
    currentBlock.Draw(boardX, boardY, cellSize);
    DrawPanel({862, 112, 360, 560}, level.GetPrimaryColor());
    DrawText("SIGUIENTE", 899, 145, 15, mutedText);
    DrawNextBlock(nextBlock, 949, 181);
    DrawText("RECOMPENSA", 899, 477, 14, level.GetPrimaryColor());
    DrawText(level.GetReward().c_str(), 899, 505, 20, WHITE);
    DrawText("Completa la meta para desbloquearla", 899, 554, 14, mutedText);
    if (selectedLevel == 6 && !IsCharacterAvailable(1))
        DrawText(TextFormat("Exclusivo: %i%% en esta dificultad", GetExclusiveDropChance()), 899, 590, 14, GOLD);
    if (paused)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.78f));
        DrawPanel({420, 225, 440, 270}, level.GetPrimaryColor(), true);
        DrawCentered("PAUSA", screenWidth / 2, 263, 44, WHITE);
        DrawCentered("ESC para continuar", screenWidth / 2, 333, 18, mutedText);
        DrawCentered("Q para abandonar", screenWidth / 2, 376, 18, mutedText);
    }
}

// Dibuja el resumen y presenta visualmente el premio obtenido.
void Game::DrawResult() const
{
    const Level &level = levels[selectedLevel];
    level.DrawMap(screenTime, screenWidth, screenHeight);
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.42f));
    DrawPanel({290, 92, 700, 545}, level.GetPrimaryColor(), true);
    DrawCentered(won ? "NIVEL SUPERADO" : "SISTEMA CAIDO", screenWidth / 2, 133, 48, won ? WHITE : lightRed);
    DrawCentered(level.GetName().c_str(), screenWidth / 2, 194, 18, mutedText);
    Character rewardCharacter = character;
    if (won)
    {
        if (newCharacterUnlock)
            rewardCharacter.SetType(1);
        else
        {
            rewardCharacter.SetType(0);
            rewardCharacter.SetAccessory(selectedLevel + 1);
        }
    }
    rewardCharacter.Draw({460, 360}, 2.55f, won ? 1 : 2, screenTime);
    DrawText("PUNTUACION", 605, 279, 14, mutedText);
    DrawText(TextFormat("%07i", score), 605, 306, 36, WHITE);
    DrawText("LINEAS", 605, 372, 14, mutedText);
    DrawText(TextFormat("%i / %i", lines, level.GetTargetLines()), 605, 399, 28, level.GetPrimaryColor());
    if (won)
    {
        DrawText(tryHardMode ? "VISTA PREVIA TRYHARD" : (newUnlock ? "NUEVO ACCESORIO" : "ACCESORIO CONSEGUIDO"),
                 605, 465, 14, level.GetPrimaryColor());
        DrawText(level.GetReward().c_str(), 605, 491, 22, WHITE);
    }
    else
        DrawText("La pila llego al limite", 605, 478, 15, mutedText);
    if (characterDropAttempted)
    {
        DrawText(newCharacterUnlock ? "CADETE CARMESI DESBLOQUEADO" : "EL EXCLUSIVO NO APARECIO",
                 605, 526, 15, newCharacterUnlock ? GOLD : mutedText);
        if (!newCharacterUnlock)
            DrawText(TextFormat("Probabilidad %i%% / repite el nivel", GetExclusiveDropChance()), 605, 550, 13, mutedText);
    }
    DrawCentered("ENTER: niveles | R: repetir | C: personalizar", screenWidth / 2, 598, 16, mutedText);
}

// Dibuja un contenedor oscuro con borde de seleccion opcional.
void Game::DrawPanel(Rectangle rectangle, Color color, bool selected) const
{
    DrawRectangleRounded(rectangle, 0.12f, 8, selected ? Color{44, 23, 31, 250} : panelBlack);
    DrawRectangleRoundedLinesEx(rectangle, 0.12f, 8, selected ? 3 : 1, selected ? color : Fade(color, 0.45f));
}

// Centra un texto horizontalmente respecto a una coordenada.
void Game::DrawCentered(const char *text, int centerX, int y, int size, Color color) const
{
    DrawText(text, centerX - MeasureText(text, size) / 2, y, size, color);
}

// Dibuja el encabezado comun de todas las pantallas secundarias.
void Game::DrawHeader(const char *title, const char *hint) const
{
    DrawRectangle(0, 0, screenWidth, 76, {10, 8, 11, 245});
    DrawRectangle(0, 74, screenWidth, 2, mainRed);
    DrawText(title, 38, 20, 34, WHITE);
    DrawText(hint, screenWidth - MeasureText(hint, 16) - 34, 29, 16, mutedText);
}

// Ajusta la matriz local para mostrar la siguiente pieza dentro de su panel.
void Game::DrawNextBlock(const Block &block, int x, int y) const
{
    const std::vector<Color> colors = GetCellColors();
    for (const Position &tile : block.GetCellPositions())
        DrawRectangleRounded({x + (tile.column - 3) * 20.0f, y + (tile.row + 1) * 20.0f, 17, 17}, 0.18f, 4, colors[block.id - 1]);
}

// Convierte la dificultad actual en una etiqueta para la interfaz.
const char *Game::GetDifficultyName() const
{
    if (difficulty == Difficulty::Easy)
        return "FACIL";
    if (difficulty == Difficulty::Hard)
        return "DIFICIL";
    return "NORMAL";
}

// Devuelve el factor que modifica la velocidad base del nivel.
float Game::GetDifficultyMultiplier() const
{
    if (difficulty == Difficulty::Easy)
        return 1.32f;
    if (difficulty == Difficulty::Hard)
        return 0.72f;
    return 1.0f;
}

// Recorre las pantallas, incluido TryHard, para validar el render sin interaccion.
void Game::AdvanceSmokeTest()
{
    smokeFrame++;
    if (smokeFrame == 10)
        EnterTryHard();
    else if (smokeFrame == 11)
    {
        character.SetType(1);
        screen = GameScreen::Customize;
    }
    else if (smokeFrame == 12)
        StartLevel();
    else if (smokeFrame == 13)
    {
        screen = GameScreen::Result;
        won = true;
    }
}
