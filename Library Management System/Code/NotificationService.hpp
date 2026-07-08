#pragma once

#include "NotificationStrategy.hpp"

class NotificationService {
private:
    NotificationStrategy* strategy_;

public:
    NotificationService(NotificationStrategy* strategy) :
        strategy_(strategy) {}

    void sendNotification(std::string& memberId, std::string& content) {
        strategy_->sendNotification(memberId, content);
    }
};