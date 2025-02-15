#include "TaskManager.hpp"

#include "Parser.hpp"
#include "Reader.hpp"
#include <format>

void TaskManager::logTask(const Task &task, const std::string &message,
                          const std::chrono::microseconds &time) {
  logger.logTime(std::format("Task {} {}", task.id, message), time);
}

void TaskManager::readFileStage() {
  while (running || taskCounter) {
    Task task;
    if (stage1Queue.try_pop(task)) {
      auto duration = task.nextStage();
      logTask(task, "started step 1", duration);

      task.data = Reader::read(task.filename);

      duration = task.nextStage();
      stage2Queue.push(task);
      logTask(task, "finished step 1", duration);
    }
  }
}

void TaskManager::processDataStage() {
  while (running || taskCounter) {
    Task task;
    if (stage2Queue.try_pop(task)) {
      auto duration = task.nextStage();
      logTask(task, "started step 2", duration);

      Parser p(task.data);
      task.title = p.getTitle();
      task.image = p.getImage();
      task.ingredients = p.getIngredients();
      task.steps = p.getSteps();

      duration = task.nextStage();
      stage3Queue.push(task);
      logTask(task, "finished step 2", duration);
    }
  }
}

void TaskManager::writeToDBStage() {
  while (running || taskCounter) {
    Task task;
    if (stage3Queue.try_pop(task)) {
      auto duration = task.nextStage();
      logTask(task, "started step 3", duration);

      db.insertRecipe(task.id, task.title, task.ingredients, task.steps,
                      task.image);

      duration = task.nextStage();
      finished.push_back(task);
      taskCounter--;
      logTask(task, "finished step 3", duration);
    }
  }
}

void TaskManager::run() {
  running = true;
  std::thread reader(&TaskManager::readFileStage, this);
  std::thread processor(&TaskManager::processDataStage, this);
  std::thread writer(&TaskManager::writeToDBStage, this);

  reader.join();
  processor.join();
  writer.join();
}

void TaskManager::stop() { running = false; }

TaskManager::TaskManager() : db("./DB.SQLite") {}

void TaskManager::push(Task t) {
  stage1Queue.push(t);
  taskCounter++;
}
