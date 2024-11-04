#ifndef VIEW_S21_GRAPHICSCENE_H
#define VIEW_S21_GRAPHICSCENE_H

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWidget>

#include "../Controller/s21_controller_cave.h"
#include "../Controller/s21_controller_maze.h"

namespace s21 {

class GraphicWidget : public QGraphicsView
{
public:
    explicit GraphicWidget(QWidget *ui, ControllerCave* cntr_cave_, ControllerMaze* cntr_maze_);
    ~GraphicWidget();

    void DrawCave();
    void DrawMaze();
    void DrawMazeWay();
    void Clear(QString message);
    void SetMazeMode(bool mode);
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::pair<int, int> GetNumberOfPos(float x, float y);
    std::pair<float, float> GetMiddleCoordsOfCell(std::pair<int, int> pos);

    QWidget *ui_;
    QGraphicsScene *scene_;
    s21::ControllerCave* controller_cave_;
    s21::ControllerMaze* controller_maze_;
    std::pair<int, int> cave_size_;
    std::pair<int, int> maze_size_;
    QGraphicsRectItem* start_point_;
    QGraphicsRectItem* finish_point_;
    int cnt_drawn_points_;
    float step_x_;
    float step_y_;
    bool is_maze_;

signals:
    void signalMouseCoords(QPointF point);

public slots:
    void slotMouseCoords(QPointF point);
};

}  // namespace s21

#endif // VIEW_S21_GRAPHICSCENE_H
