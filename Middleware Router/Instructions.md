36. Design a Middleware Router
Asked in
Atlassian
Design an in-memory middleware router for a web service. The router stores path patterns mapped to a result string (think: server id). When a request path comes in, the router returns the result of the best matching route.
You are building a lightweight routing layer similar to how web frameworks match URLs to handlers.
Implement a Router that supports route patterns as explained below.

A path is a string starting with / and segmented by /. Example: /bar/a/baz has segments ["bar","a","baz"].
A route pattern is a path that may contain:
Static segments, e.g. /foo/baz
Wildcard segment * that matches exactly one segment (not empty, no slashes). Example: /bar/*/baz matches /bar/a/baz and /bar/123/baz, but not /bar/a/b/c/baz.
Path param segment starting with :, e.g. /users/:id. It matches exactly one segment too, and the segment value is captured as a parameter.
Matching & Precedence Rules
When callRoute(path) is invoked, multiple patterns may match. Choose the winner using:

Specificity first: A pattern with more static segments is more specific. (Static segment beats : param, which beats * wildcard.)
Longer pattern wins: If two patterns have the same static/param/wildcard makeup, the one with more segments is more specific.
Ordered checking tie-break: If still tied, the route that was added earlier wins.
If no routes match, return "NOT_FOUND".

Methods
constructor
Router()
Creates an empty router.
addRoute
void addRoute(String pathPattern, String result)
pathPattern is non-blank and starts with /.
result is a non-blank string (server id to return).
Adding the same pathPattern again updates its result but preserves original insertion order.
callRoute
String callRoute(String path)
path is a concrete request path (no wildcards/params).
Returns the result of the best matching pattern.
Returns "NOT_FOUND" if nothing matches.
searchRoutes
List<String> searchRoutes(String wildcardPattern)
Allows querying stored routes using a wildcard pattern, returning all results whose patterns match that query.

Examples
Example 1 — Basic static route

Router router = new Router();
router.addRoute("/bar", "server-1");

router.callRoute("/bar");        // "server-1"
router.callRoute("/bar/xyz");    // "NOT_FOUND"
    
Examples — Testing only searchRoutes

// Create a fresh router
Router router = new Router();

// Add a mix of patterns
router.addRoute("/bar/*/baz", "server-wild");     // wildcard
router.addRoute("/bar/:x/baz", "server-param");  // param
router.addRoute("/bar/a/baz", "server-static");  // fully static
router.addRoute("/bar/*/qux", "server-qux");     // different last segment

// Update an existing pattern (order stays where it was, result changes)
router.addRoute("/bar/*/baz", "server-wild-v2");

 1) Wildcard query matches multiple stored patterns.
 Returned in insertion order (not by precedence).
router.searchRoutes("/bar/*/baz");
returns ["server-wild-v2", "server-param", "server-static"]

 2) Fully static query still matches param/wildcard patterns
 that could match the same concrete path.
router.searchRoutes("/bar/a/baz");
returns ["server-wild-v2", "server-param", "server-static"]

 3) Query with same length but different static segment.
router.searchRoutes("/bar/*/qux");
returns ["server-qux"]

 4) Length mismatch => no matches.
router.searchRoutes("/bar/*/a/baz");
returns []

 5) No compatible route at all.
router.searchRoutes("/nope/*");
returns []
Example 2 — Wildcards (Scale Up 1)

Router router = new Router();
router.addRoute("/foo", "server-foo");
router.addRoute("/bar/*/baz", "server-bar");

router.callRoute("/foo");        // "server-foo"
router.callRoute("/bar/a/baz");  // "server-bar"
router.callRoute("/bar/x/baz");  // "server-bar"
router.callRoute("/bar/a/b/c");  // "NOT_FOUND"  (pattern length mismatch)
    
Example 3 — Path params (Scale Up 2)

Router router = new Router();
router.addRoute("/users/:id", "user-service");
router.addRoute("/users/:id/orders/:orderId", "order-service");

router.callRoute("/users/42");                 // "user-service"
router.callRoute("/users/42/orders/7");        // "order-service"
router.callRoute("/users/42/orders");          // "NOT_FOUND"
    
Example 4 — Multiple matches & precedence

Router router = new Router();
router.addRoute("/foo/baz", "server-foo");
router.addRoute("/foo/*", "server-bar");

// Both patterns match "/foo/baz".
// "/foo/baz" is more specific (static beats wildcard),
// so it wins regardless of insertion order.
router.callRoute("/foo/baz");    // "server-foo"
router.callRoute("/foo/xyz");    // "server-bar"
    
Example 5 — Ordered checking tie-break

Router router = new Router();
router.addRoute("/a/:x/c", "server-1");
router.addRoute("/a/:y/c", "server-2");

// Both are equally specific and same length.
// Earlier added wins.
router.callRoute("/a/b/c");      // "server-1"
    
