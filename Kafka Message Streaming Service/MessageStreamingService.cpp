#include <bits/stdc++.h>
using namespace std;

// AI suggests to use objects instead of pointers in this case
// It says not to use raw pointers in a high speed LLD interview until absolutely needed
// I'm not considering smart pointers for interviews right now

class Partition {
    vector<string> messages;

public:
    int publish(const string& message) {
        messages.push_back(message);
        return messages.size() - 1;
    }

    vector<string> consume(int cursor, int maxMessages) {
        int offset = messages.size();
        vector<string> messageList;

        while(cursor < offset && maxMessages != 0) {
            messageList.push_back(messages[cursor]);
            cursor++;
            maxMessages--;
        }

        return messageList;
    }
};

class Topic {
    string topicName;
    int partitionCount;
    vector<Partition*> partitions;

public:
    Topic(const string& topicName, int partitionCount) : topicName(topicName), partitionCount(partitionCount) {
        partitions.resize(partitionCount);

        for(int id = 0; id < partitionCount; id ++) {
            partitions[id] = new Partition();
        }
    }

    string publish(int partitionId, const string& message) {
        int offset = partitions[partitionId]->publish(message);
        return "p" + to_string(partitionId) + ":" + to_string(offset);
    }

    vector<string> consume(int partitionId, int cursor, int maxMessages) {
        return partitions[partitionId]->consume(cursor, maxMessages);
    }

    int getPartitionCount() {
        return partitionCount;
    }
};


class Consumer {
    string consumerId;
    unordered_map<string, vector<int>> cursor;

public:
    Consumer(const string& consumerId) : consumerId(consumerId) {}

    int getCursor(const string& topicName, int partitionId, int partitionCount) {
        if(cursor.find(topicName) == cursor.end()) {
            cursor[topicName].assign(partitionCount, 0);
        }

        return cursor[topicName][partitionId];
    }

    void updateCursor(const string& topicName, int partitionId, int increment) {
        cursor[topicName][partitionId] += increment;
    }
};


class MessageStreamingService {
    unordered_map<string, Topic*> topics;
    unordered_map<string, Consumer*> consumers;

public:
    bool createTopic(string topicName, int partitionCount) {
        if(topics.find(topicName) != topics.end()) {
            return false;
        }

        topics[topicName] = new Topic(topicName, partitionCount);
        return true;
    }

    string publish(string topicName, int partitionId, string message) {
        return topics[topicName]->publish(partitionId, message);
    }

    vector<string> consume(string topicName, string consumerId, int partitionId, int maxMessages) {
        if(topics.find(topicName) == topics.end()) {
            return {};
        }

        Topic* topic = topics[topicName];
        int partitionCount = topic->getPartitionCount();

        if(partitionId >= partitionCount) {
            cout<<"Invalid Partition Id\n";
            return {};
        }

        if(consumers.find(consumerId) == consumers.end()) {
            consumers[consumerId] = new Consumer(consumerId);
        }

        Consumer* consumer = consumers[consumerId];

        int cursor = consumer->getCursor(topicName, partitionId, partitionCount);
        vector<string> messages = topic->consume(partitionId, cursor, maxMessages);
        consumer->updateCursor(topicName, partitionId, messages.size());

        return messages;
    }
};


int main() {
    return 0;
}