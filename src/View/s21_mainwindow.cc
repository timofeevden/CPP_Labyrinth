#include "s21_mainwindow.h"
#include "forms/ui_s21_mainwindow.h"

#include <QDateTime>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QTimer>

#include "s21_graphic_widget.h"

namespace s21 {

MainWindow::MainWindow(ControllerCave* cntr_cave_, ControllerMaze* cntr_maze_, QWidget *parent)
    : QMainWindow(parent), controller_cave_(cntr_cave_), controller_maze_(cntr_maze_),  ui_(new Ui::MainWindow), auto_mode_(false)
{
    this->setWindowTitle("Maze");
    ui_->setupUi(this);
    setFixedSize(745, 512);
    //  создаем и добавляем виджет
    graphic_widget_ = new s21::GraphicWidget(this, controller_cave_, controller_maze_);
    ui_->widgetLayout->addWidget(graphic_widget_);
    //  создаем и связываем таймер для клеточного автомата
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::AutoNextStep);
    //  связываем позунки и кнопки
    connect(ui_->maze_width_slider, &QSlider::valueChanged, this, &MainWindow::onMazeWidthSliderValueChanged);
    connect(ui_->maze_height_slider, &QSlider::valueChanged, this, &MainWindow::onMazeHeightSliderValueChanged);
    connect(ui_->cave_width_slider, &QSlider::valueChanged, this, &MainWindow::onCaveWidthSliderValueChanged);
    connect(ui_->cave_height_slider, &QSlider::valueChanged, this, &MainWindow::onCaveHeightSliderValueChanged);
    connect(ui_->birth_slider, &QSlider::valueChanged, this, &MainWindow::onBirthSliderValueChanged);
    connect(ui_->death_slider, &QSlider::valueChanged, this, &MainWindow::onDeathSliderValueChanged);
    connect(ui_->auto_cave_stop_pushButton, SIGNAL(clicked()), this, SLOT(StopAutoMode()));
    connect(ui_->load_file_maze_button, SIGNAL(clicked()), this, SLOT(LoadFile()));
    connect(ui_->load_file_cave_button, SIGNAL(clicked()), this, SLOT(LoadFile()));
    connect(ui_->save_file_maze_button, SIGNAL(clicked()), this, SLOT(SaveFile()));
    connect(ui_->save_file_cave_button, SIGNAL(clicked()), this, SLOT(SaveFile()));
}


MainWindow::~MainWindow() {
    delete ui_;
}

////////////////////////////////////////  Загрузка и сохранение файла //////////////////////////////////////////////

void MainWindow::LoadFile() {
    QString pathfile = QFileDialog::getOpenFileName(this, "Load a file", QDir::currentPath());
    if (!pathfile.isEmpty()) {
        path_ = pathfile.toStdString();
        is_scene_not_empty_ = true;
        if (ui_->tabWidget->currentIndex() == 0) {
            LoadMaze();
        } else {
            LoadCave();
        }
    }
}

void MainWindow::LoadMaze() {
    controller_maze_->OpenFile(path_);
    if (controller_maze_->GetError()) {
        is_scene_not_empty_ = false;
        graphic_widget_->Clear(QString::fromStdString(controller_maze_->GetErrorMessage()));
    } else {
        is_scene_not_empty_ = true;
        ui_->maze_width_slider->setValue(controller_maze_->GetSize().second);
        ui_->maze_height_slider->setValue(controller_maze_->GetSize().first);
        graphic_widget_->SetMazeMode(true);
        graphic_widget_->DrawMaze();
    }
}

void MainWindow::LoadCave() {
    is_scene_not_empty_ = true;
    controller_cave_->OpenFile(path_);
    ui_->cave_width_slider->setValue(controller_cave_->GetSize().second);
    ui_->cave_height_slider->setValue(controller_cave_->GetSize().first);
    graphic_widget_->SetMazeMode(false);
    graphic_widget_->DrawCave();
}

void MainWindow::SaveFile() {
    if (is_scene_not_empty_ == false) {
        QMessageBox::warning(ui_->centralwidget, "Failury saving!", "Scene is empty!");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save to file", "./");
    if (!filename.isEmpty()) {
        if (ui_->tabWidget->currentIndex() == 0) {
            controller_maze_->SaveFile(filename.toStdString());
        } else {
            controller_cave_->SaveToFile(filename.toStdString());
        }
        QMessageBox::information(ui_->centralwidget, "Success!", "File saved successfully");
    } else {
        QMessageBox::warning(ui_->centralwidget, "Failury saving!", "File was not saved");
    }
}

void MainWindow::on_reset_pushButton_clicked() {
    is_scene_not_empty_ = false;
    graphic_widget_->SetMazeMode(false);
    controller_maze_->Clear();
    graphic_widget_->Clear("Up to another round!");
}

////////////////////////////////////////  Ползунки //////////////////////////////////////////////

void MainWindow::onCaveWidthSliderValueChanged(int value) {
    ui_->cave_width_value_label->setText(QString::number(value));
}

void MainWindow::onMazeWidthSliderValueChanged(int value) {
    ui_->maze_width_value_label->setText(QString::number(value));
}

void MainWindow::onMazeHeightSliderValueChanged(int value) {
    ui_->maze_height_value_label->setText(QString::number(value));
}

void MainWindow::onCaveHeightSliderValueChanged(int value) {
    ui_->cave_height_value_label->setText(QString::number(value));
}

void MainWindow::onBirthSliderValueChanged(int value) {
    ui_->birth_value_label->setText(QString::number(value));
}

void MainWindow::onDeathSliderValueChanged(int value) {
    ui_->death_value_label->setText(QString::number(value));
}

//  Переключатель между Лабиринтом и Пещерами
void MainWindow::on_tabWidget_currentChanged(int tab_index) {
    if (tab_index == 0) {
        graphic_widget_->DrawMaze();
        ui_->maze_height_slider->setValue(controller_maze_->GetSize().first);
        ui_->maze_width_slider->setValue(controller_maze_->GetSize().second);
    } else {
        graphic_widget_->DrawCave();
        ui_->cave_height_slider->setValue(controller_cave_->GetSize().first);
        ui_->cave_width_slider->setValue(controller_cave_->GetSize().second);
    }
}

////////////////////////////////////////    Генерация пещер и запуск клеточного автомата //////////////////////////////////////////////


void MainWindow::on_generate_cave_pushButton_clicked() {
    controller_cave_->GenerateCave(ui_->cave_height_slider->value(), ui_->cave_width_slider->value(), ui_->chance_value_spinBox->value());
    graphic_widget_->DrawCave();
}

void MainWindow::on_next_step_pushButton_clicked() {
    StopAutoMode();
    controller_cave_->SetParametrs(ui_->birth_slider->value(), ui_->death_slider->value());

    if (controller_cave_->DoNextStep()) {
        graphic_widget_->DrawCave();
    } else {
        QMessageBox::information(ui_->centralwidget, "Last step was completed!", "It was the last step, there is no more changes");
    }
}

void MainWindow::on_auto_cave_start_pushButton_clicked() {
    ui_->generate_cave_pushButton->setEnabled(false);
    ui_->load_file_cave_button->setEnabled(false);
    ui_->save_file_cave_button->setEnabled(false);
    ui_->auto_cave_start_pushButton->setEnabled(false);
    ui_->tabWidget->setTabEnabled(0, false);
    timer_->start(ui_->delay_value_spinBox->value());
}

void MainWindow::StopAutoMode() {
    timer_->stop();
    ui_->generate_cave_pushButton->setEnabled(true);
    ui_->load_file_cave_button->setEnabled(true);
    ui_->save_file_cave_button->setEnabled(true);
    ui_->auto_cave_start_pushButton->setEnabled(true);
    ui_->tabWidget->setTabEnabled(0, true);
}

void MainWindow::AutoNextStep() {
    controller_cave_->SetParametrs(ui_->birth_slider->value(), ui_->death_slider->value());

    if (controller_cave_->DoNextStep()) {
        graphic_widget_->DrawCave();
    } else {
        StopAutoMode();
        QMessageBox::information(ui_->centralwidget, "Auto-mode was completed!", "It was the last step, there is no more changes");
    }
}

/////////////////////////////// ГЕНЕРАЦИЯ ЛАБИРИНТА //////////////////////

void MainWindow::on_generate_maze_pushButton_clicked() {
    controller_maze_->SetSize(ui_->maze_width_slider->value(), ui_->maze_height_slider->value());
    controller_maze_->Generate();
    if (controller_maze_->GetError()) {
         is_scene_not_empty_ = false;
        graphic_widget_->Clear(QString::fromStdString(controller_maze_->GetErrorMessage()));
    } else {
        is_scene_not_empty_ = true;
        graphic_widget_->SetMazeMode(true);
        graphic_widget_->DrawMaze();
    }
}

}  // namespace s21
