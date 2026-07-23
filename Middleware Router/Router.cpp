#include <bits/stdc++.h>
using namespace std;


class Router {
    unordered_map<string, int> pathIndex;
    vector<vector<int>> lengthBuckets; // segment count -> indices into paths/results
    vector<vector<string>> paths;
    vector<string> results;

    vector<string> convertToSegments(const string& path) {
        vector<string> segments;
        int start = 1, end = 1;

        while(end < path.length()) {
            end = path.find('/', start);
            string segment = path.substr(start, end - start);

            segments.push_back(segment);
            start = end + 1;
        }

        return segments;
    }

    tuple<bool, int, int> check(const vector<string>& queryPath, const vector<string>& patternSegments) {
        int staticSegmentCount = 0, paramSegmentCount = 0;
        int segmentsCount = queryPath.size();

        for(int i = 0; i < segmentsCount; i++) {
            if(queryPath[i] == "*" || patternSegments[i][0] == ':') {
                continue;
            }

            if(patternSegments[i] == "*") {
                continue;
            }
            if(patternSegments[i][0] == ':') {
                paramSegmentCount++;
                continue;
            }
            if(patternSegments[i] == queryPath[i]) {
                staticSegmentCount++;
            }
            else {
                return {false, 0, 0};
            }
        }

        return {true, staticSegmentCount, paramSegmentCount};
    }

public:
    Router() {
        lengthBuckets.resize(1000);
    }

    void addRoute(const string& pathPattern, const string& result) {
        auto it = pathIndex.find(pathPattern);
        if(it != pathIndex.end()) {
            int index = it->second;
            results[index] = result;
            return ;
        }

        vector<string> segments = convertToSegments(pathPattern);
        pathIndex[pathPattern] = results.size();
        lengthBuckets[segments.size()].push_back(results.size());

        paths.push_back(segments);
        results.push_back(result);
    }

    string callRoute(const string& path) {
        vector<string> queryPath = convertToSegments(path);
        int size = queryPath.size();
        int potentialMatchingPathsCount = lengthBuckets[size].size();
        int matchingStaticSegments = 0, matchingParamSegments = 0;
        string result = "NOT_FOUND";

        for(int i = 0; i < potentialMatchingPathsCount; i++) {
            auto [areEqual, staticSegmentCount, paramSegmentCount] = check(queryPath, paths[lengthBuckets[size][i]]);

            if(areEqual && (staticSegmentCount > matchingStaticSegments || 
            staticSegmentCount == matchingStaticSegments && paramSegmentCount > matchingParamSegments)) {
                result = results[lengthBuckets[size][i]];
                matchingStaticSegments = staticSegmentCount;
                matchingParamSegments = paramSegmentCount;
            }
        }

        return result;
    }

    vector<string> searchRoutes(const string& wildcardPattern) {
        vector<string> queryPath = convertToSegments(wildcardPattern);
        int size = queryPath.size();
        int potentialMatchingPathsCount = lengthBuckets[size].size();
        vector<string> routeResults;

        for(int i = 0; i < potentialMatchingPathsCount; i++) {
            auto [areEqual, staticSegmentCount, paramSegmentCount] = check(queryPath, paths[lengthBuckets[size][i]]);

            if(areEqual) {
                routeResults.push_back(results[lengthBuckets[size][i]]);
            }
        }

        return routeResults;
    }

};


void print(const vector<string>& searchResults) {
    for(auto& result : searchResults) {
        cout<<result<<" ";
    }
    cout<<'\n';
}


int main() {
    // =========================================================================
    // Example 1 — Basic static route
    // =========================================================================
    std::cout << "--- Example 1 ---" << std::endl;
    Router router1;
    router1.addRoute("/bar", "server-1");
    
    // Prints: server-1
    std::cout << router1.callRoute("/bar") << std::endl; 
    // Prints: NOT_FOUND
    std::cout << router1.callRoute("/bar/xyz") << std::endl; 


    // =========================================================================
    // Examples — Testing only searchRoutes
    // =========================================================================
    std::cout << "\n--- Testing searchRoutes ---" << std::endl;
    Router routerSearch;
    
    // Add a mix of patterns
    routerSearch.addRoute("/bar/*/baz", "server-wild");
    routerSearch.addRoute("/bar/:x/baz", "server-param");
    routerSearch.addRoute("/bar/a/baz", "server-static");
    routerSearch.addRoute("/bar/*/qux", "server-qux");
    
    // Update an existing pattern
    routerSearch.addRoute("/bar/*/baz", "server-wild-v2");

    // 1) Wildcard query matches multiple stored patterns
    std::vector<std::string> search1 = routerSearch.searchRoutes("/bar/*/baz");
    print(search1);
    
    // 2) Fully static query matches param/wildcard patterns
    std::vector<std::string> search2 = routerSearch.searchRoutes("/bar/a/baz");
    print(search2);
    
    // 3) Query with same length but different static segment
    std::vector<std::string> search3 = routerSearch.searchRoutes("/bar/*/qux");
    print(search3);
    
    // 4) Length mismatch => no matches
    std::vector<std::string> search4 = routerSearch.searchRoutes("/bar/*/a/baz");
    print(search4);
    
    // 5) No compatible route at all
    std::vector<std::string> search5 = routerSearch.searchRoutes("/nope/*");
    print(search5);


    // =========================================================================
    // Example 2 — Wildcards (Scale Up 1)
    // =========================================================================
    std::cout << "\n--- Example 2 ---" << std::endl;
    Router router2;
    router2.addRoute("/foo", "server-foo");
    router2.addRoute("/bar/*/baz", "server-bar");
    
    std::cout << router2.callRoute("/foo") << std::endl;       // "server-foo"
    std::cout << router2.callRoute("/bar/a/baz") << std::endl; // "server-bar"
    std::cout << router2.callRoute("/bar/x/baz") << std::endl; // "server-bar"
    std::cout << router2.callRoute("/bar/a/b/c") << std::endl; // "NOT_FOUND"


    // =========================================================================
    // Example 3 — Path params (Scale Up 2)
    // =========================================================================
    std::cout << "\n--- Example 3 ---" << std::endl;
    Router router3;
    router3.addRoute("/users/:id", "user-service");
    router3.addRoute("/users/:id/orders/:orderId", "order-service");
    
    std::cout << router3.callRoute("/users/42") << std::endl;          // "user-service"
    std::cout << router3.callRoute("/users/42/orders/7") << std::endl; // "order-service"
    std::cout << router3.callRoute("/users/42/orders") << std::endl;   // "NOT_FOUND"


    // =========================================================================
    // Example 4 — Multiple matches & precedence
    // =========================================================================
    std::cout << "\n--- Example 4 ---" << std::endl;
    Router router4;
    router4.addRoute("/foo/baz", "server-foo");
    router4.addRoute("/foo/*", "server-bar");
    
    std::cout << router4.callRoute("/foo/baz") << std::endl; // "server-foo"
    std::cout << router4.callRoute("/foo/xyz") << std::endl; // "server-bar"


    // =========================================================================
    // Example 5 — Ordered checking tie-break
    // =========================================================================
    std::cout << "\n--- Example 5 ---" << std::endl;
    Router router5;
    router5.addRoute("/a/:x/c", "server-1");
    router5.addRoute("/a/:y/c", "server-2");
    
    std::cout << router5.callRoute("/a/b/c") << std::endl; // "server-1"

    return 0;
}
