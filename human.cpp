#include "human.h"
#include "fire.h"
#include "smoke.h"
#include <QMessageBox>

Human::Human(int x, int y, RoomPlan* roomPlan): ObjectOnRoomPlan (x, y, roomPlan)
{
    dead = false;
    isPanic = false;
    isKnowsExitWay = false;
    isHaveFireExtinguisher = false;

    visitTime = 0;
    smokeHealthPoint = 50;
    fireHelathPoint = 10;

    courage = rand() % 100;

    fireExtinguisher = nullptr;
}

bool Human::isDead()
{
    return dead;
}

void Human::resetPath()
{
    while (!pathStack.empty()) {
        pathStack.pop();
    }
}

void Human::step()
{
    if(dead){
        return;
    }

    // Если включается эвакуация, то сбрасывается путь
    if(roomPlan->isEvacuation() && !isPanic && !isHaveFireExtinguisher){
        isPanic = true;
        resetPath();
    }

    // Выход
    if(x == roomPlan->getExitX() && y == roomPlan->getExitY() && (visitTime > 150 || isPanic)){
        leave();
        return;
    }

    // Проверяет есть ли рядом дым или огонь и получает урон от них
    if (isPanic){
        getDamage();
    }

    // Обьекты, которые человек видит
    std::list<ObjectOnRoomPlan* > objectList = roomPlan->getObjectsInRange(this, VISION_RANGE);
    ObjectOnRoomPlan* obj;

    for (std::list<ObjectOnRoomPlan* >::iterator i = objectList.begin(); i != objectList.end(); i++) {
        // Если еще не паникует
        if(!isPanic) {
            // Если видит огонь или дым, начинает паниковать
            if((obj = dynamic_cast<Fire *>(*(i)))){
                isPanic = true;
                findPathBfs(roomPlan->getExitX(), roomPlan->getExitY());
            }
            if((obj = dynamic_cast<Smoke *>(*(i)))){
                isPanic = true;
                findPathBfs(roomPlan->getExitX(), roomPlan->getExitY());
            }
        }

        // Если паникует
        if(isPanic) {
            // Если еще нет эвакуации и видит кнопку,
            // Ищет к ней путь, или нажимает если уже пришел
            if(!roomPlan->isEvacuation()) {
                if(EvacuationButton* but = dynamic_cast<EvacuationButton *>(*(i))){
                    if(but->getX() == x && but->getY() == y){
                        pushButton(but);
                        if(isHaveFireExtinguisher){
                            findPathBfsInFire(roomPlan->getIgnitionX(), roomPlan->getIgnitionY());
                        }
                    }
                    else {
                         findPathBfs(but->getX(), but->getY());
                    }
                }
            }

            // Если не знает путь и видит указатель "Выход",
            // Ищет к нему путь, или изучает его, если уже пришел
            if(!isKnowsExitWay){
                if(ExitPointer* point = dynamic_cast<ExitPointer* >(*(i))){
                    if(point->getX() == x && point->getY() == y){
                        watchExitPointer(point);

                        if(isHaveFireExtinguisher){
                            findPathBfsInFire(roomPlan->getIgnitionX(), roomPlan->getIgnitionY());
                        }                      
                    }
                    else {
                        findPathBfs(point->getX(), point->getY());
                    }
                }
            }

            // Если храбрость > 70 и нет огнетушителя
            if(courage > 70 && !isHaveFireExtinguisher){
                // Если видит огнетушитель и его еще не взяли,
                // Ищет к нему путь, или берет его и ищет путь к очагу возгорания, если уже пришел
                if(FireExtinguisher* extinguisher = dynamic_cast<FireExtinguisher* >(*(i))){
                    if(!extinguisher->isHaveOwner()){
                        if(extinguisher->getX() == x && extinguisher->getY() == y){
                            takeFireExtinguisher(extinguisher);
                            goToIgnition();
                        }
                        else {
                            findPathBfs(extinguisher->getX(), extinguisher->getY());                          
                        }
                    }
                }
            }

            // Если есть огнетушитель и видит огонь,
            // Начинает его тушить и идет на его место
            if(isHaveFireExtinguisher){
                if(Fire* fire = dynamic_cast<Fire *>(*(i))){
                    useFireExtinguisher(fire);
                    findPathBfsInFire(fire->getX(), fire->getY());
                    return;
                }
            }
        }
    }

    // Если не знает путь (не читал табличку) и находится в дыму,
    // Не может найти путь - движется в случайную соседнюю клетку
    if(!isKnowsExitWay && roomPlan->isSmokeCoordinate(x, y)){
        goRandomInSmoke();
        resetPath();
        return;
    }

    // Если есть найденный путь, идет по нему
    if(!pathStack.empty()){
            goPath();
    }
    // Иначе, если долго в помещении или паникует, то идет к выходу
    else if (visitTime >= 150 || isPanic){
        resetPath();
        if(!findPathBfs(roomPlan->getExitX(), roomPlan->getExitY())){
            findPathBfsInFire(roomPlan->getExitX(), roomPlan->getExitY());
        }
    }
    // Иначе идет в случайную точку
    else {
        goRandomPath();
    }

    // Несет огнетушитель
    if(isHaveFireExtinguisher){
        fireExtinguisher->step();
    }

    visitTime++;
}

void Human::goRandomPath()
{
    int x, y;
    do {
        x = (rand() % 40) * roomPlan->getCellWidth();
        y = (rand() % 40) * roomPlan->getCellWidth();
    } while(!roomPlan->isFreeCoordinateForStep(x , y));

    if(!findPathBfs(x, y)){
        findPathBfsInFire(x, y);
    }
}

void Human::goRandomInSmoke()
{
    int r, c;
    do {
        r = (rand() % 3 - 1) * roomPlan->getCellWidth();
        c = (rand() % 3 - 1) * roomPlan->getCellWidth();
    } while(!roomPlan->isFreeCoordinateForStep(x + r, y + c));

    setCoordinate(x + r, y + c);
    if(isHaveFireExtinguisher){
        fireExtinguisher->step();
    }
}

void Human::setCoordinate(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Human::findPathBfs(int x, int y)
{
    resetPath();
    std::queue<int> qX;     // очередь для координат X
    std::queue<int> qY;     // очередь для координат Y

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCell[4] = {0, 0, 1 , -1};

    int width = roomPlan->getWidth() / roomPlan->getCellWidth();
    int height = roomPlan->getHeight() / roomPlan->getCellWidth();

    int** room = new int*[width];
    for (int i = 0; i < width; i++) {
        room[i] = new int[height];
    }

    int** arr = roomPlan->getObjectsMatrix();

    // Копируется матрица объектов в комнате
    // Дым и огонь, стена считаются не проходимым препятсвием
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if(arr[i][j] > 3 && arr[i][j] != 5){
                room[i][j] = 0;
            }
            else {
                room[i][j] = -arr[i][j];
            }
        }
    }

    x /= roomPlan->getCellWidth();
    y /= roomPlan->getCellWidth();

    // Текущая клетка помещается в очередь,
    // в матрицу ставится значение 1
    qX.push(this->x / roomPlan->getCellWidth());
    qY.push(this->y / roomPlan->getCellWidth());

    room[this->x / roomPlan->getCellWidth()] [this->y / roomPlan->getCellWidth()] = 1;

    // Цикл BFS
    while(!qX.empty()){
        int row = qX.front();
        qX.pop();

        int colmn = qY.front();
        qY.pop();

        if(row == x && colmn == y){
            break;
        }

        // Если сверху, внизу, справо, слево не занято,
        // ставим туда значение текущей клетки + 1
        // помещаем в очередь
        int r, c;
        for (int i = 0; i < 4; i++) {
            r = row + directionRow[i];
            c = colmn + directionCell[i];

            if (room[r][c] == 0){
                qX.push(r);
                qY.push(c);

                room[r][c] = room[row][colmn] + 1;
            }

            if(r == x && c == y){
                break;
            }
        }
    }

    // Если в искомой точке нет значение, то туда нет путь
    int count = room[x][y];
    if(count <= 0){
        return false;
    }

    // Пока не вернемся в начальное положение
    // Проверяем соседние клетки на наличие клетки с значением текущей клетки - 1,
    // идем в нее и помещаем координаты в стек пути

    int a = x, b = y;
    int r = x, c = y;

    pathStack.push(a * roomPlan->getCellWidth());
    pathStack.push(b * roomPlan->getCellWidth());

    while(room[a][b] != 1) {
        for (int i = 0; i < 4; i++) {
            a = r + directionRow[i];
            b = c + directionCell[i];

            if (room[a][b] == count - 1) {
                pathStack.push(a * roomPlan->getCellWidth());
                pathStack.push(b * roomPlan->getCellWidth());

                count--;
                r = a;
                c = b;

                break;
            }
        }
    }

    pathStack.pop();
    pathStack.pop();

    for (int i = 0; i < width; i++){
            delete [] room[i];
    }

    for (int i = 0; i < width; i++){
            delete [] arr[i];
    }

    return true;
}

// Поиск пути, где огонь и дым считаются преодолимыми препятсвиями
void Human::findPathBfsInFire(int x, int y)
{
    resetPath();

    std::queue<int> qX;
    std::queue<int> qY;

    int directionRow[4] = {-1, 1, 0 , 0};
    int directionCell[4] = {0, 0, 1 , -1};

    int width = roomPlan->getWidth() / roomPlan->getCellWidth();
    int height = roomPlan->getHeight() / roomPlan->getCellWidth();

    int** room = new int*[width];
    for (int i = 0; i < width; i++) {
        room[i] = new int[height];
    }

    int** arr = roomPlan->getRoomMatrix();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            room[i][j] = -arr[i][j];
        }
    }

    x /= roomPlan->getCellWidth();
    y /= roomPlan->getCellWidth();

    qX.push(this->x / roomPlan->getCellWidth());
    qY.push(this->y / roomPlan->getCellWidth());

    room[this->x / roomPlan->getCellWidth()][this->y / roomPlan->getCellWidth()] = 1;

    while(!qX.empty()){
        int row = qX.front();
        qX.pop();

        int colmn = qY.front();
        qY.pop();

        if(row == x && colmn == y){
            break;
        }

        int r, c;
        for (int i = 0; i < 4; i++) {
            r = row + directionRow[i];
            c = colmn + directionCell[i];

            if (room[r][c] == 0){
                qX.push(r);
                qY.push(c);

                room[r][c] = room[row][colmn] + 1;
            }

            if(r == x && c == y){
                break;
            }
        }
    }

    int count = room[x][y];
    if(count <= 0){
        return;
    }

    int a = x, b = y;
    int r = x, c = y;

    pathStack.push(a * roomPlan->getCellWidth());
    pathStack.push(b * roomPlan->getCellWidth());

    while(room[a][b] != 1) {
        for (int i = 0; i < 4; i++) {
            a = r + directionRow[i];
            b = c + directionCell[i];

            if (room[a][b] == count - 1) {
                pathStack.push(a * roomPlan->getCellWidth());
                pathStack.push(b * roomPlan->getCellWidth());

                count--;
                r = a;
                c = b;

                break;
            }
        }
    }

    pathStack.pop();
    pathStack.pop();

    for (int i = 0; i < width; i++){
            delete [] room[i];
    }

    for (int i = 0; i < width; i++){
            delete [] arr[i];
    }
}

void Human::goPath()
{
    int y = pathStack.top();
    pathStack.pop();

    int x = pathStack.top();
    pathStack.pop();


    setCoordinate(x, y);
}

void Human::leave()
{
    roomPlan->deleteObject(this);
    roomPlan->updatePeopleCount(-1);
    if(isHaveFireExtinguisher){
        roomPlan->deleteObject(fireExtinguisher);
    }
}

void Human::getDamage()
{
    int cell = roomPlan->getCellWidth();

    if(roomPlan->isFireCoordinate(x, y)){
        fireHelathPoint--;
    }
    else if(roomPlan->isFireCoordinate(x + cell, y)){
        fireHelathPoint--;
    }
    else if(roomPlan->isFireCoordinate(x - cell, y)){
        fireHelathPoint--;
    }
    else if(roomPlan->isFireCoordinate(x, y + cell)){
        fireHelathPoint--;
    }
    else if(roomPlan->isFireCoordinate(x, y - cell)){
        fireHelathPoint--;
    }

    if(roomPlan->isSmokeCoordinate(x, y)){
        smokeHealthPoint--;
    }

    if(smokeHealthPoint <= 0){
        death();
    }
    else if (fireHelathPoint <= 0) {
        death();
    }
}

void Human::death()
{
    dead = true;

    roomPlan->updatePeopleCount(-1);
    roomPlan->updateDeathCount();

    if(isHaveFireExtinguisher){
        fireExtinguisher->resetOwner();
    }
}

void Human::pushButton(EvacuationButton *button)
{
    button->push();
}

void Human::watchExitPointer(ExitPointer* pointer)
{
    int* a = new int;
    int* b = new int;
    pointer->getExitCoordinate(a, b);
    isKnowsExitWay = true;

    if(!isHaveFireExtinguisher){
        findPathBfs(*a, *b);
    }

    delete a;
    delete b;
}

void Human::takeFireExtinguisher(FireExtinguisher* fireExtinguisher)
{
    this->fireExtinguisher = fireExtinguisher;
    isHaveFireExtinguisher = true;

    fireExtinguisher->setOwner(this);
}

void Human::goToIgnition()
{
    findPathBfsInFire(roomPlan->getIgnitionX(), roomPlan->getIgnitionY());
    isKnowsExitWay = true;
}

void Human::useFireExtinguisher(Fire *fire)
{
    fireExtinguisher->extinguish(fire);
}



