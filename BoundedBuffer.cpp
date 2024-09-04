#include "BoundedBuffer.h"
using namespace std;
#include <mutex>
#include <string>


BoundedBuffer::BoundedBuffer(int size) : cap(size) {
    this->count = 0;
}

void BoundedBuffer::insert(const std::string &item) {
    std::unique_lock<std::mutex> lock(mtx);
    not_full.wait(lock, [this]() { return (int) buffer.size() < cap; });
    buffer.push(item);
    this->count += 1;
    not_empty.notify_one();
}

std::string BoundedBuffer::remove() {
    std::unique_lock<std::mutex> lock(mtx);
    not_empty.wait(lock, [this]() { return (int) buffer.size() > 0 ;});
    std::string item = buffer.front();
    buffer.pop();
    this->count -= 1;
    not_full.notify_one();
    return item;
}

int BoundedBuffer::get_count() {
    std::lock_guard<std::mutex> lock(mtx);
    return (int) this->buffer.size();
}
