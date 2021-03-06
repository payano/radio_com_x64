/*
 * MessagePkg.h
 *
 *  Created on: Feb 2, 2018
 *      Author: johan
 */
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "CommonPkg.h"

#pragma once

namespace MessagePkg {

template <typename T>
class Queue
{
 public:
	Queue() = default;

  T pop()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }

  void pop(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }

  void push(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }

  void push(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    mlock.unlock();
    cond_.notify_one();
  }

  bool isEmpty(){return queue_.size() == 0 ? true:false;}
  size_t size(){return queue_.size();}

 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

struct Message {
	common::Method method;
	std::string base;
	std::string topic;
	std::string value;

};
} /* namespace MessagePkg */

