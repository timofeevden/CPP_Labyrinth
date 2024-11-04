#include "s21_model_cave.h"

#include <fstream>

namespace s21 {

ModelCave::ModelCave() {
    size_n_ = 0;
    size_m_ = 0;
    error_ = 0;
    birth_limit_ = 0;
    death_limit_ = 0;
    birth_chance_ = 0;
    srand(time(0));
}

ModelCave::~ModelCave() {}

int ModelCave::OpenFile(const std::string& filepath) {
    Clear();
    std::ifstream cave_file(filepath);

    if (!cave_file.is_open()) {
        error_ = 1;
    } else {
        while(!cave_file.eof() && !error_) {
            cave_file >> size_n_;
            cave_file >> size_m_;

            if (size_n_ > 0 && size_n_ <= 50 && size_m_ > 0 && size_m_ <= 50) {
                data_.resize(size_n_, std::vector<bool>(size_m_));

                for (int i = 0, tmp = 0; i < size_n_ && !error_ && !cave_file.eof(); ++i) {
                    for (int j = 0; j < size_m_ && !cave_file.eof(); ++j) {
                        cave_file >> tmp;
                        if (tmp != 1 && tmp != 0) {
                            error_ = 3;
                            break;
                        }
                        tmp == 1 ? data_[i][j] = true : data_[i][j] = false;
                    }
                }
                break;
            } else {
                error_ = 2;
            }
        }
        cave_file.close();
    }

    return error_;
}


int ModelCave::GetError() {
    return error_;
}


int ModelCave::GetSizeN() {
    return size_n_;
}


int ModelCave::GetSizeM() {
    return size_m_;
}


const std::vector<std::vector<bool>>& ModelCave::GetData() {
    return data_;
}


void ModelCave::SetParametrs(int new_birth_limit, int new_death_limit) {
    birth_limit_ = new_birth_limit;
    death_limit_ = new_death_limit;
}


int ModelCave::DoNextStep() {
    std::vector<std::vector<bool>> new_data = data_;
    int cnt_changed_cells = 0;

    for (int i = 0; i < size_n_; ++i) {
        for (int j = 0; j < size_m_; ++j) {
            int alive_neighbours = CheckAliveNeighbours(i, j);

            if (data_[i][j] == 1 && alive_neighbours < death_limit_) {
                new_data[i][j] = 0;
                ++cnt_changed_cells;
            } else if (data_[i][j] == 0 && alive_neighbours > birth_limit_) {
                new_data[i][j] = 1;
                ++cnt_changed_cells;
            }
        }
    }
    data_ = std::move(new_data);

    return cnt_changed_cells;
}


void ModelCave::GenerateCave(int n, int m, int chance) {
    data_.clear();
    data_.resize(n);
    birth_chance_ = chance;
    size_n_ = n;
    size_m_ = m;

    for (int i = 0; i < size_n_; ++i) {
        data_[i].resize(size_m_);
        for (int j = 0; j < size_m_; ++j) {
            data_[i][j] = GenerateCell();
        }
    }
}

bool ModelCave::GenerateCell() {
    int random = rand() % 101;
    if (random <= birth_chance_) return 1;
    return 0;
}

void ModelCave::SaveToFile(const std::string &file_path) {
    std::ofstream out_file(file_path);

    if (out_file.is_open()) {
        out_file << size_n_ << " " << size_m_ << "\n";

        for (int i = 0; i < size_n_; ++i) {
          for (int j = 0; j < size_m_; ++j) {
              out_file << static_cast<int>(data_[i][j]);
              if (j != size_m_ - 1) {
                  out_file << " ";
              }
          }
          out_file << "\n";
        }
  }
}

int ModelCave::CheckAliveNeighbours(int i, int j) {
    int cnt = 0;

    for (int pos_i = i - 1; pos_i <= i + 1; ++pos_i) {
        for (int pos_j = j - 1; pos_j <= j + 1; ++pos_j) {
            if (pos_i < 0 || pos_i >= size_n_ || pos_j < 0 || pos_j >= size_m_) {
                ++cnt;
            } else if (pos_i == i && pos_j == j) {
                continue;
            } else if (data_[pos_i][pos_j]) {
                ++cnt;
            }
        }
    }

    return cnt;
}

void ModelCave::Clear() {
    data_.clear();
    size_n_ = 0;
    size_m_ = 0;
    error_ = 0;
}

}  // namespace s21
