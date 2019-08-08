#ifndef ROOMPLANDRAWER_H
#define ROOMPLANDRAWER_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "roomplan.h"


// Класс для отрисовки плана помещения на форме
// Класс привязан к QWidget на котором будет производится отрисовка
class RoomPlanDrawer : public QWidget
{
private:
    // План, который рисуем
    RoomPlan* roomPlan;

    // Ширина клетки
    const int CELL_WIDTH = 10;

    enum objOnRoomPlan {
        EMPTY = 0,
        WALL = 1,
        EXIT = 2,
        FIRE = 3,
        HUMAN = 4,
        DEAD_HUMAN = -4,
        SMOKE = 5,
        EVACUATION_BUT = 6,
        EXIT_POINTER = 7,
        FIREEXTINGUISHER = 8
    };

public:
    // Конструктор
    // parent - форма
    explicit RoomPlanDrawer(QWidget* parent = nullptr);

    // Конструктор
    RoomPlanDrawer();

    // Рисование
    void draw();

    // Событие отрисовки
    void paintEvent(QPaintEvent* event);

    // Установка плана помещения
    // roomPlan - план помещения, который рисуем
    void setRoomPlan(RoomPlan* roomPlan);
};

#endif // ROOMPLANDRAWER_H
