#ifndef SMOKE_H
#define SMOKE_H
#include "objectonroomplan.h"

// Дым
class Smoke : public ObjectOnRoomPlan
{
private:
    // Распространился ли огонь
    bool isSpread;
    // Время существования
    int smokeTime;

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
   explicit Smoke(int x, int y, RoomPlan* roomPlan);

    // Шаг
    void step();

private:
    // Распространение влево, вправо, вниз, вверх
    void spread();
};

#endif // SMOKE_H
