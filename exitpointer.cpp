#include "exitpointer.h"

ExitPointer::ExitPointer(int x, int y, RoomPlan* roomPlan) : ObjectOnRoomPlan (x, y, roomPlan)
{

}

void ExitPointer::getExitCoordinate(int *x, int *y)
{
    *x = roomPlan->getExitX();
    *y = roomPlan->getExitY();
}
