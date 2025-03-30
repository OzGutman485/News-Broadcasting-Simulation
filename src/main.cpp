#include "BoundedBuffer.h"
#include "Producer.cpp"
#include "Unbounded_Buffer.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <atomic>

using namespace std;

void dispatch(vector<BoundedBuffer *> producerQueues, BoundedBuffer *sportsQueue, BoundedBuffer *newsQueue,
              BoundedBuffer *weatherQueue, int numProducers) {
    int doneCount = 0;
    vector<bool> producerDone(numProducers, false);
    while (doneCount < numProducers) {
        bool itemProcessed = false;
        for (int i = 0; i < numProducers; ++i) {
            if (!producerDone[i] && producerQueues[i]->get_count() > 0) {
                std::string item = producerQueues[i]->remove();
                if (item == "DONE") {
                    producerDone[i] = true;
                    doneCount++;
                } else if (item.find("SPORTS") != std::string::npos) {
                    sportsQueue->insert(item);
                } else if (item.find("NEWS") != std::string::npos) {
                    newsQueue->insert(item);
                } else if (item.find("WEATHER") != std::string::npos) {
                    weatherQueue->insert(item);
                }
                itemProcessed = true;
            }
        }
        if (!itemProcessed) {
            std::this_thread::yield();
        }
    }
    sportsQueue->insert("DONE");
    newsQueue->insert("DONE");
    weatherQueue->insert("DONE");
}

void co_analyze(BoundedBuffer *analyze, UnboundedBuffer *insert) {
    while (true) {
        string st = analyze->remove();
        if (st == "DONE") {
            insert->insert(st);
            break;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            insert->insert(st);
        }
    }
}

void print(UnboundedBuffer *co, std::atomic<int> *save) {
    while (true) {
        string st = co->remove();
        if (st == "DONE") {
            if (++(*save) == 3) {
                cout << st << endl;
                break;
            }
        } else {
            cout << st << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    // Check for correct usage
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    // Open the configuration file
    std::ifstream config_file(argv[1]);
    if (!config_file) {
        std::cerr << "Unable to open configuration file." << std::endl;
        return 1;
    }
    vector <Producer> producers;
    vector < BoundedBuffer * > producer_queues;
    vector <std::thread> producer_threads;
    int num_producers = 0;
    int coeditor_queue_size = 0;
    string line;
    while (std::getline(config_file, line)) {
        std::istringstream iss(line);
        std::string token;
        if (line.find("PRODUCER") != std::string::npos) {
            int id, numProduct, size = 0;
            iss >> token >> id;
            std::getline(config_file, line); // read next line for number of products
            std::istringstream iss2(line);
            iss2 >> numProduct;
            std::getline(config_file, line); // read next line for queue size
            std::istringstream iss3(line);
            std::string sizeToken;
            iss3 >> sizeToken >> sizeToken >> sizeToken >> size; // "queue size = [size]"
            BoundedBuffer *queue = new BoundedBuffer(size);
            producer_queues.push_back(queue);
            producers.emplace_back(id, numProduct, *queue);
            num_producers += 1;
        } else if (line.find("Co-Editor queue size") != std::string::npos) {
            std::istringstream iss2(line);
            std::string dummy;
            iss2 >> dummy >> dummy >> dummy >> dummy >> coeditor_queue_size;
        }
    }
    // Execute the producers
    for (int i = 0; i < num_producers; ++i) {
        producer_threads.emplace_back(&Producer::produce, &producers[i]);
    }
    // Create BoundedBuffer instances for sports, news, and weather queues
    BoundedBuffer *sports_queue = new BoundedBuffer(coeditor_queue_size);
    BoundedBuffer *news_queue = new BoundedBuffer(coeditor_queue_size);
    BoundedBuffer *weather_queue = new BoundedBuffer(coeditor_queue_size);
    // Create Dispatcher and threads for dispatching news
    thread dispatcher_thread(dispatch, producer_queues, sports_queue, news_queue, weather_queue, num_producers);
    // Create BoundedBuffer for co-editor and threads for co-analysis
    UnboundedBuffer *co_editor = new UnboundedBuffer();
    thread co1(co_analyze, sports_queue, co_editor);
    thread co2(co_analyze, news_queue, co_editor);
    thread co3(co_analyze, weather_queue, co_editor);
    // Thread for printing co-editor output
    atomic<int> countdone(0);
    atomic<int> *save = &countdone;
    thread screen_manager(print, co_editor, save);
    // Cleanup: delete BoundedBuffer instances
    for (auto &thread: producer_threads) {
        thread.join();
    }
    dispatcher_thread.join();
    co1.join();
    co2.join();
    co3.join();
    screen_manager.join();
    if (*save == 3) {
        for (auto queue: producer_queues) {
            delete queue;
        }
    }
    delete sports_queue;
    delete news_queue;
    delete weather_queue;
    return 0;
}
