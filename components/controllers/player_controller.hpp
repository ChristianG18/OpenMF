#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include <controllers/character_entity_controller.hpp>
#include <renderer/base_renderer.hpp>
#include <input/base_input_manager.hpp>

namespace MFGame
{

class PlayerController: public CharacterEntityController
{
public:
    PlayerController(MFGame::SpatialEntity *playerEntity, MFRender::Renderer *renderer, MFInput::InputManager *inputManager);
    virtual ~PlayerController() {};

    void update();

protected:
    MFRender::Renderer *mRenderer;
    MFInput::InputManager *mInputManager;
};

}

#endif
