#include "objectonroomplan.h"


ObjectOnRoomPlan::ObjectOnRoomPlan(int x, int y, RoomPlan* roomPlan)
{
    this->roomPlan = roomPlan;
    this->x = x;
    this->y = y;
}

int ObjectOnRoomPlan::getX()
{
    return x;
}

int ObjectOnRoomPlan::getY()
{
    return y;
}

ObjectOnRoomPlan::~ObjectOnRoomPlan()
{

}

