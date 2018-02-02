#ifndef CHARACTER_ENTITY_CONTROLLER_H
#define CHARACTER_ENTITY_CONTROLLER_H

#include <controllers/entity_controller.hpp>
#include <input/base_input_manager.hpp>

namespace MFGame
{

class CharacterEntityController: public EntityController
{
public:
    typedef enum
    {
        WALK = 0,
        RUN,
        CROUCH,
    } MovementState;

    CharacterEntityController(SpatialEntity *entity);

    void setSpeeds(float walk, float run, float crouch)
    {
        mSpeeds[WALK] = walk;
        mSpeeds[RUN] = run;
        mSpeeds[CROUCH] = crouch;
    }

    void setMovementState(CharacterEntityController::MovementState newState)
    {
        mMovementState = newState;
    }

    void jump();
    bool isOnGround();
    virtual void moveLeft(bool start=true);
    virtual void moveRight(bool start=true);
    virtual void moveForward(bool start=true);
    virtual void moveBackward(bool start=true);

protected:
    void applyCurrentMotion();

    MovementState mMovementState;
    MFMath::Vec3 mMovementVector;
    float mSpeeds[3];
};

}

#endif // CHARACTER_ENTITY_CONTROLLER_H
