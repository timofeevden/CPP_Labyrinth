#include "s21_controller_cave.h"

namespace s21 {

ControllerCave::ControllerCave(ModelCave* ptr) : model_(ptr) {}

ControllerCave::~ControllerCave() {}

int ControllerCave::OpenFile(const std::string& filepath) {
    model_->OpenFile(filepath);
    return model_->GetError();
}

int ControllerCave::GetError() {
    return model_->GetError();
}

std::pair<int, int> ControllerCave::GetSize() {
    return std::pair<int, int>(model_->GetSizeN(), model_->GetSizeM());
}

void ControllerCave::SetParametrs(int new_birth_limit, int new_death_limit) {
    model_->SetParametrs(new_birth_limit, new_death_limit);
}

int ControllerCave::DoNextStep() {
    return model_->DoNextStep();
}

void ControllerCave::GenerateCave(int n, int m, int chance) {
    model_->GenerateCave(n, m, chance);
}

void ControllerCave::SaveToFile(const std::string &file_path) {
    model_->SaveToFile(file_path);
}

const std::vector<std::vector<bool>>& ControllerCave::GetData() {
    return model_->GetData();
}

}  // namespace s21
