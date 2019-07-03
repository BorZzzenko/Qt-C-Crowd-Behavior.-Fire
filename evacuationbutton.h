#ifndef EVACUATIONBUTTON_H
#define EVACUATIONBUTTON_H
#include "objectonroomplan.h"

// Кнопка эвакуации
class EvacuationButton : public ObjectOnRoomPlan
{

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    explicit EvacuationButton(int x, int y, RoomPlan* roomPlan);

    // Нажатие на кнопку,
    // включение сигнализации
    void push();
};

#endif // EVACUATIONBUTTON_H
