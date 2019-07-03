#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fireextinguisher.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("fireIcon.png"));

    enterKey = new QShortcut(this);
    enterKey->setKey(Qt::Key::Key_Enter);
    connect(enterKey, SIGNAL(activated()), this, SLOT(shortCutEnter()));

    escapeKey = new QShortcut(this);
    escapeKey->setKey(Qt::Key::Key_Escape);
    connect(escapeKey, SIGNAL(activated()), this, SLOT(on_closeButton_clicked()));

    roomPlan = new RoomPlan(400, 400);
    roomPlan->addHuman();

    srand(time(nullptr));

    roomPlan->generateEvacuationButtons(10);
    roomPlan->generateExitPointers(10);
    roomPlan->generateFireExtinguishers(10);

    roomPlanDrawer = ui->paintWidget;
    roomPlanDrawer->setRoomPlan(roomPlan);    
    roomPlanDrawer->draw();

    connect(timer, SIGNAL(timeout()), this, SLOT(stepModel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::shortCutEnter()
{
    if(timer->isActive()){
        timer->stop();
    }
    else {
        timer->start(FPS);
    }
}

void MainWindow::stepModel()
{
    // Вывод информации на форму
    roomPlan->isEvacuation() ? ui->evacuationLabel->setText("true") : ui->evacuationLabel->setText("false");
    roomPlan->isIgnition() ? ui->ignitionLabel->setText("true") : ui->ignitionLabel->setText("false");

    ui->peopleCountLabel->setText(QString::number(roomPlan->getPeopleCount()));
    ui->deadCountLabel->setText(QString::number(roomPlan->getDeathCount()));

    // Если количество людей = 0
    if(roomPlan->getPeopleCount() == 0){
        // Если идет эвакуация или заблокирован выход,
        // остановка модели
        if(roomPlan->isEvacuation() ||
           !roomPlan->isFreeCoordinate(roomPlan->getExitX(), roomPlan->getExitY())){
            timer->stop();
        }
    }

    // Случайное добавление человека
    if(roomPlan->getPeopleCount() < 5){
        int chance = rand() % 100;
        if(chance >= 95){
           roomPlan->addHuman();
        }
    }

    // Шаг модели и отрисовка
    roomPlan->step();
    roomPlanDrawer->draw();
}

void MainWindow::on_closeButton_clicked()
{
    close();
}

void MainWindow::on_startButton_clicked()
{
    timer->start(FPS);
}

void MainWindow::on_stopButton_clicked()
{
    timer->stop();
}

void MainWindow::on_updateButton_clicked()
{
    roomPlan->updateRoom();
}
