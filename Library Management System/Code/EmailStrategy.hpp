#pragma once

#include "NotificationStrategy.hpp"
#include <iostream>

class EmailStrategy : public NotificationStrategy {
public:
    void sendNotification(std::string& memberId, std::string& content) override {
        std::cout<<"Email: Hi " + memberId + ", " + content + '\n';
    }
};