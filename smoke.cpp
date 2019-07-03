#include "smoke.h"

Smoke::Smoke(int x, int y, RoomPlan *roomPlan): ObjectOnRoomPlan (x, y, roomPlan)
{
    isSpread = false;
    smokeTime = 0;
}

void Smoke::step()
{
    if(!isSpread){
        if(smokeTime > 5){
          spread();
        }
        smokeTime++;
    }
}

void Smoke::spread()
{
    isSpread = true;

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCol[4] = {0, 0, 1 , -1};

    int** walls = roomPlan->getRoomMatrix();

    int r, c;
    for (int i = 0; i < 4; i++) {
        r = x / roomPlan->getCellWidth() + directionRow[i];
        c = y / roomPlan->getCellWidth() + directionCol[i];

        if (walls[r][c] == 0) {
            if(!roomPlan->isSmokeCoordinate(r * roomPlan->getCellWidth(), c * roomPlan->getCellWidth())){
                roomPlan->addSmoke(r * roomPlan->getCellWidth(), c * roomPlan->getCellWidth());
            }
        }
    }

    for (int i = 0; i < roomPlan->getWidth(); i++){
            delete [] walls[i];
    }
}
