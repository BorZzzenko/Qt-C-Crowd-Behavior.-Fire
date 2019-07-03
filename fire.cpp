#include "fire.h"


Fire::Fire(int x, int y, RoomPlan *roomPlan) : ObjectOnRoomPlan (x, y, roomPlan)
{
    burningTime = 0;
    isSpread = false;
    healthPoint = 100;
}

void Fire::step()
{
    // Если время горения >= 20, то распространяется
    if(!isSpread){
        if(burningTime >= 20){
           spread();
        }

        burningTime++;
    }
}

void Fire::getDamage(int damage)
{
    healthPoint -= damage;

    // Если очки здоровья < 0, то огонь затухает
    if(healthPoint <= 0){
        roomPlan->deleteObject(this);
        roomPlan->addSmoke(x, y);
    }
}

 // Распространение огня влево, вверх, вправо, вниз,
 // если там нет стен

void Fire::spread()
{
    isSpread = true;

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCol[4] = {0, 0, 1 , -1};

    int** walls = roomPlan->getRoomMatrix();
    int cell = roomPlan->getCellWidth();

    int r, c;
    for (int i = 0; i < 4; i++) {
        r = x / cell + directionRow[i];
        c = y / cell + directionCol[i];

        if (walls[r][c] == 0) {
            if(roomPlan->isSmokeCoordinate(r * cell, c * cell)){
                roomPlan->deleteSmoke(r * cell, c * cell);
            }
            roomPlan->addFire(r * cell, c * cell);
        }
    }

    for (int i = 0; i < roomPlan->getWidth(); i++){
            delete [] walls[i];
    }
}
