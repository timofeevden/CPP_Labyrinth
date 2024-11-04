#ifndef VIEW_S21_MAINWINDOW_H
#define VIEW_S21_MAINWINDOW_H

#include <QDateTime>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>

#include "s21_graphic_widget.h"
#include "../Controller/s21_controller_cave.h"
#include "../Controller/s21_controller_maze.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(ControllerCave* cntr_cave_, ControllerMaze* cntr_maze_, QWidget *parent = nullptr);
    ~MainWindow();

private:
    void LoadMaze();
    void LoadCave();

private slots:
    void onCaveWidthSliderValueChanged(int value);
    void onMazeWidthSliderValueChanged(int value);
    void onMazeHeightSliderValueChanged(int value);
    void onCaveHeightSliderValueChanged(int value);
    void onBirthSliderValueChanged(int value);
    void onDeathSliderValueChanged(int value);
    void on_tabWidget_currentChanged(int tab_index);
    void on_generate_maze_pushButton_clicked();
    void on_reset_pushButton_clicked();
    void on_next_step_pushButton_clicked();
    void on_generate_cave_pushButton_clicked();
    void on_auto_cave_start_pushButton_clicked();
    void AutoNextStep();
    void StopAutoMode();
    void LoadFile();
    void SaveFile();

private:
    ControllerCave* controller_cave_;
    ControllerMaze* controller_maze_;
    Ui::MainWindow *ui_;
    GraphicWidget *graphic_widget_;
    QTimer* timer_;
    std::string path_;
    bool auto_mode_;
    bool is_scene_not_empty_;
};

}  // namespace s21

#endif // VIEW_S21_MAINWINDOW_H
