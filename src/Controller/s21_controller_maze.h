#ifndef CONTROLLER_S21_CONTROLLER_MAZE_H
#define CONTROLLER_S21_CONTROLLER_MAZE_H

#include "../Model/s21_model_maze.h"

namespace s21 {

class ControllerMaze {
public:
    explicit ControllerMaze(ModelMaze* ptr);
    ~ControllerMaze();

    std::pair<int, int> OpenFile(const std::string &model_path);
    void SaveFile(const std::string &filename);
    int GetError();
    std::string GetErrorMessage();
    std::pair<int, int> GetSize();
    void SetSize(int height, int width);
    const std::vector<std::vector<bool>>& GetVerticalData();
    const std::vector<std::vector<bool>>& GetHorisontalData();
    void Generate();
    void Clear();
    void SetNewStart(int i, int j);
    void SetNewFinish(int i, int j);
    std::pair<int, int> GetStartPos();
    std::pair<int, int> GetFinishPos();
    std::vector<std::pair<int, int>> GetExitWay();

private:
    ModelMaze* model_;
};

}  // namespace s21

#endif // CONTROLLER_S21_CONTROLLER_MAZE_H
