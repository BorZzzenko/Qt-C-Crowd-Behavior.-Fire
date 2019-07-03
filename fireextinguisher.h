#ifndef FIREEXTINGUISHER_H
#define FIREEXTINGUISHER_H

#include "objectonroomplan.h"
#include "fire.h"

class Human;

// Огнетушитель
class FireExtinguisher : public ObjectOnRoomPlan
{
private:
    // Есть ли подобравший
    bool isOwned;

    // Ссылка на подобравшего
    Human* owner;

    // Мощность, количество урона по огню
    int POWER = 50;

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    explicit FireExtinguisher(int x, int y, RoomPlan* roomPlan);

    // Шаг
    void step();

    // Подобрал ли человек
    bool isHaveOwner();

    // Установка владельца
    // man - человек подобравший огнетушитель
    void setOwner(Human* man);

    // Сброс владельца
    void resetOwner();

    // Тушение пожара
    // fire - огонь
    void extinguish(Fire* fire);
};

#endif // FIREEXTINGUISHER_H
