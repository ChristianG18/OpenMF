#ifndef RENDERER_BASE_H
#define RENDERER_BASE_H

#include <fstream>
#include <math.hpp>

namespace MFRender
{

class Renderer
{
public:
    Renderer();
    virtual bool loadMission(std::string mission, bool load4ds=true, bool loadScene2Bin=true, bool loadCacheBin=true)=0;
    virtual bool loadSingleModel(std::string model)=0;
    virtual void frame(double dt)=0;
    virtual void setCameraParameters(bool perspective, float fov, float orthoSize, float nearDist, float farDist)=0;
    virtual void getCameraPositionRotation(MFMath::Vec3 &position, MFMath::Vec3 &rotYawPitchRoll)=0;
    virtual void setCameraPositionRotation(MFMath::Vec3 position, MFMath::Vec3 rotYawPitchRoll)=0;
    virtual void setFreeCameraSpeed(double newSpeed)=0;
    bool done() { return mDone; };
    virtual bool exportScene(std::string fileName)=0;
    virtual int getSelectedEntityId()=0;

    MFMath::Vec3 getCameraPosition() { MFMath::Vec3 p,r; getCameraPositionRotation(p,r); return p; };

protected:
    bool mDone;
};

Renderer::Renderer():
    mDone(false)
{
}

}

#endif
