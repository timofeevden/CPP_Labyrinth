#ifndef MODEL_S21_MODEL_MAZE_H
#define MODEL_S21_MODEL_MAZE_H

#include <fstream>
#include <string>
#include <vector>

namespace s21 {

enum Direction {
    UP = 1, RIGHT, DOWN, LEFT
};  //  направления по часовой стрелки

class ModelMaze {
public:
    using MazeMatrix = std::vector<std::vector<bool>>;

    ModelMaze();
    ~ModelMaze() = default;

    void ParseFromFile(const std::string &model_path);
    int GetError();
    std::string GetErrorMessage();
    int GetHeight();
    int GetWidth();
    const MazeMatrix& GetHorizontalWalls();
    const MazeMatrix& GetVerticalWalls();
    void SaveFile(const std::string &filename);
    void Generate();
    void Clear();
    void SetSize(size_t height, size_t width);
    void SetNewStart(int i, int j);
    void SetNewFinish(int i, int j);
    std::pair<int, int> GetStartPos();
    std::pair<int, int> GetFinishPos();
    std::vector<std::pair<int, int>> GetExitWay();

private:
    void InitMatrix(std::ifstream &fin, MazeMatrix &matrix);
    void ParametersToZero();
    void SetMatrixDimension(std::ifstream &fin, int &d);
    void WriteMatrixInFile(std::ofstream &out, MazeMatrix &matrix);
    void InvalidFileThrow();
    bool IsWall(int i, int j, int direction);
    void GoNextStep(int& i, int& j, s21::Direction& direction);
    bool FindNewDirection(int& i, int& j, std::vector<std::vector<int>>& field, s21::Direction& direction, int condition);
    bool FindDirectionInCell(int& i, int& j, std::vector<std::vector<int>>& field, s21::Direction& direction, int condition, int dir_cnt);
    bool CheckOtherCell(int i, int j, const std::vector<std::vector<int>>& field, int dir, int condition);
    bool Rng();
    s21::Direction IntToDirection(int dir);
    void MatrixResize(MazeMatrix &matrix);
    void SetMatrixDimension(std::ifstream &fin, size_t &d);
    void CreateCurrentRow();
    void CreateVerticalWalls(size_t i);
    void MergeSets(int new_value, int old_value);
    void CreateHorizontalWalls(size_t i);
    void CheckedHorizontalWalls(size_t i);
    void PrepareNewRow(size_t i);
    void CreateLastRow(size_t i);
    void MatrixClear(MazeMatrix &matrix);

    int width_;
    int height_;
    MazeMatrix horizontal_walls_;
    MazeMatrix vertical_walls_;
    int error_;
    std::string error_str_;
    std::pair<int, int> start_pos_;
    std::pair<int, int> finish_pos_;
    size_t set_value_;
    std::vector<int> row_;
    std::string path_;
};

}  // namespace s21

#endif // MODEL_S21_MODEL_MAZE_H
