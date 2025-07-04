#ifndef MENUCOMPONENT_H
#define MENUCOMPONENT_H

#include <functional>
#include <vector>
#include <string>
#include <SDL.h>
#include "ecs/ecs.h"
#include "ecs/base/transformcomponent.h"

struct MenuEntry {
    std::string text;
    std::function<void()> callback;
};

class MenuComponent : public Component
{
public:
    MenuComponent(int width, int height);
    
    void init() override;
    void update() override;
    void draw() override;
    
    void addEntry(const std::string& text, std::function<void()> callback);
    void selectPrevious();
    void selectNext();
    void executeSelected();
    
    void setTextColor(SDL_Color color) { textColor = color; }
    void setSelectedColor(SDL_Color color) { selectedColor = color; }
    void setCursorChar(const std::string& cursor) { cursorChar = cursor; }
    
private:
    int menuWidth;
    int menuHeight;
    int currentSelection;
    std::vector<MenuEntry> entries;
    std::vector<Entity*> textEntities;
    Entity* cursorEntity;
    Entity* backgroundEntity;
    
    SDL_Color textColor;
    SDL_Color selectedColor;
    std::string cursorChar;
    
    TransformComponent* transform;
    
    void updateDisplay();
    void handleInput();
    void createBackground();
};

#endif // MENUCOMPONENT_H