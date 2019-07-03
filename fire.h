#ifndef FIRE_H
#define FIRE_H

#include "objectonroomplan.h"

// Огонь
class Fire : public ObjectOnRoomPlan
{
private:
    // Время горения
    int burningTime;

    // "Очки здоровья", уменьшаются под воздействием огнетушителя
    int healthPoint;

    // Распространился ли огонь
    bool isSpread;

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    explicit Fire(int x, int y, RoomPlan* roomPlan);

    // Шаг
    void step();

    // Получение урона от огнетушителя
    // damage - количество урона
    void getDamage(int damage);

private:
    // Распространение огня влево, вправо, вниз, вверх
    void spread();
};

#endif // FIRE_H
