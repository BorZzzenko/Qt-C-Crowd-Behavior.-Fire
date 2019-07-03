#include "fireextinguisher.h"
#include "human.h"

FireExtinguisher::FireExtinguisher(int x, int y, RoomPlan* roomPlan) : ObjectOnRoomPlan (x, y, roomPlan)
{
    isOwned = false;
    owner = nullptr;
}

void FireExtinguisher::step()
{
    // Если есть владелец, берем его координаты
    // и ставим огнетушитель рядом в свободную клетку

    if(isOwned){
        int directionX[4] = {-10, 10, 0, 0};
        int directionY[4] = {0, 0, 10, -10};

        int x = owner->getX();
        int y = owner->getY();

        for (int i = 0; i < 4; i++) {
            if(roomPlan->isFreeCoordinateForStep(x + directionX[i], y + directionY[i])){
                this->x = x + directionX[i];
                this->y = y + directionY[i];
                break;
            }
        }
    }
}

bool FireExtinguisher::isHaveOwner()
{
    return isOwned;
}

void FireExtinguisher::setOwner(Human* man)
{
    isOwned = true;
    owner = man;
}

void FireExtinguisher::resetOwner()
{
    isOwned = false;
    owner = nullptr;
}

void FireExtinguisher::extinguish(Fire *fire)
{
    fire->getDamage(POWER);
}
