#include "BoundedBuffer.h"
#include <iostream>

class Dispatcher {
private:
    std::vector<BoundedBuffer*>& producerQueues;
    BoundedBuffer& sportsQueue;
    BoundedBuffer& newsQueue;
    BoundedBuffer& weatherQueue;
    int numProducers;

public:
    Dispatcher(std::vector<BoundedBuffer*>& producerQueues,
               BoundedBuffer& sportsQueue, BoundedBuffer& newsQueue,
               BoundedBuffer& weatherQueue, int numProducers)
            : producerQueues(producerQueues), sportsQueue(sportsQueue),
              newsQueue(newsQueue), weatherQueue(weatherQueue),
              numProducers(numProducers) {}

    void dispatch() {
        int doneCount = 0;
        size_t currentQueue = 0;

        while (doneCount < numProducers) {
            if (currentQueue >= producerQueues.size()) {
                currentQueue = 0;
            }
            std::string item = producerQueues[currentQueue]->remove();
            if (item == "DONE") {
                doneCount++;
            } else if (item.find("SPORTS") != std::string::npos) {
                sportsQueue.insert(item);
            } else if (item.find("NEWS") != std::string::npos) {
                newsQueue.insert(item);
            } else if (item.find("WEATHER") != std::string::npos) {
                weatherQueue.insert(item);
            }

            currentQueue++;
        }

        sportsQueue.insert("DONE");
        newsQueue.insert("DONE");
        weatherQueue.insert("DONE");
    }
};

