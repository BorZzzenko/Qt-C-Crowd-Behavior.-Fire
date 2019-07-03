#include "evacuationbutton.h"

EvacuationButton::EvacuationButton(int x, int y, RoomPlan *roomPlan) : ObjectOnRoomPlan (x, y, roomPlan)
{

}

void EvacuationButton::push()
{
    roomPlan->setEvacuation(true);
}
