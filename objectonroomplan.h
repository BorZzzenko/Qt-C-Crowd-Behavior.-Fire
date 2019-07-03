#ifndef OBJECTONROOMPLAN_H
#define OBJECTONROOMPLAN_H

#include "roomplan.h"

// Обьект на плане помещения
class ObjectOnRoomPlan
{
protected:
    // Абцисса
    int x;
    // Ордината
    int y;

    // Ссылка на план помещения
    RoomPlan* roomPlan;

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    ObjectOnRoomPlan(int x, int y, RoomPlan* roomPlan);

    // Возвращает абциссу
    int getX();
    // Возвращает ординату
    int getY();

    virtual ~ObjectOnRoomPlan();
};

#endif // OBJECTONROOMPLAN_H
