#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QShortcut>
#include "roomplan.h"
#include "roomplandrawer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT

private:
    // Ссылка на помещение для моделирования
    RoomPlan* roomPlan;

    // Ссылка на обьект для рисования помещения
    RoomPlanDrawer* roomPlanDrawer;

    // Таймер для моделирования времени
    QTimer* timer = new QTimer();
    const int FPS = 400;

    // Горячая клавиша запуска и остановки модели
    QShortcut* enterKey;
    // Горячая клавиша закрытия формы
    QShortcut* escapeKey;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void shortCutEnter();

    // Шаг модели
    void stepModel();

    void on_closeButton_clicked();
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_updateButton_clicked();
};

#endif // MAINWINDOW_H
