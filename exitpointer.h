#ifndef EXITPOINTER_H
#define EXITPOINTER_H
#include "objectonroomplan.h"

// Указатель "выход"
class ExitPointer : public ObjectOnRoomPlan
{
public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    explicit ExitPointer(int x, int y, RoomPlan* roomPlan);

    // Получение координат выхода
    // x - ссылка в которую будет записана абцисса выхода
    // y - ссылка в которую будет записана ордината выхода
    void getExitCoordinate(int* x, int* y);
};

#endif // EXITPOINTER_H
