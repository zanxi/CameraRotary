#ifndef CAMERAACTION_H
#define CAMERAACTION_H

#include <memory>
#include "camera_driver.hpp"

class Camera_Action
{
    enum STATE{idle=0,active=1};
    bool state=false; // false - idle, true - active
    bool commandMove = false; // false - unmove true - move
    //enum COMMAND{move=0};

    unsigned curr_x;
    unsigned curr_y;
    unsigned xmax;
    unsigned ymax;

    AbstractCameraDriver *defcam;    

public:
    Camera_Action()
    {
        defcam = reinterpret_cast<AbstractCameraDriver*>(GetCameraDriverGlobalInstance());
        defcam->GetMaxPosition(xmax,ymax);
        defcam->GetCurrentPosition(curr_x,curr_y);
    };

    unsigned Get_Current_X()
    {
       defcam->GetCurrentPosition(curr_x,curr_y);
       return curr_x;
    }

    unsigned Get_Current_Y()
    {
        defcam->GetCurrentPosition(curr_x,curr_y);
       return curr_y;
    }


    unsigned Get_XMAX()
    {
       return xmax;
    }

    unsigned Get_YMAX()
    {
       return ymax;
    }

    bool GetState()
    {
        return state;
    }
    void SetState(bool _state)
    {
       state = _state;
    }

    bool GetMove()
    {
        return commandMove;
    }
    void SetMove(bool _commandMove)
    {
       commandMove = _commandMove;
    }

    void StepLeft()
    {
        defcam->StepLeft();

    }

    void StepRight()
    {
        defcam->StepRight();
    }

    void StepUp()
    {
        defcam->StepUp();
    }

    void StepDown()
    {
        defcam->StepDown();
    }


    void Action(STATE state)
    {
        if(state==idle) return;
        else {
           AbstractCameraDriver *defcam = reinterpret_cast<AbstractCameraDriver*>(GetCameraDriverGlobalInstance());
           unsigned curr_x;
           unsigned curr_y;
           defcam->GetCurrentPosition(curr_x,curr_y);
           defcam->StepLeft();
        }

        //AbstractCameraDriver defcam = GetCameraDriverGlobalInstance();
        //defcam.StepLeft();

    }
};



#endif // CAMERAACTION_H
