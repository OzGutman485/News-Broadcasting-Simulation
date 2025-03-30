#include "BoundedBuffer.h"
#include "random"
#include <iostream>

using namespace std;

class Producer {
private:
    int id;
    int num_products;
    BoundedBuffer &queue;
    int count_news;
    int count_weather;
    int count_sport;
public:
    Producer(int id, int num_products, BoundedBuffer &queue)
            : id(id), num_products(num_products), queue(queue) {
        count_news = -1;
        count_sport = -1;
        count_weather = -1;
    }

    void produce() {
        random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 3);
        int chose_count;
        for (int i = 0; i < num_products; ++i) {
            int random_number = dis(gen);
            if (random_number % 3 == 0) {
                count_sport += 1;
            } else if (random_number % 3 == 1) {
                count_news += 1;
            } else {
                count_weather += 1;
            }
            chose_count = (random_number % 3 == 0) ? count_sport : (random_number % 3 == 1) ? count_news
                                                                                            : count_weather;
            std::string type = (random_number % 3 == 0) ? "SPORTS" : (random_number % 3 == 1) ? "NEWS" : "WEATHER";
            std::string message = "Producer " + std::to_string(id) + " " + type + " " + std::to_string(chose_count);
            queue.insert(message);
        }
        string message = "DONE";
        queue.insert(message);
    }
};

