#pragma once

#include <string>

class NotificationStrategy {
public:
    virtual void sendNotification(std::string& memberId, std::string& content) = 0;
};