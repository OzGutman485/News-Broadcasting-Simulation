#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class BoundedBuffer {
private:
    std::queue<std::string> buffer;
    int cap;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
    int count;
public:
    int get_count();
    explicit BoundedBuffer(int size);
    void insert(const std::string& item);
    std::string remove();
};

#endif // BOUNDED_BUFFER_H
