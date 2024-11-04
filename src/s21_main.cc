#include <QApplication>

#include "View/s21_mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::ModelCave model_cave;
  s21::ModelMaze model_maze;
  s21::ControllerCave controller_cave(&model_cave);
  s21::ControllerMaze controller_maze(&model_maze);
  s21::MainWindow view(&controller_cave, &controller_maze);
  view.setWindowTitle("Maze");
  view.show();
  return a.exec();
}
