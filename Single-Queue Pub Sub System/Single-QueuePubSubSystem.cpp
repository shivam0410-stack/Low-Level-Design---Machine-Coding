#include <bits/stdc++.h>
using namespace std;

// Adding a vector<Message> to use as queue in the system and using offset for subscribers,
// we can empower queues to consume messages at their own pace.

class Subscriber {
    string id;
    unordered_set<string> eventTypes;
    int processedMessagesCount;

public:
    Subscriber(const string& id) : id(id), processedMessagesCount(0) {}

    int getProcessedMessagesCount() {
        return processedMessagesCount;
    }

    void setEventTypesToProcess(const vector<string>& eventTypesToProcess) {
        eventTypes.clear();
        for(auto& eventType : eventTypesToProcess) {
            eventTypes.insert(eventType);
        }
    }

    void update(const string& eventType, const string& message) {
        if(eventTypes.find(eventType) == eventTypes.end()) {
            return ;
        }

        cout<<message<<" processed by "<<id<<'\n';
        processedMessagesCount++;
    }
};


class QueueManager {
    unordered_map<string, Subscriber*> subscribers;
    unordered_map<string, Subscriber*> unSubscribers;

public:
    void addSubscriber(const string& subscriberId, const vector<string>& eventTypesToProcess) {
        Subscriber* subscriber = nullptr;
        auto unSubscriberIt = unSubscribers.find(subscriberId);

        if(unSubscriberIt == unSubscribers.end()) {
            subscriber = new Subscriber(subscriberId);
        }
        else {
            subscriber = unSubscriberIt->second;
            unSubscribers.erase(unSubscriberIt);
        }

        subscribers[subscriberId] = subscriber;
        subscriber->setEventTypesToProcess(eventTypesToProcess);
        cout<<"Subscriber: "<<subscriberId<<" Added\n";
    }

    void removeSubscriber(string subscriberId) {
        auto subscriberIt = subscribers.find(subscriberId);
        if(subscriberIt == subscribers.end()) {
            cout<<"Active Subscriber not found\n";
            return ;
        }

        unSubscribers[subscriberId] = subscriberIt->second;
        subscribers.erase(subscriberIt);
        cout<<"Removed subscriber: "<<subscriberId<<'\n';
    }

    void sendMessage(string eventType, string message) {
        for(auto& subscriber : subscribers) {
            subscriber.second->update(eventType, message);
        }
    }

    int countProcessedMessages(string subscriberId) {
        Subscriber* subscriber = nullptr;
        auto subscriberIt = subscribers.find(subscriberId);

        if(subscriberIt == subscribers.end()) {
            auto unSubscriberIt = unSubscribers.find(subscriberId);
            if(unSubscriberIt == unSubscribers.end()) {
                cout<<"No subscriber ever existed with id: "<<subscriberId<<'\n';
            }
            else {
                subscriber = unSubscriberIt->second;
            }
        }
        else {
            subscriber = subscriberIt->second;
        }

        return subscriber->getProcessedMessagesCount();
    }

};

int main() {
    return 0;
}