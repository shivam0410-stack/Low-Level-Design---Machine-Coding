#include <bits/stdc++.h>
using namespace std;


class Machine {
    string id;
    unordered_set<string> capabilities;
    int finishedJobs, unfinishedJobs;

public:
    Machine(const string& machineId, const vector<string>& machineCapabilities) : 
    id(machineId) {
        for(auto& capability : machineCapabilities) {
            capabilities.insert(capability);
        }
        finishedJobs = unfinishedJobs = 0;
    }

    string getId() { return id; }

    void assignJob() { unfinishedJobs++; }

    void completeJob() {
        unfinishedJobs--;
        finishedJobs++;
    }

    int getFinishedJobs() { return finishedJobs; }

    int getUnfinishedJobs() { return unfinishedJobs; }

    bool findCapability(const string& capability) {
        return capabilities.find(capability) != capabilities.end();
    }
};


class Algorithm {
public:
    virtual Machine* assignMachine(const vector<Machine*>& machines,
    const vector<string>& capabilitiesRequired) = 0;
};

class Algorithm1 : public Algorithm {
public:
    Machine* assignMachine(const vector<Machine*>& machines,
    const vector<string>& capabilitiesRequired) override {
        Machine* machineAssigned = nullptr;
        for(auto& machine : machines) {
            bool canAssign = true;
            for(auto& capability : capabilitiesRequired) {
                if(machine->findCapability(capability) == false) {
                    canAssign = false;
                    break;
                }
            }
            if(canAssign == false) {
                continue;
            }

            if(machineAssigned == nullptr) {
                machineAssigned = machine;
                continue;
            }

            if(machine->getUnfinishedJobs() < machineAssigned->getUnfinishedJobs()) {
                machineAssigned = machine;
                continue;
            }

            if(machine->getUnfinishedJobs() == machineAssigned->getUnfinishedJobs()
            && machine->getId() < machineAssigned->getId()) {
                machineAssigned = machine;
                continue;
            }
        }
        return machineAssigned;
    }
};

class Algorithm2 : public Algorithm {
public:
    Machine* assignMachine(const vector<Machine*>& machines,
    const vector<string>& capabilitiesRequired) override {
        Machine* machineAssigned = nullptr;
        for(auto& machine : machines) {
            bool canAssign = true;
            for(auto& capability : capabilitiesRequired) {
                if(machine->findCapability(capability) == false) {
                    canAssign = false;
                    break;
                }
            }
            if(canAssign == false) {
                continue;
            }

            if(machineAssigned == nullptr) {
                machineAssigned = machine;
                continue;
            }

            if(machine->getFinishedJobs() > machineAssigned->getFinishedJobs()) {
                machineAssigned = machine;
                continue;
            }

            if(machine->getFinishedJobs() == machineAssigned->getFinishedJobs()
            && machine->getId() < machineAssigned->getId()) {
                machineAssigned = machine;
                continue;
            }
        }
        return machineAssigned;
    }
};


class AlgorithmFactory {
    unordered_map<int, Algorithm*> registry;

public:
    Algorithm* getAlgorithm(int criteria) {
        if(registry.find(criteria) == registry.end()) {
            if(criteria == 0) registry[criteria] = new Algorithm1();
            else if(criteria == 1) registry[criteria] = new Algorithm2();
            else throw invalid_argument("Unknown criteria " + to_string(criteria));
        }
        return registry[criteria];
    }
};


class JobScheduler {
    vector<Machine*> machines;
    unordered_map<string, Machine*> machineById;
    unordered_map<string, string> jobToMachineMap;
    AlgorithmFactory factory;

    void toLowerCase(string& capability) {
        string result;
        for(auto& ch : capability) {
            ch = tolower(static_cast<unsigned char>(ch));
        }
    }

public:
    void addMachine(const string& machineId, vector<string> capabilities) {
        for(auto& capability : capabilities) {
            toLowerCase(capability);
        }
        Machine* machine = new Machine(machineId, capabilities);
        machines.push_back(machine);
        machineById[machineId] = machine;
    }

    string assignMachineToJob(const string& jobId,
    vector<string> capabilitiesRequired, int criteria) {
        for(auto& capability : capabilitiesRequired) {
            toLowerCase(capability);
        }

        Algorithm* algorithm = factory.getAlgorithm(criteria);
        Machine* machine = algorithm->assignMachine(machines, capabilitiesRequired);

        if(machine == nullptr) {
            return "";
        }

        string machineId = machine->getId();
        jobToMachineMap[jobId] = machineId;

        machine->assignJob();
        return machineId;
    }

    void jobCompleted(const string& jobId) {
        string machineId = jobToMachineMap[jobId];
        jobToMachineMap.erase(jobId);

        Machine* machine = machineById[machineId];
        machine->completeJob();
    }
};


int main() {
    // Setup Job System Pointer
    JobScheduler* sys = new JobScheduler();

    // =========================================================================
    // Example 1 - Multi-Capability Match + Criteria 0 (Least Unfinished) + Deterministic Tie
    // =========================================================================
    std::cout << "--- Example 1 ---" << std::endl;
    
    sys->addMachine("m-10", {"image compression", "audio extraction", "video thumbnail generation"});
    sys->addMachine("m-2", {"image compression", "audio extraction"});

    std::string assignedM1 = sys->assignMachineToJob("job-A", {"image compression", "audio extraction"}, 0);
    std::cout << "Job-A assigned to machine: \"" << assignedM1 << "\"" << std::endl; 
    // Expected: "m-10" (lexicographically smaller than "m-2" due to '1' < '2')


    // =========================================================================
    // Example 2 - Completion Updates → Criteria 1 (Most Finished)
    // =========================================================================
    std::cout << "\n--- Example 2 ---" << std::endl;
    
    sys->jobCompleted("job-A"); // Updates m-10 counters
    
    std::string assignedM2 = sys->assignMachineToJob("job-B", {"image compression"}, 1);
    std::cout << "Job-B assigned to machine: \"" << assignedM2 << "\"" << std::endl;
    // Expected: "m-10" (m-10 has finished = 1, m-2 has finished = 0)


    // =========================================================================
    // Example 3 - No Compatible Machine → Empty String
    // =========================================================================
    std::cout << "\n--- Example 3 ---" << std::endl;
    
    std::string assignedM3 = sys->assignMachineToJob("job-C", {"speech to text conversion"}, 0);
    std::cout << "Job-C assigned to machine: \"" << assignedM3 << "\"" << std::endl;
    // Expected: "" (No machine matches the capability)

    // Cleanup
    delete sys;
    return 0;
}
