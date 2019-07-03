#ifndef HUMAN_H
#define HUMAN_H

#include <random>
#include <queue>
#include <stack>
#include "objectonroomplan.h"
#include "evacuationbutton.h"
#include "exitpointer.h"
#include "fireextinguisher.h"

// Человек
class Human : public ObjectOnRoomPlan
{
private:
    // Время посещения помещения
    int visitTime;

    // Очки здоровья в дыму
    int smokeHealthPoint;

    // Очки здоровья в огне
    int fireHelathPoint;

    // Храбрость
    int courage;

    // Умер ли человек
    bool dead;

    // Паникует ли человек
    bool isPanic;

    // Знает ли он путь к выходу
    bool isKnowsExitWay;

    // Есть ли у него огнетушитель
    bool isHaveFireExtinguisher;

    // Ссылка на огнетушитель
    FireExtinguisher* fireExtinguisher;

    // Стек содержащий путь
    std::stack<int> pathStack;

    // Радиус зрения
    int const VISION_RANGE = 20;

public:
    // Конструктор
    // x - абцисса на плане помещения
    // y - ордината на плане
    // roomPlan - ссылка на план
    explicit Human(int x, int y, RoomPlan* roomPlan);

    // Мертв ли человек
    bool isDead();

    // Шаг
    void step();

private:
    // Идти в рандомную точку
    void goRandomPath();

    // Двигаться рандомно в дыму
    void goRandomInSmoke();

    // Установить значения координат
    // x - новая абцисса
    // y - новая ордината
    void setCoordinate(int x, int y);

    // Поиск пути в точку, алгоритмом поиска в ширину
    // Возвращает true если есть путь
    bool findPathBfs(int x, int y);

    // Поиск пути через дым и огонь
    void findPathBfsInFire(int x, int y);

    // Идти по имеющемуся пути
    void goPath();

    // Покинуть помещение
    void leave();

    // Получение урона от дыма и огня
    void getDamage();

    // Смерть
    void death();

    // Сбросить имеющийся путь
    void resetPath();

    // Нажать кнопку эвакуации
    // button - ссылка на кнопку
    void pushButton(EvacuationButton* button);

    // Узнать путь к выходу по указателю
    // pointer - ссылка на указатель
    void watchExitPointer(ExitPointer* pointer);

    // Взять огнетушитель
    // fireExtinguisher - огнетушитель
    void takeFireExtinguisher(FireExtinguisher* fireExtinguisher);

    void goToIgnition();

    // Тушить огонь
    // fire - сслыка на огонь
    void useFireExtinguisher(Fire* fire);
};

#endif // HUMAN_H
