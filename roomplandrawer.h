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
