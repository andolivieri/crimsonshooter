#include "menucomponent.h"
#include "textcomponent.h"
#include "ecs/base/spritecomponent.h"
#include "engine/game.h"
#include "engine/texturemanager.h"
#include "helpers/utils.h"
#include <iostream>

MenuComponent::MenuComponent(int width, int height)
    : menuWidth(width), menuHeight(height), currentSelection(0),
      textColor({255, 255, 255}), selectedColor({255, 0, 0}),
      cursorChar(">"), cursorEntity(nullptr), backgroundEntity(nullptr)
{
}

void MenuComponent::init()
{
    transform = &entity->emplaceComponent<TransformComponent>();
    createBackground();
}

void MenuComponent::createBackground()
{
    auto& bg = entity->m_manager.addEntity("menu_background");
    
    int padding = 20;
    int actualMenuHeight = static_cast<int>(entries.size() * 30 + padding * 2);
    
    bg.addComponent<TransformComponent>(
        static_cast<int>(transform->pos.x - padding),
        static_cast<int>(transform->pos.y - padding),
        menuWidth + padding * 2,
        actualMenuHeight
    );
    
    bg.addComponent<DumbComponent>().onDraw([&](){
        auto& bgTransform = bg.getComponent<TransformComponent>();
        
        SDL_SetRenderDrawColor(TextureManager::renderer, 0, 0, 0, 200);
        
        SDL_Rect bgRect = {
            static_cast<int>(bgTransform.pos.x),
            static_cast<int>(bgTransform.pos.y),
            bgTransform.width,
            bgTransform.height
        };
        
        SDL_RenderFillRect(TextureManager::renderer, &bgRect);
    });
    backgroundEntity = &bg;
    bg.addGroup(groupPlayers);
}

void MenuComponent::update()
{
    handleInput();
}

void MenuComponent::draw()
{
    
}

void MenuComponent::addEntry(const std::string& text, std::function<void()> callback)
{
    entries.push_back({text, callback});
    updateDisplay();
}

void MenuComponent::selectPrevious()
{
    if (entries.empty()) return;
    
    currentSelection = (currentSelection - 1 + entries.size()) % entries.size();
    updateDisplay();
}

void MenuComponent::selectNext()
{
    if (entries.empty()) return;
    
    currentSelection = (currentSelection + 1) % entries.size();
    updateDisplay();
}

void MenuComponent::executeSelected()
{
    if (currentSelection >= 0 && currentSelection < entries.size()) {
        if (entries[currentSelection].callback) {
            entries[currentSelection].callback();
        }
    }
}

void MenuComponent::updateDisplay()
{
    
    for (auto* textEntity : textEntities) {
        textEntity->destroy();
    }
    textEntities.clear();
    
    if (cursorEntity) {
        cursorEntity->destroy();
        cursorEntity = nullptr;
    }
    
    if (backgroundEntity) {
        backgroundEntity->destroy();
        backgroundEntity = nullptr;
    }
    

    createBackground();
    
    for (size_t i = 0; i < entries.size(); ++i) {
        auto& textEntity =  entity->m_manager.addEntity("menu_text_" + std::to_string(i));
        
        int yOffset = static_cast<int>(i * 30); // 30 pixels between entries
        textEntity.addComponent<TransformComponent>(
            static_cast<int>(transform->pos.x), 
            static_cast<int>(transform->pos.y + yOffset),
            200, 20
        );
        
        std::string paddedText = strRightPad(entries[i].text, 12);
        auto& textComp = textEntity.addComponent<TextComponent>(paddedText);
        textComp.setAbsolute(true);
        
        if (i == currentSelection) {
            textComp.setColor(selectedColor);
        } else {
            textComp.setColor(textColor);
        }
        
        textEntities.push_back(&textEntity);
        
        textEntity.addGroup(groupPlayers);
    }
    
    if (!entries.empty()) {
        auto& cursor =   entity->m_manager.addEntity("menu_cursor");
        int cursorY = static_cast<int>(transform->pos.y + currentSelection * 30);
        cursor.addComponent<TransformComponent>(
            static_cast<int>(transform->pos.x - 40), // cursor to the left
            cursorY,
            20, 20
        );
        
        auto& cursorText = cursor.addComponent<TextComponent>(cursorChar);
        cursorText.setAbsolute(true);
        
        cursorEntity = &cursor;
        cursor.addGroup(groupPlayers);
    }
}

void MenuComponent::handleInput()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;
    
    if (currentKeyStates[SDL_SCANCODE_UP] && !upPressed) {
        selectPrevious();
        upPressed = true;
    } else if (!currentKeyStates[SDL_SCANCODE_UP]) {
        upPressed = false;
    }
    
    if (currentKeyStates[SDL_SCANCODE_DOWN] && !downPressed) {
        selectNext();
        downPressed = true;
    } else if (!currentKeyStates[SDL_SCANCODE_DOWN]) {
        downPressed = false;
    }
    
    if (currentKeyStates[SDL_SCANCODE_RETURN] && !enterPressed) {
        executeSelected();
        enterPressed = true;
    } else if (!currentKeyStates[SDL_SCANCODE_RETURN]) {
        enterPressed = false;
    }
}