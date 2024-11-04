
#include "s21_controller_maze.h"


namespace s21 {

ControllerMaze::ControllerMaze(ModelMaze* ptr) : model_(ptr) {}

ControllerMaze::~ControllerMaze() {}

std::pair<int, int> ControllerMaze::OpenFile(const std::string &model_path) {
    model_->ParseFromFile(model_path);
    return std::make_pair(model_->GetHeight(), model_->GetWidth());
}

void ControllerMaze::SaveFile(const std::string &filename) {
    model_->SaveFile(filename);
}

int ControllerMaze::GetError() {
    return model_->GetError();
}

std::string ControllerMaze::GetErrorMessage() {
    return model_->GetErrorMessage();
}

std::pair<int, int> ControllerMaze::GetSize() {
    return std::pair<int, int>(model_->GetHeight(), model_->GetWidth());
}

void ControllerMaze::SetSize(int height, int width) {
    model_->SetSize(height, width);
}

const std::vector<std::vector<bool> > &ControllerMaze::GetVerticalData() {
    return model_->GetVerticalWalls();
}

const std::vector<std::vector<bool> > &ControllerMaze::GetHorisontalData() {
    return model_->GetHorizontalWalls();
}

void ControllerMaze::SetNewStart(int i, int j) {
    model_->SetNewStart(i, j);
}

void ControllerMaze::SetNewFinish(int i, int j) {
    model_->SetNewFinish(i, j);
}

std::pair<int, int> ControllerMaze::GetStartPos() {
    return model_->GetStartPos();
}

std::pair<int, int> ControllerMaze::GetFinishPos() {
    return model_->GetFinishPos();
}

void ControllerMaze::Generate() {
    model_->Generate();
}

void ControllerMaze::Clear() {
    model_->Clear();
}

std::vector<std::pair<int, int> > ControllerMaze::GetExitWay() {
    return model_->GetExitWay();
}

}  // namespace s21
