#include <bits/stdc++.h>
using namespace std;

struct Request {
    int source;
    int destination;
};

class LiftSystem {
    int floors, liftCapacity;
    int currentFloor, currentCapacity;
    char currentDirection;

    vector<Request> upRequests, downRequests;

    bool checkCapacity(int startFloor, int destinationFloor) {
        vector<Request>& requests = (startFloor < destinationFloor) ? upRequests : downRequests;

        vector<int> diff(floors, 0);
        for(auto& [source, destination] : requests) {
            if(source == -1 || destination == -1) continue;
            int start = min(source, destination);
            int end = max(source, destination);

            diff[start]++;
            diff[end]--;
        }

        diff[min(startFloor, destinationFloor)]++;
        diff[max(startFloor, destinationFloor)]--;

        int passengers = 0;
        for(int i=0;i<floors;i++) {
            passengers += diff[i];
            if(passengers > liftCapacity) {
                return false;
            }
        }

        return true;
    }

    bool addUpRequest(int startFloor, int destinationFloor) {
        if(!checkCapacity(startFloor, destinationFloor)) {
            return false;
        }

        if(currentDirection == 'I') {
            upRequests.push_back({startFloor, destinationFloor});
            currentDirection = 'U';
            return true;
        }

        if(currentDirection == 'D') {
            upRequests.push_back({startFloor, destinationFloor});
            return true;
        }

        if(currentFloor <= startFloor) {
            upRequests.push_back({startFloor, destinationFloor});
            return true;
        }

        return false;
    }

    bool addDownRequest(int startFloor, int destinationFloor) {
        if(!checkCapacity(startFloor, destinationFloor)) {
            return false;
        }

        if(currentDirection == 'I') {
            downRequests.push_back({startFloor, destinationFloor});
            currentDirection = 'D';
            return true;
        }

        if(currentDirection == 'U') {
            downRequests.push_back({startFloor, destinationFloor});
            return true;
        }

        if(currentFloor >= startFloor) {
            downRequests.push_back({startFloor, destinationFloor});
            return true;
        }

        return false;
    }

    void updateLift() {
        vector<Request>& requests = (currentDirection == 'U') ? upRequests : downRequests;
        int remainingRequests = requests.size();

        for(auto& [source, destination] : requests) {
            if(source == currentFloor) {
                currentCapacity++;
                source = -1;
            }
            else if(destination == currentFloor) {
                currentCapacity--;
                destination = -1;
            }
            if(destination == -1) {
                remainingRequests--;
            }
        }

        if(remainingRequests == 0) {
            if(currentDirection == 'U') {
                requests.clear();
                if(!downRequests.empty()) {
                    currentDirection = 'D';
                }
                else {
                    currentDirection = 'I';
                }
            }
            else if(currentDirection == 'D') {
                requests.clear();
                if(!upRequests.empty()) {
                    currentDirection = 'U';
                }
                else {
                    currentDirection = 'I';
                }
            }
        }
    }

public:
    LiftSystem(int floors, int liftCapacity)
    : floors(floors), liftCapacity(liftCapacity) {
        currentFloor = 0;
        currentCapacity = 0;
        currentDirection = 'I';
    }

    bool addRequest(int startFloor, int destinationFloor) {
        if(startFloor < destinationFloor) {
            if(addUpRequest(startFloor, destinationFloor)) {
                updateLift();
                return true;
            }

            return false;
        }

        if(addDownRequest(startFloor, destinationFloor)) {
            updateLift();
            return true;
        }

        return false;
    }

    string getLiftState() {
        return to_string(currentFloor) + '-' + currentDirection + '-' + to_string(currentCapacity);
    }

    void tick() {
        if(currentDirection == 'I') {
            return ;
        }

        if(currentDirection == 'U') {
            currentFloor++;
        }
        else if(currentDirection == 'D') {
            currentFloor--;
        }

        updateLift();
        cout<<currentFloor<<'\n';
    }
};


int main() {
    // Enable printing of true/false instead of 1/0
    std::cout << std::boolalpha;

    // =========================================================================
    // Example 1 : Already Passed While Going Up (Reject)
    // =========================================================================
    std::cout << "--- Example 1 ---" << std::endl;
    LiftSystem* ls1 = new LiftSystem(10, 4);

    std::cout << "Initial state: " << ls1->getLiftState() << std::endl;

    std::cout << "Add request (0, 7): " << ls1->addRequest(0, 7) << std::endl;
    std::cout << "State: " << ls1->getLiftState() << std::endl;

    ls1->tick();
    ls1->tick();
    ls1->tick();
    ls1->tick();
    std::cout << "State after 4 ticks: " << ls1->getLiftState() << std::endl;

    std::cout << "Add request (2, 5): " << ls1->addRequest(2, 5) << std::endl;
    
    delete ls1;
    std::cout << std::endl;

    // =========================================================================
    // Example 2 : Already Passed While Going Down (Reject)
    // =========================================================================
    std::cout << "--- Example 2 ---" << std::endl;
    LiftSystem* ls2 = new LiftSystem(8, 3);

    std::cout << "Add request (0, 4): " << ls2->addRequest(0, 4) << std::endl;
    std::cout << "State: " << ls2->getLiftState() << std::endl;

    ls2->tick();
    ls2->tick();
    ls2->tick();
    ls2->tick();
    std::cout << "State after 4 ticks: " << ls2->getLiftState() << std::endl;

    std::cout << "Add request (3, 1): " << ls2->addRequest(3, 1) << std::endl;
    std::cout << "State: " << ls2->getLiftState() << std::endl;

    ls2->tick();
    std::cout << "State after 1 tick: " << ls2->getLiftState() << std::endl;

    std::cout << "Add request (6, 2): " << ls2->addRequest(6, 2) << std::endl;
    
    delete ls2;
    std::cout << std::endl;

    // =========================================================================
    // Example 3 - Direction Consistency + Deferred Opposite Direction
    // =========================================================================
    std::cout << "--- Example 3 ---" << std::endl;
    LiftSystem* ls3 = new LiftSystem(12, 3);

    std::cout << "Add request (1, 4): " << ls3->addRequest(1, 4) << std::endl;
    std::cout << "Add request (2, 1): " << ls3->addRequest(2, 1) << std::endl;
    std::cout << "State: " << ls3->getLiftState() << std::endl;

    ls3->tick();
    std::cout << "State after 1 tick: " << ls3->getLiftState() << std::endl;

    ls3->tick();
    ls3->tick();
    ls3->tick();
    std::cout << "State after 3 more ticks: " << ls3->getLiftState() << std::endl;

    std::cout << "Add request (6, 1): " << ls3->addRequest(6, 1) << std::endl;
    std::cout << "Add request (3, 1): " << ls3->addRequest(3, 1) << std::endl;
    std::cout << "Add request (4, 8): " << ls3->addRequest(4, 8) << std::endl;

    delete ls3;
    
    return 0;
}