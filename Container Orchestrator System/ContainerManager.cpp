#include <bits/stdc++.h>
using namespace std;


struct Machine {
    string id;
    int totalCpuUnits, totalMemory;
    int freeCpuUnits, freeMemory;

    Machine(const string& machineId, int totalCpuUnits, int totalMemory) :
    id(machineId), totalCpuUnits(totalCpuUnits), totalMemory(totalMemory),
    freeCpuUnits(totalCpuUnits), freeMemory(totalMemory) {}
};


struct Container {
    string name;
    string imageUrl;
    int cpuUnits;
    int memMb;

    Container(const string& name, const string& imageUrl, int cpuUnits, int memMb)
    : name(name), imageUrl(imageUrl), cpuUnits(cpuUnits), memMb(memMb) {}
};


class Strategy {
public:
    virtual Machine* findMachine(const vector<Machine*>& machineList, Container* container) = 0;
};


class CpuStrategy : public Strategy {
public:
    Machine* findMachine(const vector<Machine*>& machineList, Container* container) {
        Machine* selectedMachine = nullptr;
        int maxCpuUnitsLeft = 0;

        int requiredCpuUnits = container->cpuUnits;
        int requiredMemory = container->memMb;

        for(auto& machine : machineList) {
            if(machine->freeCpuUnits >= requiredCpuUnits && machine->freeMemory >= requiredMemory) {
                if(machine->freeCpuUnits > maxCpuUnitsLeft) {
                    selectedMachine = machine;
                    maxCpuUnitsLeft = machine->freeCpuUnits;
                }
                else if(machine->freeCpuUnits == maxCpuUnitsLeft && machine->id < selectedMachine->id) {
                    selectedMachine = machine;
                }
            }
        }

        return selectedMachine;
    }
};


class MemoryStrategy : public Strategy {
public:
    Machine* findMachine(const vector<Machine*>& machineList, Container* container) {
        Machine* selectedMachine = nullptr;
        int maxMemoryLeft = 0;

        int requiredCpuUnits = container->cpuUnits;
        int requiredMemory = container->memMb;

        for(auto& machine : machineList) {
            if(machine->freeCpuUnits >= requiredCpuUnits && machine->freeMemory >= requiredMemory) {
                if(machine->freeMemory > maxMemoryLeft) {
                    selectedMachine = machine;
                    maxMemoryLeft = machine->freeMemory;
                }
                else if(machine->freeMemory == maxMemoryLeft && machine->id < selectedMachine->id) {
                    selectedMachine = machine;
                }
            }
        }

        return selectedMachine;
    }
};


class ContainerManager {
    vector<Machine*> machineList;
    unordered_map<string, Machine*> machineAssigned;
    unordered_map<string, Container*> containerMap;
    vector<Strategy*> cache;

    Strategy* getStrategy(int criteria) {
        if(criteria < 0 || criteria >= 2) {
            throw invalid_argument("Invalid Criteria");
        }

        return cache[criteria];
    }

    void updateResources(Container* container, Machine* machine) {
        machine->freeCpuUnits -= container->cpuUnits;
        machine->freeMemory -= container->memMb;
    }

    void freeResources(Container* container, Machine* machine) {
        machine->freeCpuUnits += container->cpuUnits;
        machine->freeMemory += container->memMb;
    }

public:
    ContainerManager(const vector<string>& machines) {
        for(auto& machine : machines) {
            int start = 0;
            int end = machine.find(',');
            string machineId = machine.substr(start, end - start);
            
            start = end + 1;
            end = machine.find(',', start);
            int totalCpuUnits = stoi(machine.substr(start, end - start));

            start = end + 1;
            int totalMemory = stoi(machine.substr(start));

            machineList.push_back(new Machine(machineId, totalCpuUnits, totalMemory));
        }

        cache.resize(2);
        cache[0] = new CpuStrategy();
        cache[1] = new MemoryStrategy();
    }

    string assignMachine(int criteria, const string& containerName,
    const string& imageUrl, int cpuUnits, int memMb) {
        Strategy* strategy = getStrategy(criteria);
        Container* container = new Container(containerName, imageUrl, cpuUnits, memMb);

        Machine* machine = strategy->findMachine(machineList, container);
        if(machine == nullptr) {
            return "";
        }

        containerMap[containerName] = container;
        machineAssigned[containerName] = machine;

        updateResources(container, machine);
        return machine->id;
    }

    bool stop(const string& name) {
        auto machine = machineAssigned.find(name);
        if(machine == machineAssigned.end()) {
            return false;
        }

        freeResources(containerMap[name], machineAssigned[name]);
        machineAssigned.erase(name);
        containerMap.erase(name);

        return true;
    }
};


int main() {
    std::cout << std::boolalpha; // Formats 1/0 boolean outputs as true/false

    // Common Set-up (for all scenarios) Constructor
    std::vector<std::string> machineConfigs = {
        "mA,8,16000",
        "mB,8,8000",
        "mC,4,32000"
    };
    ContainerManager* manager = new ContainerManager(machineConfigs);

    // Scenario 1 - CPU-first (criteria=0) with tie-break, then another placement
    std::cout << "--- Scenario 1 ---" << std::endl;
    
    std::cout << "Call: assignMachine(0, \"c1\", \"img://a\", 2, 2000)" << std::endl;
    std::cout << "Expected return: \"mA\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(0, "c1", "img://a", 2, 2000) << "\"" << std::endl << std::endl;

    std::cout << "Call: assignMachine(0, \"c2\", \"img://b\", 6, 1000)" << std::endl;
    std::cout << "Expected return: \"mB\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(0, "c2", "img://b", 6, 1000) << "\"" << std::endl << std::endl;

    std::cout << "Call: assignMachine(0, \"c3\", \"img://c\", 3, 1000)" << std::endl;
    std::cout << "Expected return: \"mA\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(0, "c3", "img://c", 3, 1000) << "\"" << std::endl << std::endl;

    // Scenario 2 - Memory-first (criteria=1) and a failure due to insufficient memory
    std::cout << "--- Scenario 2 ---" << std::endl;

    std::cout << "Call: assignMachine(1, \"c4\", \"img://d\", 2, 9000)" << std::endl;
    std::cout << "Expected return: \"mC\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(1, "c4", "img://d", 2, 9000) << "\"" << std::endl << std::endl;

    std::cout << "Call: assignMachine(1, \"c5\", \"img://e\", 2, 25000)" << std::endl;
    std::cout << "Expected return: \"\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(1, "c5", "img://e", 2, 25000) << "\"" << std::endl << std::endl;

    // Scenario 3 - stop(...) frees resources; retry succeeds
    std::cout << "--- Scenario 3 ---" << std::endl;

    std::cout << "Call: stop(\"c4\")" << std::endl;
    std::cout << "Expected return: true" << std::endl;
    std::cout << "Actual return:   " << manager->stop("c4") << std::endl << std::endl;

    std::cout << "Call: assignMachine(1, \"c5\", \"img://e\", 2, 25000)" << std::endl;
    std::cout << "Expected return: \"mC\"" << std::endl;
    std::cout << "Actual return:   \"" << manager->assignMachine(1, "c5", "img://e", 2, 25000) << "\"" << std::endl << std::endl;

    std::cout << "Call: stop(\"doesNotExist\")" << std::endl;
    std::cout << "Expected return: false" << std::endl;
    std::cout << "Actual return:   " << manager->stop("doesNotExist") << std::endl << std::endl;

    std::cout << "Call: stop(\"c4\") again" << std::endl;
    std::cout << "Expected return: false" << std::endl;
    std::cout << "Actual return:   " << manager->stop("c4") << std::endl << std::endl;

    // Clean up allocated memory
    delete manager;
    return 0;
}
