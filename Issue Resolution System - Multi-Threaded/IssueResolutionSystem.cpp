#include <bits/stdc++.h>
using namespace std;

// Improvements
// Better to use atomic int for data members in Agent Class instead of locks

enum class IssueState {
    OPEN,
    ASSIGNED,
    RESOLVED
};


struct Issue {
    string id, orderId, description;
    int issueType;
    IssueState state;

    Issue(const string& issueId, const string& orderId, int issueType, const string& description) :
    id(issueId), orderId(orderId), issueType(issueType), description(description),
    state(IssueState::OPEN) {}
};


struct Agent {
    string id;
    vector<bool> expertise;

    int totalIssuesOpen;
    vector<string> resolvedIssues;
    vector<int> openIssuesByType, resolvedIssuesByType;

    shared_mutex mtx;

    Agent(const string& agentId, const vector<int>& expertiseList, int size) : id(agentId) {
        expertise.assign(size, false);
        for(auto& expertiseId : expertiseList) {
            expertise[expertiseId] = true;
        }

        openIssuesByType.assign(size, 0);
        resolvedIssuesByType.assign(size, 0);
        totalIssuesOpen = 0;
    }

    void assignIssue(int issueType) {
        unique_lock<shared_mutex> lock(mtx);
        totalIssuesOpen++;
        openIssuesByType[issueType]++;
    }

    void resolveIssue(Issue* &issue) {
        int issueType = issue->issueType;

        unique_lock<shared_mutex> lock(mtx);
        openIssuesByType[issueType]--;
        resolvedIssuesByType[issueType]++;

        resolvedIssues.push_back(issue->id);
    }

    bool isAgentCapable(int issueType) {
        return expertise[issueType];
    }

    vector<string> getResolvedIssues() {
        shared_lock<shared_mutex> lock(mtx);
        return resolvedIssues;
    }
};


class Strategy {
public:
    virtual Agent* assignAgent(Issue* &issue, vector<Agent*> &agents) = 0;
};
class Strategy0 : public Strategy {
public:
    Agent* assignAgent(Issue* &issue, vector<Agent*> &agents) override {
        int minCount = INT_MAX;
        Agent* assignedAgent = nullptr;

        for(auto& agent : agents) {
            shared_lock<shared_mutex> lock(agent->mtx);
            if(agent->totalIssuesOpen < minCount) {
                minCount = agent->totalIssuesOpen;
                assignedAgent = agent;
            }
        }

        return assignedAgent;
    }
};
class Strategy1 : public Strategy {
public:
    Agent* assignAgent(Issue* &issue, vector<Agent*> &agents) override {
        int maxCount = INT_MIN;
        int issueType = issue->issueType;
        Agent* assignedAgent = nullptr;

        for(auto& agent : agents) {
            shared_lock<shared_mutex> lock(agent->mtx);
            if(agent->resolvedIssuesByType[issueType] > maxCount) {
                maxCount = agent->resolvedIssuesByType[issueType];
                assignedAgent = agent;
            }
        }

        return assignedAgent;
    }
};
class Strategy2 : public Strategy {
public:
    Agent* assignAgent(Issue* &issue, vector<Agent*> &agents) override {
        int minCount = INT_MAX;
        int issueType = issue->issueType;
        Agent* assignedAgent = nullptr;

        for(auto& agent : agents) {
            shared_lock<shared_mutex> lock(agent->mtx);
            if(agent->openIssuesByType[issueType] < minCount) {
                minCount = agent->openIssuesByType[issueType];
                assignedAgent = agent;
            }
        }

        return assignedAgent;
    }
};


class AgentManager {
    unordered_map<string, Agent*> agents;
    unordered_map<string, string> issueAssignedAgent;
    vector<Strategy*> strategies;

    mutex issueMutex;

    vector<Agent*> findAgentsWithExpertise(int issueType) {
        vector<Agent*> validAgents;
        for(auto& [id, agent] : agents) {
            if(agent->isAgentCapable(issueType)) {
                validAgents.push_back(agent);
            }
        }
        return validAgents;
    }

public:
    void init() {
        for(auto& [id, agent] : agents) {
            delete agent;
        }
        agents.clear();

        issueAssignedAgent.clear();

        for(auto& strategy : strategies) {
            delete strategy;
        }

        strategies.resize(3);
        strategies[0] = new Strategy0();
        strategies[1] = new Strategy1();
        strategies[2] = new Strategy2();
    }

    string addAgent(const string& agentId, const vector<int>& expertise, int size) {
        if(agents.find(agentId) != agents.end()) {
            return "agent already exists";
        }

        Agent* agent = new Agent(agentId, expertise, size);
        agents[agentId] = agent;
        return "success";
    }

    string assignAgent(Issue* &issue, int assignStrategy) {
        vector<Agent*> validAgents = findAgentsWithExpertise(issue->issueType);

        if(validAgents.empty()) {
            return "agent with expertise doesn't exist";
        }

        Agent* agent = strategies[assignStrategy]->assignAgent(issue, validAgents);

        unique_lock<mutex> lock(issueMutex);
        issueAssignedAgent[issue->id] = agent->id;
        lock.unlock();

        agent->assignIssue(issue->issueType);

        return agent->id;
    }

    void resolveIssue(Issue* &issue) {
        string agentId = issueAssignedAgent[issue->id];
        Agent* agent = agents[agentId];
        agent->resolveIssue(issue);
    }

    vector<string> getAgentHistory(const string& agentId) {
        return agents[agentId]->getResolvedIssues();
    }
};


class IssueResolutionSystem {
    AgentManager agentManager;
    unordered_map<string, Issue*> issues;
    vector<string> issueTypeData;

    shared_mutex issueMutex, agentMutex;
    unordered_map<string, unique_ptr<mutex>> issueMutexMap;

public:
    void init(const vector<string>& issueTypes) {
        issueTypeData = issueTypes;

        for(auto& [id, issue] : issues) {
            delete issue;
        }
        issues.clear();

        agentManager.init();
    }

    string createIssue(const string& issueId, const string& orderId, int issueType,
    const string& description) {
        unique_lock<shared_mutex> lock(issueMutex);
        if(issues.find(issueId) != issues.end()) {
            return "issue already exists";
        }

        if(issueType < 0 || issueType >= issueTypeData.size()) {
            return "invalid issue type";
        }

        Issue* issue = new Issue(issueId, orderId, issueType, description);
        issueMutexMap[issueId] = make_unique<mutex>();
        issues[issueId] = issue;

        return "issue created";
    }

    string addAgent(const string& agentId, const vector<int>& expertise) {
        unique_lock<shared_mutex> lock(agentMutex);
        return agentManager.addAgent(agentId, expertise, issueTypeData.size());
    }

    string assignIssue(const string& issueId, int assignStrategy) {
        shared_lock<shared_mutex> issueLock(issueMutex);

        auto it = issues.find(issueId);
        if(it == issues.end()) {
            return "issue doesn't exist";
        }

        Issue* issue = it->second;

        // Using at since it throws out of bounds instead of silently crashing
        unique_lock<mutex> lock(*issueMutexMap.at(issueId));
        
        issueLock.unlock();

        if(issue->state == IssueState::ASSIGNED) {
            return "issue already assigned";
        }
        if(issue->state == IssueState::RESOLVED) {
            return "issue already resolved";
        }

        shared_lock<shared_mutex> sharedLock(agentMutex);
        return agentManager.assignAgent(issue, assignStrategy);
    }

    void resolveIssue(const string& issueId, const string& resolution) {
        // Issue is always valid here
        unique_lock<mutex> lock(*issueMutexMap[issueId]);

        Issue* issue = issues[issueId];
        if(issue->state == IssueState::RESOLVED) {
            return ;
        }
        issue->state = IssueState::RESOLVED;

        shared_lock<shared_mutex> sharedLock(agentMutex);
        agentManager.resolveIssue(issue);
    }

    vector<string> getAgentHistory(const string& agentId) {
        shared_lock<shared_mutex> lock(agentMutex);
        return agentManager.getAgentHistory(agentId);
    }
};


// Helper to neatly print vector contents like [A-0, A-1, A-2]
void printVector(const std::string& label, const std::vector<std::string>& vec) {
    std::cout << label << " : [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    // Instantiate the primary object pointer
    IssueResolutionSystem* system = new IssueResolutionSystem();

    // 1. Initialization (No cout)
    system->init({"wrong product received", "order delayed", "cancel delivery", "damaged product received"});

    // 2. Add Agents (cout added)
    std::cout << system->addAgent("A-0", {0, 1, 3}) << "\n";
    std::cout << system->addAgent("A-1", {1, 2, 3}) << "\n";
    std::cout << system->addAgent("A-2", {1, 3}) << "\n";

    // 3. Issue Actions Sequence (cout added to assignments as well)
    std::cout << system->createIssue("I-0", "Order-0", 3, "sample issue description : damaged product received") << "\n";
    std::cout << system->assignIssue("I-0", 1) << "\n";

    std::cout << system->createIssue("I-1", "Order-1", 1, "sample issue description : order delayed") << "\n";
    std::cout << system->assignIssue("I-1", 1) << "\n";

    std::cout << system->createIssue("I-2", "Order-2", 1, "sample issue description : order delayed") << "\n";
    std::cout << system->assignIssue("I-2", 0) << "\n";

    std::cout << system->createIssue("I-3", "Order-3", 2, "sample issue description : cancel delivery") << "\n";
    std::cout << system->assignIssue("I-3", 2) << "\n";

    system->resolveIssue("I-3", "resolved"); // No cout
    printVector("getAgentHistory('A-1')", system->getAgentHistory("A-1"));

    std::cout << system->createIssue("I-4", "Order-4", 3, "sample issue description : damaged product received") << "\n";
    std::cout << system->assignIssue("I-4", 2) << "\n";

    std::cout << system->createIssue("I-5", "Order-5", 1, "sample issue description : order delayed") << "\n";
    std::cout << system->assignIssue("I-5", 0) << "\n";

    system->resolveIssue("I-1", "resolved"); // No cout
    printVector("getAgentHistory('A-1')", system->getAgentHistory("A-1"));

    system->resolveIssue("I-5", "resolved"); // No cout
    printVector("getAgentHistory('A-0')", system->getAgentHistory("A-0"));

    system->resolveIssue("I-4", "resolved"); // No cout
    printVector("getAgentHistory('A-0')", system->getAgentHistory("A-0"));

    system->resolveIssue("I-0", "resolved"); // No cout
    printVector("getAgentHistory('A-1')", system->getAgentHistory("A-1"));

    system->resolveIssue("I-2", "resolved"); // No cout
    printVector("getAgentHistory('A-2')", system->getAgentHistory("A-2"));

    std::cout << system->createIssue("I-6", "Order-6", 3, "sample issue description : damaged product received") << "\n";
    std::cout << system->assignIssue("I-6", 2) << "\n";

    std::cout << system->createIssue("I-7", "Order-7", 3, "sample issue description : damaged product received") << "\n";
    std::cout << system->assignIssue("I-7", 2) << "\n";

    std::cout << system->createIssue("I-8", "Order-8", 1, "sample issue description : order delayed") << "\n";
    std::cout << system->assignIssue("I-8", 2) << "\n";

    system->resolveIssue("I-7", "resolved"); // No cout
    printVector("getAgentHistory('A-2')", system->getAgentHistory("A-2"));

    system->resolveIssue("I-8", "resolved"); // No cout
    printVector("getAgentHistory('A-1')", system->getAgentHistory("A-1"));

    std::cout << system->createIssue("I-9", "Order-9", 0, "sample issue description : wrong product received") << "\n";
    std::cout << system->assignIssue("I-9", 0) << "\n";

    system->resolveIssue("I-6", "resolved"); // No cout
    printVector("getAgentHistory('A-0')", system->getAgentHistory("A-0"));

    system->resolveIssue("I-9", "resolved"); // No cout
    printVector("getAgentHistory('A-0')", system->getAgentHistory("A-0"));

    std::cout << system->createIssue("I-10", "Order-10", 2, "sample issue description : cancel delivery") << "\n";
    std::cout << system->assignIssue("I-10", 0) << "\n";

    system->resolveIssue("I-10", "resolved"); // No cout
    printVector("getAgentHistory('A-1')", system->getAgentHistory("A-1"));

    // Clean up dynamic memory
    delete system;
    return 0;
}