#include "s21_model_maze.h"

#include <algorithm>
#include <fstream>

namespace s21 {

ModelMaze::ModelMaze() {
    ParametersToZero();
    width_ = 0;
    height_ = 0;   
}

void ModelMaze::Clear() {
    ParametersToZero();
    error_str_ = "Empty model";
    width_ = 0;
    height_ = 0;
    horizontal_walls_.clear();
    vertical_walls_.clear();
}

void ModelMaze::ParametersToZero() {
    error_ = 0;
    error_str_ = "";
    set_value_ = 0;
    start_pos_.first = 0;
    finish_pos_.first = 0;
    start_pos_.second = 0;
    finish_pos_.second = 0;
}

void ModelMaze::ParseFromFile(const std::string &model_path) {
    try {
        ParametersToZero();
        std::ifstream fin(model_path);
        if (!fin) throw std::runtime_error("Failed to open file: " + model_path);
        if (fin.peek() == fin.eof()) {
            InvalidFileThrow();
        } else {
            SetMatrixDimension(fin, height_);
            SetMatrixDimension(fin, width_);
            InitMatrix(fin, vertical_walls_);
            InitMatrix(fin, horizontal_walls_);
            fin.close();
        }
    } catch (const std::exception& e) {
        error_ = 1;
        error_str_ = e.what();
    }
}

void ModelMaze::SetMatrixDimension(std::ifstream &fin, int &d) {
    fin >> d;
    if (fin.fail() || d <= 0 || d > 50) InvalidFileThrow();
}

void ModelMaze::InitMatrix(std::ifstream &fin, MazeMatrix &matrix) {
    MatrixResize(matrix);
    int data = 0;

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            fin >> data;
            if ((data != 1 && data != 0) || (fin.peek() == '\n' && j != width_ - 1)) InvalidFileThrow();
            matrix[i][j] = data;
        }
        char newline;
        fin.get(newline);
        if (i != height_ - 1 && newline != '\n') InvalidFileThrow();
    }
}

void ModelMaze::MatrixResize(MazeMatrix &matrix) {
    for (auto &row : matrix) {
        row.assign(width_, false);
    }
    matrix.resize(height_, std::vector<bool>(width_, false));
}

void ModelMaze::InvalidFileThrow() {
    error_ = 1;
    throw std::runtime_error("File is invalid.");
}

void ModelMaze::SaveFile(const std::string &filename) {
    std::ofstream out(filename);
    if (out.is_open()) {
        out << height_ << " " << width_ << std::endl;
        WriteMatrixInFile(out, vertical_walls_);
        out << std::endl;
        WriteMatrixInFile(out, horizontal_walls_);
    }
}

void ModelMaze::SetNewStart(int i, int j) {
        start_pos_.first = i;
        start_pos_.second = j;
}

void ModelMaze::SetNewFinish(int i, int j) {
        finish_pos_.first = i;
        finish_pos_.second = j;
}

std::pair<int, int> ModelMaze::GetStartPos() {
    return start_pos_;
}

std::pair<int, int> ModelMaze::GetFinishPos() {
    return finish_pos_;
}

void ModelMaze::WriteMatrixInFile(std::ofstream &out, MazeMatrix &matrix) {
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
        out << static_cast<int>(matrix[i][j]);
        if (j != width_ - 1) out << " ";
    }
    out << std::endl;
  }
}

int ModelMaze::GetError() {
  return error_;
}

std::string ModelMaze::GetErrorMessage() {
  return error_str_;
}

int ModelMaze::GetHeight() { return height_; }
int ModelMaze::GetWidth() { return width_; }

const ModelMaze::MazeMatrix &ModelMaze::GetHorizontalWalls() {
    return horizontal_walls_;
}

const ModelMaze::MazeMatrix &ModelMaze::GetVerticalWalls(){
    return vertical_walls_;
}


//////////////////////////////////////  ГЕНЕРАЦИЯ ИДЕАЛЬНОГО ЛАБИРИНТА  /////////////////////////////////////////////

void ModelMaze::Generate() {
    try {
        error_ = 0;
        error_str_ = "";
        MatrixResize(vertical_walls_);
        MatrixResize(horizontal_walls_);
        row_.resize(width_, 0);
        int i = 0;
        for (; i < height_ - 1; ++i) {
            CreateCurrentRow();
            CreateVerticalWalls(i);
            CreateHorizontalWalls(i);
            PrepareNewRow(i);
        }
        CreateLastRow(i);
    } catch (const std::exception& e) {
        error_ = 1;
        error_str_ = e.what();
    }
}

void ModelMaze::CreateCurrentRow() {
    for (auto i = 0; i < width_; ++i) {
        if (row_[i] == 0) {
            row_[i] = ++set_value_;
        }
    }
}

void ModelMaze::CreateVerticalWalls(size_t i) {
    int j = 0;
    for (; j < width_ - 1; ++j) {
        bool rng = Rng();
        if (rng || row_[j] == row_[j + 1]) {
            vertical_walls_[i][j] = true;
        } else {
            MergeSets(row_[j], row_[j + 1]);
        }
    }
    vertical_walls_[i][j] = true;
}

bool ModelMaze::Rng() {
    return rand() > (RAND_MAX / 2);
}

//  исправил слияние от 0
void ModelMaze::MergeSets(int new_value, int old_value) {
    for (auto j = 0; j < width_; ++j) {
        if (row_[j] == old_value) row_[j] = new_value;
    }
}

void ModelMaze::CreateHorizontalWalls(size_t i) {
    for (auto j = 0; j < width_; ++j) {
       bool random = Rng();
       if (random) {
            horizontal_walls_[i][j] = true;
       }
    }
    //  теперь если в множестве только 1 ячейка, то её без стены
    //  если 2 и более, то 100% должна быть 1 стена и 1 проход
    std::vector<int> pos;
    pos.reserve(width_);
    for (auto j = 0; j < width_; ++j) {
        int cnt_h_walls = 0;
        int amount = 0;
        //  считаем сколько всего ячеек и сколько в них стен
        for (auto g = 0; g < width_; ++g) {
            if (row_[j] == row_[g]) {
                ++amount;
                if (horizontal_walls_[i][j] == true) {
                    ++cnt_h_walls;
                } else {
                    pos.push_back(g);
                }
            }
        }
        if (amount == 1 || cnt_h_walls == amount) {  //  если ячейка одна во множестве, то она обязательно пустая
            for (auto g = 0; g < width_; ++g) {
                if (row_[j] == row_[g]) {
                    horizontal_walls_[i][g] = false;
                    break;
                }
            }
        } else if (cnt_h_walls == 0) {  //  если не 1 ячейка и нет стен, нужно добавить
            int random_i = pos[0];
            horizontal_walls_[i][random_i] = true;
        }
        pos.clear();
    }
}

void ModelMaze::PrepareNewRow(size_t i) {
    for (auto j = 0; j < width_; ++j) {
        if (horizontal_walls_[i][j] == true)
            row_[j] = 0;
    }
}

void ModelMaze::CreateLastRow(size_t i) {
    CreateCurrentRow();
    CreateVerticalWalls(i);
    int j = 0;
    for (; j < width_ - 1; ++j) {
        if (row_[j] != row_[j + 1]) {
            vertical_walls_[i][j] = false;
            MergeSets(row_[j], row_[j + 1]);
        } else {
  //        vertical_walls_[i][j] = true;
        }
        horizontal_walls_[i][j] = true;
    }
    vertical_walls_[i][j] = true;
    horizontal_walls_[i][j] = true;
    set_value_ = 0;
    row_.clear();
}

void ModelMaze::SetSize(size_t height, size_t width) {
    height_ = height; width_ = width;
}


////////////////////////////      РЕШЕНИЕ ЛАБИРИНТА      //////////////////////////////////////////////


std::vector<std::pair<int, int>> ModelMaze::GetExitWay() {
    std::vector<std::vector<int>> field(height_, std::vector<int>(width_, 0));
    std::vector<std::pair<int, int>> way;
    s21::Direction direction = UP;
    int i = start_pos_.first, j = start_pos_.second;

    while (true) {
        if (i == finish_pos_.first && j == finish_pos_.second) {
            way.push_back(finish_pos_);
            break;
        }
       if (field[i][j] == 0) { //  отмечаем новое место
            way.push_back(std::pair<int, int>(i, j));   //  сохраняем позицию
            field[i][j] = 1;
       } else if (field[i][j] == 1) {       //  если уже тут были
            way.pop_back(); //  убираем прошлую ячейку из пути
            //  прошлую ячейку ставим как уже посещенную 2 раза, чтобы не вренуться на неё
            if (direction == UP) {
                field[i + 1][j] = 2;
            } else if (direction == DOWN) {
                field[i - 1][j] = 2;
            } else if (direction == LEFT) {
                field[i][j + 1] = 2;
            } else if (direction == RIGHT) {
                field[i][j - 1] = 2;
            }
        } else if (field[i][j] == 2) {
            break;  //  ERROR ! тут были 2 раз!"
        }   //  смотрим, куда идти дальше:
        if (FindNewDirection(i, j, field, direction, 0) == true) {
            GoNextStep(i, j, direction);    //  делаем новый шаг
        } else {
            break;  //  ERROR! не нашел новое направление!
        }
    }

    return way;
}


    //  Есть ли стена?
bool ModelMaze::IsWall(int i, int j, int direction) {
    if (direction == 1 || direction == 5) {   //  UP
        if (i == 0 || horizontal_walls_[i - 1][j] == true) {
            return true;
        } else {
            return false;
        }
    } else if (direction == 2 || direction == 6) {    //  RIGHT
        if (j == width_ - 1 || vertical_walls_[i][j] == true) {
            return true;
        } else {
            return false;
        }
    } else if (direction == 3 || direction == 7) {   //  DOWN
        if (i == height_ - 1 || horizontal_walls_[i][j] == true) {
            return true;
        } else {
            return false;
        }
    } else if (direction == 4 || direction == 0) {   //  LEFT
        if (j == 0 || vertical_walls_[i][j - 1] == true) {
            return true;
        } else {
            return false;
        }
    }
    return true;
}

    //  Делаем шаг в заданном направлении
void ModelMaze::GoNextStep(int &i, int &j, Direction& direction) {
    if (direction == UP) {
        --i;
        direction = UP;
    } else if (direction == DOWN) {
        ++i;
        direction = DOWN;
    } else if (direction == LEFT) {
        --j;
        direction = LEFT;
    } else if (direction == RIGHT) {
        ++j;
        direction = RIGHT;
    }
}

bool ModelMaze::FindNewDirection(int &i, int &j, std::vector<std::vector<int>>& field, Direction &direction, int condition) {
    bool new_way_found = false;

    if (direction == UP) {
       new_way_found = FindDirectionInCell(i, j, field, direction, condition, 1);
    } else if (direction == RIGHT) {
        new_way_found = FindDirectionInCell(i, j, field, direction, condition, 2);
    } else if (direction == DOWN) {
       new_way_found = FindDirectionInCell(i, j, field, direction, condition, 3);
    } else if (direction == LEFT) {
        new_way_found = FindDirectionInCell(i, j, field, direction, condition, 4);
    }
    // если новых тропинок не найдено, то идем обратно туда, где прошли всего лишь 1 раз, где были 2 раза - не идем
    if  (new_way_found == false && condition == 0) {
        new_way_found = FindNewDirection(i, j, field, direction, 1);
    }

    return new_way_found;
}

bool ModelMaze::FindDirectionInCell(int &i, int &j, std::vector<std::vector<int>> &field, Direction &direction, int condition, int dir_cnt) {
    if (dir_cnt < 1 || dir_cnt > 4) {
        return false;
    }

    if (IsWall(i, j, dir_cnt - 1) == false && CheckOtherCell(i, j, field, dir_cnt - 1, condition)) {
           direction = IntToDirection(dir_cnt - 1);
           return true;
    } else {        //  есть стена
        if (IsWall(i, j, dir_cnt) == false && CheckOtherCell(i, j, field, dir_cnt, condition)) {
                direction = IntToDirection(dir_cnt);
                return true;
        } else if (IsWall(i, j, dir_cnt + 1) == false && CheckOtherCell(i, j, field, dir_cnt + 1, condition)) {
                direction = IntToDirection(dir_cnt + 1);
                return true;
        } else if (IsWall(i, j, dir_cnt + 2) == false && CheckOtherCell(i, j, field, dir_cnt + 2, condition)) {
                direction = IntToDirection(dir_cnt + 2);
                return true;
        }
    }

    return false;
}


s21::Direction s21::ModelMaze::IntToDirection(int dir) {
    if (dir == 1 || dir == 5) {
        return s21::Direction::UP;
    } else if (dir == 2 || dir == 6) {
        return s21::Direction::RIGHT;
    } else if (dir == 3  || dir == 7) {
        return s21::Direction::DOWN;
    } else if (dir == 4 || dir == 0) {
        return s21::Direction::LEFT;
    }

    return s21::Direction::UP;
}

bool ModelMaze::CheckOtherCell(int i, int j, const std::vector<std::vector<int>>& field, int dir, int condition) {
    if (dir == 1 || dir == 5) {
        return field[i - 1][j] == condition;
    } else if (dir == 2 || dir == 6) {
        return field[i][j + 1] == condition;
    } else if (dir == 3  || dir == 7) {
        return field[i + 1][j] == condition;
    } else if (dir == 4 || dir == 0) {
        return field[i][j - 1] == condition;
    } else {
        return false;
    }
}

}  // namespace s21
