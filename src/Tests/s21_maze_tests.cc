#include "s21_tests.h"

TEST(Maze, CreationFromFile) {
  s21::ModelMaze m;
  m.ParseFromFile("Tests/noerror_maze");  // правильный
  ASSERT_TRUE(!m.GetError());
  m.ParseFromFile(
      "Tests/error_maze_less");  // размеры матрицы меньше фактической матрицы
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile(
      "Tests/error_maze_more");  // размеры матрицы больше фактической матрицы
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile("Tests/error_maze_with_letter");  // присутствуют буквы
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile("Tests/error_maze_empty");  // пустой файл
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile("Tests/error_maze_without_size");  // отсутствуют размеры
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile(
      "Tests/error_maze_negative_size");  // отрицательные значения размеров
  ASSERT_TRUE(m.GetError());
  m.ParseFromFile("Tests/error_maze_over_size");  // более 50х50
  ASSERT_TRUE(m.GetError());
}

TEST(Maze, ValidSetSize) {
  int width = 30;
  int height = 25;
  s21::ModelMaze m;
  m.SetSize(height, width);
  EXPECT_EQ(m.GetHeight(), height);
  EXPECT_EQ(m.GetWidth(), width);
  ASSERT_TRUE(!m.GetError());
}

TEST(Maze, Generate) {
  int width = 25;
  int height = 25;
  s21::ModelMaze m;
  m.SetSize(height, width);
  m.Generate();
  ASSERT_TRUE(!m.GetError());
  m.Clear();
}

TEST(Maze, DesicionOfMaze1) {
  s21::ModelMaze maze;

  maze.ParseFromFile("Tests/maze_20x20.txt");

  maze.SetNewStart(1, 1);
  maze.SetNewFinish(0, 0);

  EXPECT_EQ(maze.GetStartPos().first, 1);
  EXPECT_EQ(maze.GetStartPos().second, 1);
  EXPECT_EQ(maze.GetFinishPos().first, 0);
  EXPECT_EQ(maze.GetFinishPos().second, 0);

  std::vector<std::pair<int, int>> way = maze.GetExitWay();

  EXPECT_EQ(way[0].first, 1);
  EXPECT_EQ(way[0].second, 1);

  EXPECT_EQ(way[1].first, 1);
  EXPECT_EQ(way[1].second, 0);

  EXPECT_EQ(way[2].first, 0);
  EXPECT_EQ(way[2].second, 0);
}

TEST(Maze, DesicionOfMaze2) {
  s21::ModelMaze maze;

  maze.ParseFromFile("Tests/maze_20x20.txt");

  maze.SetNewStart(0, 6);
  maze.SetNewFinish(1, 7);

  EXPECT_EQ(maze.GetStartPos().first, 0);
  EXPECT_EQ(maze.GetStartPos().second, 6);
  EXPECT_EQ(maze.GetFinishPos().first, 1);
  EXPECT_EQ(maze.GetFinishPos().second, 7);

  std::vector<std::pair<int, int>> way = maze.GetExitWay();

  EXPECT_EQ(way[0].first, 0);
  EXPECT_EQ(way[0].second, 6);

  EXPECT_EQ(way[1].first, 1);
  EXPECT_EQ(way[1].second, 6);

  EXPECT_EQ(way[2].first, 1);
  EXPECT_EQ(way[2].second, 7);
}