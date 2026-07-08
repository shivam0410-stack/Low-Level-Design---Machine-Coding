#pragma once

#include "NotificationStrategy.hpp"
#include <iostream>

class SMSStrategy : public NotificationStrategy {
public:
    void sendNotification(std::string& memberId, std::string& content) override {
        std::cout<<"SMS: Hi " + memberId + ", " + content + '\n';
    }
};