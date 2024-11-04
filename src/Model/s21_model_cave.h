#ifndef MODEL_S21_MODEL_CAVE_H
#define MODEL_S21_MODEL_CAVE_H

#include <string>
#include <vector>

namespace s21 {

class ModelCave {
public:
    explicit ModelCave();
    ~ModelCave();

    const std::vector<std::vector<bool>>& GetData();
    int OpenFile(const std::string& filepath);
    int GetError();
    int GetSizeN();
    int GetSizeM();
    int DoNextStep();
    void SetParametrs(int new_birth_limit, int new_death_limit);
    void GenerateCave(int n, int m, int chance);
    void SaveToFile(const std::string& file_path);
    void PrintData();

private:
    int CheckAliveNeighbours(int i, int j);
    bool GenerateCell();
    void Clear();

    std::vector<std::vector<bool>> data_;
    int error_;
    int size_n_;
    int size_m_;
    int birth_limit_;
    int death_limit_;
    int birth_chance_;
};

}  // namespace s21

#endif // MODEL_S21_MODEL_CAVE_H
