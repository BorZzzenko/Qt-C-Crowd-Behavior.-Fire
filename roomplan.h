#ifndef ROOMPLAN_H
#define ROOMPLAN_H

#include <QFile>
#include <QTextStream>

class Human;
class ObjectOnRoomPlan;

// План помещения
class RoomPlan
{
private:   
    // Ширина клетки
    const int CELL_WIDTH = 10;
    // Максимальное количество людей в комнате
    const int MAX_HUMAN = 5;

    // Ширина план помещения
    int width;
    // Высота плана помещения
    int height;

    // Координаты выхода
    int exitCoordinateX;
    int exitCoordinateY;

    // Координаты очага возгорания
    int ignitionCoordinateX;
    int ignitionCoordinateY;

    // Идет ли эвакуация
    bool evacuation;
    // Есть ли возгорания
    bool isBurning;

    // Количество людей в помещении
    int peopleCount;
    // Количество мертвых
    int deathCount;

    // Лист всех объектов
    std::list<ObjectOnRoomPlan* > objects;

    // Массив со стенами
    int** room;

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
    // height - длинна
    // width - высота
    RoomPlan(int height, int width);

    // Считывает массив стен из файла
    void updateRoom();

    // Возвращает ширину
    int getWidth();
    // Возвращает высоту
    int getHeight();
    // Возвращает ширину клетки
    int getCellWidth();

    // Возвращает максимальное количество людей в комнате
    int getMaxHumanInRoom();

    // Возвращают координаты выхода
    int getExitX();
    int getExitY();

    // Возвращают координаты возгорания
    int getIgnitionX();
    int getIgnitionY();

    // Возвращает количество людей
    int getPeopleCount();
    // Возвращает количество мертвых
    int getDeathCount();

    // Увеличивает число людей
    // count - количесвто людей
    void updatePeopleCount(int count);
    // Увеличивает число умерших
    void updateDeathCount();

    // Устанавливает статус эвакуации
    void setEvacuation(bool evacuation);

    // Возвращает true, если идет эвакуация
    bool isEvacuation();
    // Возвращает true, если есть возгорание
    bool isIgnition();

    // Шаг обьектов помещения
    void step();

    // Проверяет свободны ли координаты
    // x - абцисса
    // у - ордината
    bool isFreeCoordinate(int x, int y);

    // Проверяет свободны ли координаты
    // от обьектов по которым можно ходить(дым, кнопки, указатели, огнетушители, мертвецы)
    // x - абцисса
    // у - ордината
    bool isFreeCoordinateForStep(int x, int y);

    // Возвращает true, если на этих координатах есть огонь
    // х - абцисса
    // у - ордината
    bool isFireCoordinate(int x, int y);

    // Возвращает true, если на этих координатах есть дым
    // х - абцисса
    // у - ордината
    bool isSmokeCoordinate(int x, int y);

    // Добавляет обьект в помещение на пустую клетку
    // obj - ссылка на объект
    void addObject(ObjectOnRoomPlan* obj);

    // Добавляет человека на вход
    void addHuman();

    // Добавляет огонь и дым вокруг него
    // x - абцисса огня
    // у - ордината огня
    void addFire(int x, int y);

    // Добавляет дым
    // x - абцисса дыма
    // у - ордината дыма
    void addSmoke(int x, int y);

    // Удаляет обьект из помещения
    // obj - ссылка на объект
    void deleteObject(ObjectOnRoomPlan* obj);

    // Удаляет дым
    // x - абцисса дыма
    // у - ордината дыма
    void deleteSmoke(int x, int y);

    // Возвращает список объектов в радиусе видимости
    // obj - для которого ищем
    // range - радиус
    std::list<ObjectOnRoomPlan *> getObjectsInRange(ObjectOnRoomPlan* obj, int range);

    // Возвращает матрицу стен
    int **getRoomMatrix();

    // Возвращает матрицу объектов
    int **getObjectsMatrix();

    // Генерирует кнопки эвакуации
    // count - их количество
    void generateEvacuationButtons(int count);

    // Генерирует указатели "Выход"
    // count - их количество
    void generateExitPointers(int count);

    // Генерирует огнетушители
    // count - их количество
    void generateFireExtinguishers(int count);

private:
    // Находит выход в матрице стен,
    // устанавливает его координаты
    void setExitCoordinate();

    // Случайное возгорание
    void potentialIgnition();

    // Возвращает true если объекты разделяет стена
    // a - объект в помещении
    // b - объект в помещении
    bool isBehindWall(ObjectOnRoomPlan* a, ObjectOnRoomPlan* b);

    // Считает растояние между объектами
    // a - объект в помещении
    // b - объект в помещении
    // Возвращает расстояние
    int distance(ObjectOnRoomPlan* a, ObjectOnRoomPlan* b);
};

#endif // ROOMPLAN_H
