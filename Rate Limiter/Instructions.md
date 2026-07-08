34. Design a rate limiter

Design an in-memory rate limiter . Implement a RateLimiter Class with an isAllowed method.
Requests will be made to different resourceIds. Each resourceId will have a strategy associated with it .
There are following strategies. Assume 1 time unit == 1 second.
1. fixed-window-counter: Fixed Window Counter divides time into fixed blocks (like 1 second) and tracks a request count per block. If the count exceeds the limit, new requests are blocked. It’s fast and simple but can allow burst behavior at window boundaries.

2. sliding-window-counter: Sliding Window (log-based) stores timestamps of recent requests and removes those outside the window for each new request. If the number of remaining requests is still within the limit, the request is allowed. Otherwise, it is blocked. It provides accurate rate limiting but requires more memory and processing.

Implement your solution such that more strategies can be added easily.
Methods
addResource
void addResource(String resourceId, String strategy, String limits)
resourceId will always be non blank and globally unique.
Strategy will be either of "fixed-window-counter" or "sliding-window-counter"
limits is a string of two comma separated integers: maxRequestsCount+","+timePeriod
e.g. limits = "5,2" means maximum 5 requests are allowed every 2 seconds.
maxRequests and timePeriod will both be ≥ 1
If addResource is called again with same resourceId then it will update its strategy and limits.
isAllowed
boolean isAllowed(String resourceId, int timestamp)
resourceId will always be a valid resourceId.
timestamp (seconds) will always be in increasing order across all resources.
Examples
We use a single resource "login-api". First it uses fixed-window-counter, then we update it to sliding-window-counter with a second addResource call.

Step 1 – Configure fixed-window-counter
addResource("login-api", "fixed-window-counter", "2,5")
Allow at most 2 requests every 5 seconds.
(Windows: [0..4], [5..9], [10..14], ...)

Step 2 – First call in window [0..4]
isAllowed("login-api", 1) → true
First request in [0..4] ⇒ count = 1 ≤ 2 ⇒ allowed.

Step 3 – Second call in same window
isAllowed("login-api", 2) → true
Still window [0..4] ⇒ count = 2 ≤ 2 ⇒ allowed (window now full).

Step 4 – Third call in same window (blocked)
isAllowed("login-api", 4) → false
Still window [0..4]. This would be the 3rd request ⇒ 3 > 2 ⇒ blocked.

Step 5 – Update strategy to sliding-window-counter
addResource("login-api", "sliding-window-counter", "2,3")
Now "login-api" allows at most 2 requests in any 3-second sliding window.
Old fixed-window state is replaced by this new strategy.

Step 6 – Sliding window: first request
isAllowed("login-api", 6) → true
Window length = 3. At t = 6 we look at [4..6]. No previous requests for the new config ⇒ count = 1 ≤ 2 ⇒ allowed.

Step 7 – Sliding window: second request
isAllowed("login-api", 7) → true
At t = 7, window [5..7] contains {6, 7} ⇒ count = 2 ≤ 2 ⇒ allowed.

Step 8 – Sliding window: third request (blocked)
isAllowed("login-api", 8) → false
At t = 8, window [6..8] would contain {6, 7, 8} ⇒ count = 3 > 2 ⇒ blocked.