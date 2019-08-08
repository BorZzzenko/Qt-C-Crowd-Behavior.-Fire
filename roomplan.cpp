#include "roomplan.h"
#include "objectonroomplan.h"
#include "human.h"
#include "fire.h"
#include "smoke.h"
#include "evacuationbutton.h"
#include "fireextinguisher.h"

RoomPlan::RoomPlan(int height, int width)
{
    this->height = height;
    this->width = width;

    evacuation = false;
    isBurning = false;

    peopleCount = 0;
    deathCount = 0;

    ignitionCoordinateX = 10;
    ignitionCoordinateY = 10;

    room = new int*[width];
    for (int i = 0; i < width; i++) {
        room[i] = new int[height];
    }

    updateRoom();
}

void RoomPlan::updateRoom()
{
    int num;
    QFile file("walls.dat");
     if(file.open(QIODevice::ReadOnly)){
         QTextStream fileStream(&file);

         for (int i = 0; i < height / CELL_WIDTH; i++) {
             for (int j = 0; j < width / CELL_WIDTH; j++) {
                 fileStream >> num;
                 room[j][i] = num;
             }
         }
     }

     file.close();
     setExitCoordinate();
}

int RoomPlan::getWidth()
{
    return width;
}

int RoomPlan::getHeight()
{
    return height;
}

int RoomPlan::getCellWidth()
{
    return CELL_WIDTH;
}

int RoomPlan::getExitX()
{
    return exitCoordinateX;
}

int RoomPlan::getExitY()
{
    return exitCoordinateY;
}

int RoomPlan::getIgnitionX()
{
    return ignitionCoordinateX;
}

int RoomPlan::getIgnitionY()
{
    return ignitionCoordinateY;
}

int RoomPlan::getPeopleCount()
{
    return peopleCount;
}

int RoomPlan::getDeathCount()
{
    return deathCount;
}

void RoomPlan::updateDeathCount()
{
    deathCount++;
}

void RoomPlan::updatePeopleCount(int count)
{
    peopleCount += count;
}

void RoomPlan::setEvacuation(bool p)
{
    evacuation = p;
}

bool RoomPlan::isEvacuation()
{
    return evacuation;
}

bool RoomPlan::isIgnition()
{
    return isBurning;
}

void RoomPlan::step()
{
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {

        if(Fire* fire = dynamic_cast<Fire*>(*(i))){
            fire->step();
        }
        else if(Smoke* smoke = dynamic_cast<Smoke*>(*(i))){
            smoke->step();
        }
        else if(Human* obj = dynamic_cast<Human*>(*(i))){
            obj->step();
        }
    }

    potentialIgnition();
}

void RoomPlan::potentialIgnition()
{
    // Если нет возгорания
    if(!isBurning){
        int chance = rand() % 300;

        // Если загорелось,
        // Либо огонь + дым
        if(chance > 295){
            isBurning = true;

            int number = rand() % 300;

           if(number < 150){
                int x, y;

                do {
                    x = (rand() % 40) * CELL_WIDTH;
                    y = (rand() % 40) * CELL_WIDTH;
                } while(!isFreeCoordinate(x , y));

                addFire(x, y);
                ignitionCoordinateX = x;
                ignitionCoordinateY = y;
            }
           // Либо дым
            else {
                int x, y;

                do {
                    x = (rand() % 40) * CELL_WIDTH;
                    y = (rand() % 40) * CELL_WIDTH;
                } while(!isFreeCoordinate(x , y));

                addSmoke(x, y);
                ignitionCoordinateX = x;
                ignitionCoordinateY = y;
            }
        }
    }
}

bool RoomPlan::isFreeCoordinate(int x, int y)
{
    ObjectOnRoomPlan* obj;
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        obj = *(i);
        if(obj->getX() == x && obj->getY() == y){
            if(Human* h = dynamic_cast<Human*>(*(i))){
                if(h->isDead())
                    return true;
            }
            return false;
        }

    }

    int tX = x / CELL_WIDTH;
    int tY = y / CELL_WIDTH;

    if(room[tX][tY] != 0){
        return false;
    }

    return true;
}

bool RoomPlan::isFreeCoordinateForStep(int x, int y)
{
    ObjectOnRoomPlan* obj;
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        obj = *(i);
        if(obj->getX() == x && obj->getY() == y) {
            if((obj = dynamic_cast<Smoke*>(*(i)))){
                return true;
            }
            if(EvacuationButton* but = dynamic_cast<EvacuationButton* >(*(i))){
                return true;
            }
            if(ExitPointer* pointer = dynamic_cast<ExitPointer* >(*(i))){
                return true;
            }
            if(FireExtinguisher* extinguisher = dynamic_cast<FireExtinguisher*>(*(i))){
                return true;
            }
            if(Human* h = dynamic_cast<Human*>(*(i))){                
                return true;
            }
            return false;
        }
    }

    int tX = x / CELL_WIDTH;
    int tY = y / CELL_WIDTH;

    if(room[tX][tY] != 0){
        return false;
    }

    return true;
}

bool RoomPlan::isFireCoordinate(int x, int y)
{
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        if(Fire* fire = dynamic_cast<Fire*>(*(i))){
            if(fire->getX() == x && fire->getY() == y)
                return true;
        }
    }
    return false;
}

bool RoomPlan::isSmokeCoordinate(int x, int y)
{
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        if(Smoke* smoke = dynamic_cast<Smoke*>(*(i))){
            if(smoke->getX() == x && smoke->getY() == y)
                return true;
        }
    }
    return false;
}

void RoomPlan::addObject(ObjectOnRoomPlan* obj)
{
    if(isFreeCoordinate(obj->getX(), obj->getY())){
        objects.push_back(obj);
    }
}

void RoomPlan::addHuman()
{
    if(isFreeCoordinate(exitCoordinateX, exitCoordinateY) && !evacuation){
        Human* human = new Human(exitCoordinateX, exitCoordinateY, this);
        objects.push_back(human);
        peopleCount++;
    }
}

void RoomPlan::addFire(int x, int y)
{
    if(isFreeCoordinateForStep(x, y)){
        Fire* fire = new Fire(x, y, this);
        objects.push_back(fire);
    }

    addSmoke(x + CELL_WIDTH, y);
    addSmoke(x, y + CELL_WIDTH);
    addSmoke(x - CELL_WIDTH, y);
    addSmoke(x, y - CELL_WIDTH);
}

void RoomPlan::addSmoke(int x, int y)
{
    if(isFreeCoordinateForStep(x, y)){
        Smoke* smoke = new Smoke(x, y, this);
        objects.push_back(smoke);
    }
}


void RoomPlan::deleteObject(ObjectOnRoomPlan* obj)
{
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        if(obj == *(i)){
            objects.erase(i);
            delete obj;
            return;
        }
    }
}

void RoomPlan::deleteSmoke(int x, int y)
{
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++) {
        if(Smoke* smoke = dynamic_cast<Smoke *>(*(i))){
            if(smoke->getX() == x && smoke->getY() == y){
                objects.erase(i);
                delete smoke;
            }
        }
    }
}

std::list<ObjectOnRoomPlan *> RoomPlan::getObjectsInRange(ObjectOnRoomPlan *obj, int range)
{
    std::list<ObjectOnRoomPlan* > list ;
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++){
        if(distance(obj, *(i)) <= range * range && !isBehindWall(obj, *(i))){
            list.push_back(*(i));
        }
    }

    return list;
}

int **RoomPlan::getRoomMatrix()
{
    int** arr = new int*[height];
    for (int i = 0; i < height; i++) {
        arr[i] = new int[width];
    }

    for (int i = 0; i < width / CELL_WIDTH; i++) {
        for (int j = 0; j < height / CELL_WIDTH; j++) {
            arr[i][j] = room[i][j];
        }
    }

    return arr;
}

int **RoomPlan::getObjectsMatrix()
{
    int** arr = new int*[height];
    for (int i = 0; i < height; i++) {
        arr[i] = new int[width];
    }

    for (int i = 0; i < width / CELL_WIDTH; i++) {
        for (int j = 0; j < height / CELL_WIDTH; j++) {
            arr[i][j] = room[i][j];
        }
    }

    ObjectOnRoomPlan* obj;
    for (std::list<ObjectOnRoomPlan* >::iterator i = objects.begin(); i != objects.end(); i++){
        // Огонь
        if((obj = dynamic_cast<Fire *>(*(i)))){
            if(arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] == EMPTY){
               arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] = FIRE;
            }
        }     
        // Дым 5
        else if((obj = dynamic_cast<Smoke *>(*(i)))){
            if(arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] == EMPTY){
               arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] = SMOKE;
            }
        }
        // Кнопка тревоги 6
        else if((obj = dynamic_cast<EvacuationButton *>(*(i)))){
            if(arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] != HUMAN){
               arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] = EVACUATION_BUT;
            }
        }
        // Табличка выход 7
        else if((obj = dynamic_cast<ExitPointer *>(*(i)))){
            if(arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] != HUMAN){
               arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] = EXIT_POINTER;
            }
        }
        // Огнетушитель 8
        else if((obj = dynamic_cast<FireExtinguisher *>(*(i)))){
            if(arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] != HUMAN){
               arr[obj->getX() / CELL_WIDTH][obj->getY() / CELL_WIDTH] = FIREEXTINGUISHER;
            }
        }
        // Человек 4
        else if(Human* h = dynamic_cast<Human *>(*(i))){
            if(h->isDead() && arr[h->getX() / CELL_WIDTH][h->getY() / CELL_WIDTH] != HUMAN)
                arr[h->getX() / CELL_WIDTH][h->getY() / CELL_WIDTH] = DEAD_HUMAN;

            else {
                arr[h->getX() / CELL_WIDTH][h->getY() / CELL_WIDTH] = HUMAN;
            }
        }
    }


    return arr;
}

// Берем случайную свободную точку,
// Если рядом есть стена то устанавливаем туда кнопку
void RoomPlan::generateEvacuationButtons(int count)
{
    int k = 0;

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCol[4] = {0, 0, 1 , -1};

    while(k < count){
        int x, y;

         x = (rand() % 40) * CELL_WIDTH;
         y = (rand() % 40) * CELL_WIDTH;

         if(isFreeCoordinate(x, y)){
             int r, c;
             for (int i = 0; i < 4; i++) {
                 r = x / CELL_WIDTH + directionRow[i];
                 c = y / CELL_WIDTH + directionCol[i];

                 if (room[r][c] == WALL) {
                     addObject(new EvacuationButton(x, y, this));
                     k++;
                     break;
                 }
             }
         }
    }
}

// Берем случайную свободную точку,
// Если рядом есть стена то устанавливаем туда указатель
void RoomPlan::generateExitPointers(int count)
{
    int counter = 0;

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCol[4] = {0, 0, 1 , -1};

    while(counter < count){
        int x, y;

         x = (rand() % 40) * CELL_WIDTH;
         y = (rand() % 40) * CELL_WIDTH;

         if(isFreeCoordinate(x, y)){
             int r, c;
             for (int i = 0; i < 4; i++) {
                 r = x / CELL_WIDTH + directionRow[i];
                 c = y / CELL_WIDTH + directionCol[i];

                 if (room[r][c] == WALL) {
                     addObject(new ExitPointer(x, y, this));
                     counter++;
                     break;
                 }
             }
         }
    }
}

// Берем случайную свободную точку и ставим туда огнетушитель
void RoomPlan::generateFireExtinguishers(int count)
{
    int k = 0;

    while(k < count){
        int x, y;

         x = (rand() % 40) * CELL_WIDTH;
         y = (rand() % 40) * CELL_WIDTH;

         if(isFreeCoordinate(x, y)){
             addObject(new FireExtinguisher(x, y, this));
             k++;
         }
    }
}

void RoomPlan::setExitCoordinate()
{
    int widthLength = width / CELL_WIDTH;

    for (int i = 0; i < widthLength; i++) {
        if(room[i][0] == EXIT){
            exitCoordinateX = i * CELL_WIDTH;
            exitCoordinateY = CELL_WIDTH;
            return;
        }
    }

    for (int i = 0; i < widthLength; i++) {
        if(room[i][widthLength - 1] == EXIT){
            exitCoordinateX = i * CELL_WIDTH;
            exitCoordinateY = (widthLength - 2) * CELL_WIDTH;
            return;
        }
    }

    int heightLength = height / CELL_WIDTH;

    for (int i = 0; i < widthLength; i++) {
        if(room[0][i] == EXIT){
            exitCoordinateX = CELL_WIDTH;
            exitCoordinateY = i * CELL_WIDTH;
            return;
        }
    }

    for (int i = 0; i < widthLength; i++) {
        if(room[heightLength - 1][i] == EXIT){
            exitCoordinateX = (heightLength - 2) * CELL_WIDTH;
            exitCoordinateY = i * CELL_WIDTH;
            return;
        }
    }
}

int RoomPlan::distance(ObjectOnRoomPlan *a, ObjectOnRoomPlan *b)
{
    return (a->getX() - b->getX()) * (a->getX() - b->getX()) + (a->getY() - b->getY()) * (a->getY() - b->getY());
}

bool RoomPlan::isBehindWall(ObjectOnRoomPlan *a, ObjectOnRoomPlan *b)
{
    // Вычисляем точку между объектами
    int x = b->getX() - a->getX();
    int y = b->getY() - a->getY();

    x = x > 0 ? 1 : x < 0 ? -1 : 0;
    y = y > 0 ? 1 : y < 0 ? -1 : 0;

    if(room[a->getX() / CELL_WIDTH + x][a->getY() / CELL_WIDTH + y] == WALL){
        return true;
    }

    return false;
}

