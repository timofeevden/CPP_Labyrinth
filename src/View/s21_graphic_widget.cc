#include "s21_graphic_widget.h"

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPen>
#include <QWidget>

namespace s21 {

GraphicWidget::GraphicWidget(QWidget *ui, ControllerCave* cntr_cave_, ControllerMaze* cntr_maze_)
    : ui_(ui), controller_cave_(cntr_cave_), controller_maze_(cntr_maze_)
{
    scene_ = new QGraphicsScene();
    start_point_ = nullptr;
    finish_point_ = nullptr;
    cnt_drawn_points_ = 0;
    is_maze_ = false;
    step_x_ = 0.0;
    step_y_ = 0.0;

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключим скроллбар по горизонтали
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Отключим скроллбар по вертикали
    setAlignment(Qt::AlignCenter);                        // Делаем привязку содержимого к центру
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Растягиваем содержимое по виджету

     setSceneRect(-250, -250, 500, 500);
     setScene(scene_);
}

GraphicWidget::~GraphicWidget() {}

void GraphicWidget::DrawCave() {
    is_maze_ = false;
    scene_->clear();
    items().clear();
    cave_size_ = controller_cave_->GetSize();

    if (!controller_cave_->GetError()) {
        std::vector<std::vector<bool>> data = controller_cave_->GetData();
        float step_x = 500.0 / cave_size_.second;
        float step_y = 500.0 / cave_size_.first;

        for (int i = 0; i < cave_size_.first; ++i) {
            for (int j = 0; j < cave_size_.second; ++j) {
                if (data[i][j] == 1) {
                    scene_->addRect(step_x * j - 250.0, step_y * i  - 250.0, step_x, step_y, Qt::NoPen, QBrush(Qt::black));
                } else {
                    scene_->addRect(step_x * j - 250.0, step_y * i  - 250.0, step_x, step_y, Qt::NoPen, QBrush(Qt::white));
                }
            }
        }
    }
    scene_->update();
}

void GraphicWidget::DrawMaze() {
    scene_->clear();
    items().clear();
    cnt_drawn_points_ = 0;
    maze_size_ = controller_maze_->GetSize();

    if (!controller_maze_->GetError() && maze_size_.first && maze_size_.second) {
        is_maze_ = true;
        std::vector<std::vector<bool>> vertical_walls = controller_maze_->GetVerticalData();
        std::vector<std::vector<bool>> horisontal_walls = controller_maze_->GetHorisontalData();
        step_x_ = 500.0 / maze_size_.second;
        step_y_ = 500.0 / maze_size_.first;
        QPen lines_pen(Qt::white);  //  Цвет и толщина линий
        lines_pen.setWidth(2);
        //  Рамка
        scene_->addLine(-250, -250, -250, 250, lines_pen);
        scene_->addLine(-250, -250, 250, -250, lines_pen);
        scene_->addLine(250, 250, 250, -250, lines_pen);
        scene_->addLine(250, 250, -250, 250, lines_pen);

        for (int i = 0; i < maze_size_.first; ++i) {
            for (int j = 0; j < maze_size_.second; ++j) {
                if (vertical_walls[i][j]) {
                    scene_->addLine(step_x_ * (j + 1.0) - 250.0, step_y_ * i  - 250.0,
                                    step_x_ * (j + 1.0) - 250.0, step_y_ * (i + 1.0)  - 250.0, lines_pen);
                }
                if (horisontal_walls[i][j]) {
                    scene_->addLine(step_x_ * j  - 250.0, step_y_ * (i + 1.0)  - 250.0,
                                    step_x_ * (j + 1.0)  - 250.0, step_y_ * (i + 1.0)  - 250.0, lines_pen);
                }
            }
        }
        start_point_ = scene_->addRect(-5, -5, 10, 10, QPen(), QBrush(Qt::blue));
        finish_point_ = scene_->addRect(-5, -5, 10, 10, QPen(), QBrush(Qt::red));
        start_point_->hide();
        finish_point_->hide();
        start_point_->setZValue(1);
        finish_point_->setZValue(1);
    }
    scene_->update();
}


void GraphicWidget::mousePressEvent(QMouseEvent *event) {
    if (is_maze_ && !controller_maze_->GetError()) {
        float x = event->localPos().x() - 250.0;    //  считываем коорд-ты курсора
        float y = event->localPos().y() - 250.0;
        std::pair<int, int> pos = GetNumberOfPos(x, y); //  получаем номер ячейки
        std::pair<float, float> coords = GetMiddleCoordsOfCell(pos);    //  находим центр ячейки
        x = coords.first;
        y = coords.second;

        if (cnt_drawn_points_ == 0) {   // добавляем старт
            start_point_->setPos(x, y);
            start_point_->show();
            ++cnt_drawn_points_;
            controller_maze_->SetNewStart(pos.first, pos.second);
        } else if (cnt_drawn_points_ == 1 && pos != controller_maze_->GetStartPos()) {  // добавляем финиш
            finish_point_->setPos(x, y);
            finish_point_->show();
            ++cnt_drawn_points_;
            controller_maze_->SetNewFinish(pos.first, pos.second);
            DrawMazeWay();  //  тут начинаем строить маршрут
        } else if (cnt_drawn_points_ == 2) {    //  очишаем
            cnt_drawn_points_ = 0;
            DrawMaze();
        }
    }
}


void GraphicWidget::SetMazeMode(bool mode) {
    is_maze_ = mode;
}


std::pair<int, int> GraphicWidget::GetNumberOfPos(float x, float y) {
    int pos_i = 0, pos_j = 0;
    for (int j = 0; j < maze_size_.second; ++j) {
        if (x >= j * step_x_  - 250.0 &&
            x < (j + 1.0) * step_x_ - 250.0) {
            pos_j = j;  //  координата между границ ячеек
            break;
        }
    }
    for (int i = 0; i < maze_size_.first; ++i) {
        if (y >= i * step_y_  - 250.0 &&
            y < (i + 1.0) * step_y_ - 250.0) {
            pos_i = i;  //  координата между границ ячеек
            break;
        }
    }

    return std::pair<int, int>(pos_i, pos_j);
}

std::pair<float, float> GraphicWidget::GetMiddleCoordsOfCell(std::pair<int, int> pos) {
    std::pair<float, float> coords;
    coords.first = step_x_ * pos.second + step_x_ / 2.0 - 250.0; //  находим центр ячейки
    coords.second = step_y_ * pos.first + step_y_ / 2.0 - 250.0;

    return coords;
}



void GraphicWidget::DrawMazeWay() {
    std::vector<std::pair<int, int>> way = controller_maze_->GetExitWay();
    QPen way_pen(Qt::yellow);  //  Цвет и толщина линий
    way_pen.setWidth(2);

    std::pair<float, float> line_start, line_end;
    for (int i = 1; i < way.size(); ++i) {
        line_start = GetMiddleCoordsOfCell(way[i - 1]);
        line_end = GetMiddleCoordsOfCell(way[i]);
        scene_->addLine(line_start.first, line_start.second, line_end.first, line_end.second, way_pen);
    }
}


void GraphicWidget::Clear(QString message) {
    scene_->clear();
    items().clear();
    scene_->update();
    is_maze_ = false;
    cnt_drawn_points_ = 0;
    QGraphicsTextItem *textItem = new QGraphicsTextItem(message);
    textItem->setDefaultTextColor(Qt::white);
    textItem->setPos(scene_->sceneRect().center() - textItem->boundingRect().center());
    scene_->addItem(textItem);
}

}  // namespace s21
