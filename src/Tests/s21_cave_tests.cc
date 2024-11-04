#include "s21_tests.h"

TEST(Cave, OpenFile) {
  s21::ModelCave model;

  model.OpenFile("nofile");
  EXPECT_TRUE(model.GetError());

  model.OpenFile("Tests/cave_error.txt");
  EXPECT_TRUE(model.GetError());

  model.OpenFile("Tests/cave_error2.txt");
  EXPECT_TRUE(model.GetError());

  model.OpenFile("Tests/cave_10x10.txt");

  if (model.GetError() == 0) {
    EXPECT_EQ(model.GetError(), 0);
    EXPECT_EQ(model.GetSizeN(), 10);
    EXPECT_EQ(model.GetSizeM(), 10);

    std::vector<std::vector<bool>> data = model.GetData();

    std::vector<bool> true_result = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0};
    EXPECT_TRUE((data[0] == true_result));

    true_result = {0, 0, 1, 1, 0, 0, 0, 0, 0, 1};
    EXPECT_TRUE(data[1] == true_result);

    true_result = {0, 0, 1, 0, 1, 0, 1, 1, 0, 1};
    EXPECT_TRUE((data[2] == true_result));

    true_result = {0, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    EXPECT_TRUE((data[3] == true_result));

    true_result = {0, 0, 0, 1, 1, 0, 0, 1, 1, 1};
    EXPECT_TRUE((data[4] == true_result));

    true_result = {0, 1, 0, 1, 0, 1, 0, 0, 0, 0};
    EXPECT_TRUE((data[5] == true_result));

    true_result = {1, 1, 0, 0, 0, 0, 0, 1, 0, 0};
    EXPECT_TRUE((data[6] == true_result));

    true_result = {0, 0, 0, 0, 0, 0, 1, 0, 1, 1};
    EXPECT_TRUE((data[7] == true_result));

    true_result = {1, 0, 0, 0, 0, 1, 1, 0, 0, 0};
    EXPECT_TRUE(data[8] == true_result);

    true_result = {0, 1, 1, 0, 0, 1, 1, 0, 0, 0};
    EXPECT_TRUE(data[9] == true_result);
  }
}

TEST(Cave, GenerateFromFile) {
  s21::ModelCave model;
  model.OpenFile("Tests/cave_10x10.txt");

  if (model.GetError() == 0) {
    EXPECT_EQ(model.GetError(), 0);
    EXPECT_EQ(model.GetSizeN(), 10);
    EXPECT_EQ(model.GetSizeM(), 10);
    model.SetParametrs(4, 3);

    while (model.DoNextStep()) {
    }

    std::vector<std::vector<bool>> data = model.GetData();
    std::vector<bool> true_result = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_TRUE(data[0] == true_result);
    EXPECT_TRUE(data[1] == true_result);
    EXPECT_TRUE((data[2] == true_result));
    EXPECT_TRUE((data[3] == true_result));

    true_result = {1, 1, 1, 1, 1, 1, 0, 0, 1, 1};
    EXPECT_TRUE((data[4] == true_result));

    true_result = {1, 1, 0, 0, 0, 0, 0, 0, 0, 1};
    EXPECT_TRUE((data[5] == true_result));

    true_result = {1, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    EXPECT_TRUE((data[6] == true_result));

    true_result = {1, 0, 0, 0, 0, 0, 0, 0, 1, 1};
    EXPECT_TRUE((data[7] == true_result));

    true_result = {1, 1, 0, 0, 0, 1, 1, 1, 1, 1};
    EXPECT_TRUE(data[8] == true_result);

    true_result = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_TRUE(data[9] == true_result);
  }
}

TEST(Cave, GenerateRandom) {
  s21::ModelCave model;
  model.GenerateCave(10, 10, 10);
  EXPECT_EQ(model.GetError(), 0);
  model.SetParametrs(6, 7);

  while (model.DoNextStep()) {
  }

  std::vector<std::vector<bool>> data = model.GetData();

  std::vector<bool> true_result = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  EXPECT_TRUE((data[0] == true_result));
  EXPECT_TRUE(data[1] == true_result);
  EXPECT_TRUE((data[2] == true_result));
  EXPECT_TRUE((data[3] == true_result));
  EXPECT_TRUE((data[4] == true_result));
  EXPECT_TRUE((data[5] == true_result));
  EXPECT_TRUE((data[6] == true_result));
  EXPECT_TRUE((data[7] == true_result));
  EXPECT_TRUE(data[8] == true_result);
  EXPECT_TRUE(data[9] == true_result);
}
