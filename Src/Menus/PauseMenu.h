#ifndef PAUSEMENU_H_INCLUDED
#define PAUSEMENU_H_INCLUDED

#include "../World/World.h"
#include "GUI/GUIButton.h"
#include "GUI/GUIFrame.h"

class PauseMenu {
private:
    // Whether the pause menu is being shown on startup as oppose to in the middle of a game.
    bool isMainMenu;

    enum class SubState {
        Main,
        NewGame,
        LoadGame,
        Options,
        Quitting
    };

    enum class OptionsTab {
        Graphics,
        Audio,
        Controls,
        Gameplay
    };

    SubState currState;
    OptionsTab currOptionsTab;

    void setState(SubState state);
    void setOptionsTab(OptionsTab tab);

    // --Buttons.

    // Main state.
    GUIButton newgame;
    GUIButton loadgame;
    GUIButton options;
    GUIButton quit;

    // Quitting state.
    GUIFrame quitPrompt;
    GUIButton quitYes;
    GUIButton quitNo;

public:
    PauseMenu(const UIAssets* assets);

    void update(World* world, PGE::Vector2f mousePosition);
};

#endif // PAUSEMENU_H_INCLUDED
