#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class UnboundedBuffer {
private:
    std::queue<std::string> buffer;
    mutable std::mutex mtx;
    std::condition_variable not_empty;

public:
    UnboundedBuffer() {}

    void insert(const std::string &item) {
        std::unique_lock<std::mutex> lock(mtx);
        buffer.push(item);
        not_empty.notify_one();
    }

    std::string remove() {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [this]() { return !buffer.empty(); });
        std::string item = buffer.front();
        buffer.pop();
        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return buffer.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return buffer.size();
    }
};