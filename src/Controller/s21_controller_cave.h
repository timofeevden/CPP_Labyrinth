#ifndef CONTROLLER_S21_CONTROLLER_CAVE_H
#define CONTROLLER_S21_CONTROLLER_CAVE_H

#include "../Model/s21_model_cave.h"

namespace s21 {

class ControllerCave {
public:
    explicit ControllerCave(ModelCave* ptr);
    ~ControllerCave();

    int OpenFile(const std::string& filepath);
    int GetError();
    const std::vector<std::vector<bool>>& GetData();
    std::pair<int, int> GetSize();
    void SetParametrs(int new_birth_limit, int new_death_limit);
    int DoNextStep();
    void GenerateCave(int n, int m, int chance);
    void SaveToFile(const std::string& file_path);

private:
    ModelCave* model_;
};

}  // namespace s21

#endif // CONTROLLER_S21_CONTROLLER_CAVE_H
