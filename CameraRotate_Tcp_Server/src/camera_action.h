#ifndef CAMERAACTION_H
#define CAMERAACTION_H

// класс для работы с драйверами камеры из бибилотеки camera_drive.hpp и camera_drive.cpp

#include "camera_driver.hpp"

class Camera_Action
{
    enum STATE{idle=0,active=1};
    bool state=false; // false - idle, true - active - переменная  статуса работы камеры - не активна или в движении
    bool commandMove = false; // false - unmove true - move
    //enum COMMAND{move=0};

    unsigned curr_x; // текущая позиция X
    unsigned curr_y; // текущая позиция Y
    unsigned xmax; // max X
    unsigned ymax; // max X

    AbstractCameraDriver *defcam;  // указатель на драйвер объект камеру

public:
    Camera_Action()
    {
        defcam = reinterpret_cast<AbstractCameraDriver*>(GetCameraDriverGlobalInstance());
        defcam->GetMaxPosition(xmax,ymax);
        defcam->GetCurrentPosition(curr_x,curr_y);
    };

   // получить текущая позиция X
    unsigned Get_Current_X()
    {
       defcam->GetCurrentPosition(curr_x,curr_y);
       return curr_x;
    }
    
    // получить текущая позиция Y

    unsigned Get_Current_Y()
    {
        defcam->GetCurrentPosition(curr_x,curr_y);
       return curr_y;
    }

    
    // получить max X

    unsigned Get_XMAX()
    {
       return xmax;
    }

    // получить max Y

    unsigned Get_YMAX()
    {
       return ymax;
    }

    // получить статус работы камеры

    bool GetState()
    {
        return state;
    }

    // установить статус  работы камеры

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

    // движение камеры влево на единицу

    void StepLeft()
    {
        defcam->StepLeft();

    }

    // движение камеры вправо на единицу

    void StepRight()
    {
        defcam->StepRight();
    }

    // движение камеры вверх на единицу

    void StepUp()
    {
        defcam->StepUp();
    }

    // движение камеры вниз на единицу

    void StepDown()
    {
        defcam->StepDown();
    }


   // лишняя функция
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
