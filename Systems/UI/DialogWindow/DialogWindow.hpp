#pragma once
#include <Entities/utils/dialogComponent.hpp>
#include <Entities/utils/component.hpp>
#include <Entities/utils/entitiesManager.hpp>
#include <Infrastructure/entityManager.h>
#include <Map/Creating/Common/dist.hpp>
#include <Entities/components/map.hpp>
#include <Common/buildPath.hpp>
#include <Common/wrapText.hpp>

class DialogWindow
{
private:
    bool succesfullyInit = true;

    bool isActivate = false;
    bool isDialogerNear = false;
    int interlocutorID = 0;
    std::shared_ptr<DialogTreeComponent> currDialog;

    static constexpr float distForDialog = TILE_SIZE * 3;

//===========================STORE-FOR-SELECT===========================
    bool needWrapTextOnNextPage = false;
    bool remains_init = false;
    std::string remains = "";
    int selectedAnswerID = 0;
    bool canSlideAnswerID = true;
    bool canEnterAnswer = true;
    
    sf::Clock clockForSlide;
    sf::Clock clockForEnter;
    float treshholdTime = 0.3f;

//===========================STORE-FOR-REDRIG===========================
    sf::Clock clockForRedrig;
    bool useRedrigLeft = false;
    float strongOfRedrigLeft = 0.0f;
    bool useRedrigRight = false;
    float strongOfRedrigRight = 0.0f;
    static constexpr float max_duration_redrig = 1.f;

//==================================UI==================================
    // Button
    Vect2D BUTTON_E_SIZE = {64, 64};
    Vect2D BUTTON_E_POS_CENTER = {350, 300};

    // DialogMenu
    sf::Font NameOfPersonFont;
    sf::Font DialogFont;
    sf::Color DialogTextColor = sf::Color(220, 191, 144);
    sf::Color AnswerTextColor = DialogTextColor;
    sf::Color SelectedAnswerTextColor = sf::Color(253, 245, 150);
    static constexpr int fontSizeName = 24;
    static constexpr int fontSizeDialog = 12;
    static constexpr int fontSizeAnswer = 12;

    Vect2D DIALOG_MENU_SIZE = {460, 152};
    Vect2D DIALOG_MENU_POS_CENTER = {350, 100};
    Vect2D DIALOG_MENU_TEXTBOX = {396.175908222, 90};

    Vect2D DIALOG_ANSWER_POS_CENTER = {350, 250};
    Vect2D DIALOG_ANSWER_SIZE = {342.2265625, 100};
    Vect2D DIALOG_ANSWER_TEXTBOX_CENTER = {350, 250};
    Vect2D DIALOG_ANSWER_TEXTBOX_SIZE = {282.2265625, 85};
    static constexpr float marginBetweenAnswers = 10;

    Vect2D ANSWER_CURSOR_SHIFT = {16, -3};
    Vect2D ANSWER_CURSOR_SIZE = {12, 12};

    Vect2D DIALOG_MENU_NAMEBOX_POS = {175.780752071, 30};
    Vect2D DIALOG_MENU_NAMEBOX = {124.920331421, 14.0625};

    Vect2D DIALOG_MENU_AVATAR_L_CENTER = {82.486979167, 100.703125};
    Vect2D DIALOG_MENU_AVATAR_L_SIZE = {104.817708333, 104.817708333};
    Vect2D DIALOG_MENU_AVATAR_R_CENTER = {621.158854167, 100.703125};
    Vect2D DIALOG_MENU_AVATAR_R_SIZE = {104.817708333, 104.817708333};

//================================STORE================================
    std::string oponentName = "";

    std::string pathAvatarLeft = "";
    bool useGridLeft = false;
    Vect2D tileSizeLeft;
    Vect2D tileIDLeft;
    bool useShiftLeft = false;
    Vect2D shiftLeft;

    std::string pathAvatarRight = "";
    bool useGridRight = false; 
    Vect2D tileSizeRight;
    Vect2D tileIDRight;
    bool useShiftRight = false;
    Vect2D shiftRight;
public:
    DialogWindow() {
        if (!NameOfPersonFont.loadFromFile(buildFullPath("Store/view/Fonts/dialog/name/alagard-12px-unicode.ttf", 1))) {
            std::cerr <<  "Шрифт для имен персонажей не загружен. Ошибка.";
            succesfullyInit = false;
        }

        if (!DialogFont.loadFromFile(buildFullPath("Store/view/Fonts/dialog/text/Mogul Cotlin Bold.ttf", 1))) {
            std::cerr <<  "Шрифт для диалогов не загружен. Ошибка.";
            succesfullyInit = false;
        }
    }

    void Controller(EntitiesManager& manager) {
        if(!isActivate && isDialogerNear && sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            isActivate = true;
            initProgDone = false;
            currDialog = manager.getComponent<DialogTreeComponent>(interlocutorID);
            currDialog->current_node = currDialog->nodes["root"];
        }

        if(isDialogerNear && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            isActivate = false;
        }

        if(isActivate && canSlideAnswerID && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            selectedAnswerID--;
            canSlideAnswerID = false;
            clockForSlide.restart();
            if(selectedAnswerID < 0) 
                selectedAnswerID = currDialog->current_node->answers.size() - 1;
        }

        if(isActivate && canSlideAnswerID && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            selectedAnswerID++;
            canSlideAnswerID = false;
            clockForSlide.restart();
            if(selectedAnswerID >= currDialog->current_node->answers.size())
                selectedAnswerID = 0;
        }
        if(isActivate && canEnterAnswer && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            auto nextNode = currDialog->current_node->answers[selectedAnswerID].target_node_id;
            currDialog->current_node = currDialog->nodes[nextNode];
            selectedAnswerID = 0;
            initProgDone = false;
            clockForEnter.restart();
            canEnterAnswer = false;
        }

        if(!canSlideAnswerID && clockForSlide.getElapsedTime().asSeconds() > treshholdTime) {
            canSlideAnswerID = true;
        }

        if(!canEnterAnswer && clockForEnter.getElapsedTime().asSeconds() > treshholdTime) {
            canEnterAnswer = true;
        }
    }

    void UI(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        std::vector<int> entityIDs = manager.with<PositionOnMapComponent>().with<DialogTreeComponent>().without<DialogStartOnOver>().with<SizeComponent>().get();
        std::vector<int> dialogPointIDs = manager.with<PositionOnMapComponent>().with<DialogStartOnOver>().with<DialogTreeComponent>().without<DialogDone>().get();
        int playerID = manager.withClassName("*player*")[0];

        checkDialogPoint(manager, dialogPointIDs, playerID);
        runInitProg();
        drawDialogMenu(window, manager, textureLoader);
        drawDialogText(window, manager, textureLoader);
        drawDialogName(window, manager, textureLoader);
        drawAvatars(window, manager, textureLoader);
        drawAnswerMenu(window, manager, textureLoader);
        drawButtonEForDialog(window, manager, textureLoader, entityIDs);
    };

    bool canPlay() {
        return !isActivate;
    }

//================================COMAND================================
private:
    bool initProgDone = false;
    void exit() {
        isActivate = false;
    }

    void redrig(var_env_type var_env) {
        auto sideIt = var_env.find("side");
        auto strongIt = var_env.find("strong");
        if(sideIt == var_env.end()) return;
        if(strongIt == var_env.end()) return;

        auto sideVoidPtr = sideIt->second;
        auto sidePtr = std::static_pointer_cast<std::string>(sideVoidPtr);
        auto side = *sidePtr;

        auto strongVoidPtr = strongIt->second;
        auto strongPtr = std::static_pointer_cast<int>(strongVoidPtr);
        auto strong = *strongPtr;

        
        if(side == "left") {
            useRedrigLeft = true;
            strongOfRedrigLeft = strong;
        }
        else if(side == "right") {
            useRedrigRight = true;
            strongOfRedrigRight = strong;
        }
        
        clockForRedrig.restart();
    }
    
    void setAvatar(var_env_type var_env) {
        auto sideIt = var_env.find("side");
        auto pathIt = var_env.find("path");
        auto tileSizeIt = var_env.find("tileSize");
        auto tileIDIt = var_env.find("tileID");
        auto shiftIt = var_env.find("shift");

        bool useGrid = true;
        bool useShift = true;

        if(sideIt == var_env.end()) return;
        if(pathIt == var_env.end()) return;

        if(tileSizeIt == var_env.end()) useGrid = false;
        if(tileIDIt == var_env.end()) useGrid = false;
        if(shiftIt == var_env.end()) useShift = false;

        if(useGrid && tileSizeIt == var_env.end()) return;
        if(useGrid && tileIDIt == var_env.end()) return;
        if(!useGrid && shiftIt != var_env.end())  return;

        auto sideVoidPtr = sideIt->second;
        auto sidePtr = std::static_pointer_cast<std::string>(sideVoidPtr);
        auto side = *sidePtr.get();

        auto pathVoidPtr = pathIt->second;
        auto pathPtr = std::static_pointer_cast<std::string>(pathVoidPtr);
        auto path = *pathPtr.get();

        if(side == "left") {
            pathAvatarLeft = path;
            useGridLeft = useGrid;
            useShiftLeft = useShift;
            if(useGrid) {
                auto tileSizeVoidPtr = tileSizeIt->second;
                auto tileSizePtr = std::static_pointer_cast<Vect2D>(tileSizeVoidPtr);
                tileSizeLeft.x = tileSizePtr->x; tileSizeLeft.y = tileSizePtr->y;

                auto tileIDVoidPtr = tileIDIt->second;
                auto tileIDPtr = std::static_pointer_cast<Vect2D>(tileIDVoidPtr);
                tileIDLeft.x = tileIDPtr->x; tileIDLeft.y = tileIDPtr->y;

                if(useShift) {
                    auto shiftVoidPtr = shiftIt->second;
                    auto shiftPtr = std::static_pointer_cast<Vect2D>(shiftVoidPtr);
                    shiftLeft.x = shiftPtr->x; shiftLeft.y = shiftPtr->y;
                }
            }
        }

        else if(side == "right") {
            pathAvatarRight = path;
            useGridRight = useGrid;
            useShiftRight = useShift;
            if(useGrid) {
                auto tileSizeVoidPtr = tileSizeIt->second;
                auto tileSizePtr = std::static_pointer_cast<Vect2D>(tileSizeVoidPtr);
                tileSizeRight.x = tileSizePtr->x; tileSizeRight.y = tileSizePtr->y;

                auto tileIDVoidPtr = tileIDIt->second;
                auto tileIDPtr = std::static_pointer_cast<Vect2D>(tileIDVoidPtr);
                tileIDRight.x = tileIDPtr->x; tileIDRight.y = tileIDPtr->y;

                if(useShift) {
                    auto shiftVoidPtr = shiftIt->second;
                    auto shiftPtr = std::static_pointer_cast<Vect2D>(shiftVoidPtr);
                    shiftRight.x = shiftPtr->x; shiftRight.y = shiftPtr->y;
                }
            }
        }
    }
    
    void setName(var_env_type var_env) {
        auto it = var_env.find("name");
        if(it == var_env.end()) return;
        auto nameVoidPtr = it->second;
        auto namePtr = std::static_pointer_cast<std::string>(nameVoidPtr);
        oponentName = *namePtr.get();
    }

    void nextLevel(EntitiesManager& manager) {
        auto gameState = manager.with<GameStateComponent>().getComponent<GameStateComponent>(); 
        gameState->level++;
    };

    void runInitProg() {
        if(initProgDone) return;
        if(!currDialog) return;
        auto prog = currDialog->getProgramm();

        for(auto comand : prog) {
            switch (comand.type)
            {
            case ComandType::exit:
                exit();
                break;
            case ComandType::redrig:
                redrig(comand.var_env);
                break;
            case ComandType::setAvatar:
                setAvatar(comand.var_env);
                break;
            case ComandType::setName:
                setName(comand.var_env);
                break;
            case ComandType::nextLevel:
                nextLevel();
                break;
            default:
                break;
            }
        }
        initProgDone = true;
    }

//
private:
    void checkDialogPoint(EntitiesManager& manager, std::vector<int> dialogPointIDs, int playerID) {
        if(isActivate) return;
        Vect2D playerPos = manager.getComponent<PositionOnMapComponent>(playerID)->point;
        for(int dialogPointID : dialogPointIDs) {
            Vect2D dialogPos = manager.getComponent<PositionOnMapComponent>(dialogPointID)->point;
            float len = dist(dialogPos, playerPos);
            if(len < distForDialog) {
                interlocutorID = dialogPointID;
                isActivate = true;
                currDialog = manager.getComponent<DialogTreeComponent>(dialogPointID);
                DialogDone flag; manager.addComponent<DialogDone>(flag, dialogPointID);
                return;
            }
        }
    }

//==================================UI==================================
private:
    void drawDialogText(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!isActivate) return;

        auto text = currDialog->current_node->text;
        auto [wrappedText, remain] = createWrappedText(DialogFont, text, fontSizeDialog, DIALOG_MENU_TEXTBOX.x, DIALOG_MENU_TEXTBOX.y);

        if(remain.size() != 0) {
            needWrapTextOnNextPage = true;
        }

        Vect2D LEFT_UP_POINT = DIALOG_MENU_POS_CENTER - DIALOG_MENU_TEXTBOX / 2;
        wrappedText.setPosition(LEFT_UP_POINT.x, LEFT_UP_POINT.y);
        wrappedText.setFillColor(DialogTextColor);
        wrappedText.setOutlineColor(sf::Color::Black);
        wrappedText.setOutlineThickness(1);

        window.draw(wrappedText);
    };

    void drawDialogName(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!isActivate) return;
        if(oponentName.size() == 0) return;
    
        sf::Text nameText(sf::String::fromUtf8(oponentName.begin(), oponentName.end()), NameOfPersonFont, fontSizeDialog);

        nameText.setPosition(DIALOG_MENU_NAMEBOX_POS.x, DIALOG_MENU_NAMEBOX_POS.y);
        nameText.setFillColor(DialogTextColor);
        nameText.setOutlineColor(sf::Color::Black);
        nameText.setOutlineThickness(1);

        window.draw(nameText);
    }

    void drawAvatars(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!isActivate) return;
        if(pathAvatarLeft.size() == 0 && pathAvatarRight.size() == 0) return;
        sf::Sprite sprite_left;
        sf::Sprite sprite_right;

        Vect2D LEFT_UP_POINT_L = DIALOG_MENU_AVATAR_L_CENTER - DIALOG_MENU_AVATAR_L_SIZE / 2;
        Vect2D LEFT_UP_POINT_R = DIALOG_MENU_AVATAR_R_CENTER - DIALOG_MENU_AVATAR_R_SIZE / 2;
        
        if(useShiftLeft) 
            LEFT_UP_POINT_L -= shiftLeft;

        if(useShiftRight)
            LEFT_UP_POINT_R -= shiftRight;

        if(useRedrigLeft) {
            Vect2D shiftLeft = Vect2D(1, 1) / Vect2D(1, 1).length() * strongOfRedrigLeft;
            LEFT_UP_POINT_L -= shiftLeft * sin(clockForRedrig.getElapsedTime().asSeconds() * 80);
            if(clockForRedrig.getElapsedTime().asSeconds() > max_duration_redrig) {
                useRedrigLeft = false; 
                useRedrigRight = false;
            }
        }

        if(useRedrigRight) {
            Vect2D shiftRigth = Vect2D(1, 1) / Vect2D(1, 1).length() * strongOfRedrigRight;
            LEFT_UP_POINT_R -= shiftRigth * sin(clockForRedrig.getElapsedTime().asSeconds() * 80);
            if(clockForRedrig.getElapsedTime().asSeconds() > max_duration_redrig) {
                useRedrigLeft = false; 
                useRedrigRight = false;
            }
        }

        if(pathAvatarLeft.size() != 0)
            if(!useGridLeft) {
                sprite_left = textureLoader.getSprite(buildFullPath(pathAvatarLeft, 1), LEFT_UP_POINT_L, DIALOG_MENU_AVATAR_L_SIZE);
            }
            else {
                sprite_left = textureLoader.getSprite(buildFullPath(pathAvatarLeft, 1), LEFT_UP_POINT_L, DIALOG_MENU_AVATAR_L_SIZE, tileSizeLeft, tileIDLeft, {1, 1});
            }

        if(pathAvatarRight.size() != 0)
            if(!useGridRight) {
                sprite_right = textureLoader.getSprite(buildFullPath(pathAvatarRight, 1), LEFT_UP_POINT_R, DIALOG_MENU_AVATAR_R_SIZE);
            }
            else {
                sprite_right = textureLoader.getSprite(buildFullPath(pathAvatarRight, 1), LEFT_UP_POINT_R, DIALOG_MENU_AVATAR_R_SIZE, tileSizeRight, tileIDRight,  {-1, 1});
            }

        window.draw(sprite_left);
        window.draw(sprite_right);
    }

    void drawAnswerMenu(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!isActivate) return;

        Vect2D LEFT_UP_POINT = DIALOG_ANSWER_POS_CENTER - DIALOG_ANSWER_SIZE / 2;
        auto DIALOG_ANSWER_Sprite = textureLoader.getSprite("Store/view/Interactive_icons/AnswerMenu.png", LEFT_UP_POINT, DIALOG_ANSWER_SIZE);
        window.draw(DIALOG_ANSWER_Sprite);

        auto lenOfAnswers = currDialog->current_node->answers.size();
        for(int id = 0; id < lenOfAnswers;  id++) {
            auto heightLeftUP = (fontSizeAnswer + marginBetweenAnswers)* id;
            Vect2D LEFT_UP_POINT_TEXTBOX = DIALOG_ANSWER_TEXTBOX_CENTER - DIALOG_ANSWER_TEXTBOX_SIZE/ 2;
            LEFT_UP_POINT_TEXTBOX.y += heightLeftUP;

            auto answer = currDialog->current_node->answers[id];
            auto [wrappedText, remain] = createWrappedText(DialogFont, answer.text, fontSizeAnswer, DIALOG_ANSWER_TEXTBOX_SIZE.x, fontSizeAnswer + 4);

            wrappedText.setPosition(LEFT_UP_POINT_TEXTBOX.x, LEFT_UP_POINT_TEXTBOX.y);
            if(selectedAnswerID == id) {
                wrappedText.setFillColor(SelectedAnswerTextColor);
                auto cursorSprite = textureLoader.getSprite("Store/view/Interactive_icons/AnswerCursor.png", LEFT_UP_POINT_TEXTBOX - ANSWER_CURSOR_SHIFT, ANSWER_CURSOR_SIZE);
                window.draw(cursorSprite);
            }
            else
                wrappedText.setFillColor(DialogTextColor);

            wrappedText.setOutlineColor(sf::Color::Black);
            wrappedText.setOutlineThickness(1);
            window.draw(wrappedText);
        }
    }

    void drawDialogMenu(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader) {
        if(!isActivate) return;

        Vect2D LEFT_UP_POINT = DIALOG_MENU_POS_CENTER - DIALOG_MENU_SIZE / 2;
        auto DIALOG_MENU_Sprite = textureLoader.getSprite("Store/view/Interactive_icons/DialogMenu.png", LEFT_UP_POINT, DIALOG_MENU_SIZE);

        window.draw(DIALOG_MENU_Sprite);
    };

    void drawButtonEForDialog(sf::RenderWindow &window, EntitiesManager& manager, TextureLoader& textureLoader, std::vector<int> entityIDs) {
        if(isActivate) return;
        if(!manager.withClassName("*player*").size()) return;

        int playerID = manager.withClassName("*player*")[0];
        auto playerPos = manager.getComponent<PositionOnMapComponent>(playerID)->point;
        bool needToDraw = false;
        isDialogerNear = false;

        for(int dialogerID : entityIDs) {
            auto dialogerPos = manager.getComponent<PositionOnMapComponent>(dialogerID)->point;
            float len = dist(dialogerPos, playerPos);
            if(len < distForDialog) {
                needToDraw = true;
                isDialogerNear = true;
                interlocutorID = dialogerID;
                break;
            }
        }

        if(!needToDraw) return;

        Vect2D LEFT_UP_POINT = BUTTON_E_POS_CENTER - BUTTON_E_SIZE / 2;
        auto Button_E_Sprite = textureLoader.getSprite("Store/view/Interactive_icons/Press_E_icon.png", LEFT_UP_POINT, BUTTON_E_SIZE);

        window.draw(Button_E_Sprite);
    };
};