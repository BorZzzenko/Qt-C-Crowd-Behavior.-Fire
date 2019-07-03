#include "roomplandrawer.h"
#include "wall.h"
#include "human.h"
#include "fire.h"

#include <QScopedPointer>

RoomPlanDrawer::RoomPlanDrawer(QWidget *parent) : QWidget(parent)
{

}

RoomPlanDrawer::RoomPlanDrawer()
{

}

void RoomPlanDrawer::draw()
{
    // Вызывает paintEvent
    repaint();
}

void RoomPlanDrawer::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::white);

    //
    // Отрисовка сетки
    for(int i = 0; i < roomPlan->getWidth(); i += CELL_WIDTH){
        for(int j = 0; j < roomPlan->getHeight(); j+= CELL_WIDTH){
            painter.drawRect(i, j, CELL_WIDTH, CELL_WIDTH);
        }
    }

    painter.drawLine(0, 0, roomPlan->getWidth(), 0);
    painter.drawLine(0, 0, 0, roomPlan->getHeight());

    painter.drawLine(0, roomPlan->getHeight(), roomPlan->getWidth() - 1, roomPlan->getHeight() - 1);
    painter.drawLine(roomPlan->getWidth(), 0,roomPlan->getWidth() - 1, roomPlan->getHeight() - 1);

    int width = roomPlan->getWidth() / CELL_WIDTH;
    int height = roomPlan->getHeight() / CELL_WIDTH;
    //

    // Отрисовка объектов
    int** walls = roomPlan->getObjectsMatrix();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Стена 1
            if(walls[i][j] == 1){
                painter.setPen(Qt::white);
                painter.setBrush(QColor(1, 1, 1));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Выход 2
            else if(walls[i][j] == 2){
                painter.setPen(Qt::white);
                painter.setBrush(QBrush(Qt::black, Qt::BDiagPattern));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Человек 4
            else if(walls[i][j] == 4){
                painter.setPen(QColor(222, 184, 135));
                painter.setBrush(QColor(222, 184, 135));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Мертвый человек -4
            else if(walls[i][j] == -4){
                painter.setPen(QColor(139, 0, 0));
                painter.setBrush(QColor(139, 0, 0));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Огонь 3
            else if(walls[i][j] == 3){
                painter.setPen(Qt::white);
                painter.setBrush(QBrush(QColor(255, 140, 0), Qt::DiagCrossPattern));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }

            // Дым 5
            else if(walls[i][j] == 5){
                painter.setPen(Qt::white);
                painter.setBrush(QBrush(QColor(169, 169, 169), Qt::Dense5Pattern));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Кнопка 6
            else if(walls[i][j] == 6){
                painter.setPen(QColor(255, 130, 130));
                painter.setBrush(QBrush(QColor(255, 130, 130), Qt::Dense3Pattern));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Указатель 7
            else if(walls[i][j] == 7){
                painter.setPen(QColor(0, 250, 154));
                painter.setBrush(QColor(0, 250, 154));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
            // Огнетушитель
            else if(walls[i][j] == 8){
                painter.setPen(QColor(220, 20, 80));
                painter.setBrush(QColor(220, 20, 80));

                painter.drawRect(i * CELL_WIDTH, j * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH);
            }
        }
    }

    for (int i = 0; i < width; i++){
            delete [] walls[i];
    }
}

void RoomPlanDrawer::setRoomPlan(RoomPlan* roomPlan)
{
    this->roomPlan = roomPlan;
}

