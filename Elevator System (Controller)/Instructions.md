23. Design an Elevator System - Request Feasibility (Single Lift)
Asked in
Microsoft
Build the controller for single lift in a building. The building has floorsCount floors numbered 0 to floorsCount-1 .

You have a lift which can take liftCapacity people at a go.

The lift can stop at any number of floors but for any person who is being served by the lift has to reach from entry floor to destination floor within maxStops stops.

A lift will serve a new request only if it can maintain the rule of maxStops stops for people who are already in the lift and also for the person sending the request.

Assume that lift will start only after all requests have arrived. So it remains in IDLE state at floor 0 throughout method calls.
Stop counting rule: For a rider, count the number of distinct floors where the elevator halts after the rider boards, excluding the destination floor and boarding floor. This count must be ≤ maxStops.

PS : This question is more of DSA or DSA based design question but Microsoft is obsessed with elevator system design and asks this and its variants a lot of times in their low level design interview rounds.
Constructor and Methods
1) Constructor: LiftSystem(int floorsCount, int liftCapacity, int maxStops)
Constraints: 2 ≤ floorsCount ≤ 100,
1 ≤ liftCapacity ≤ 20 ,
1 ≤ maxStops ≤ 10.
2) boolean requestPickup( int source, int destination)
Submit a new ride request. Must return accept (true) or reject (false) immediately.
source and destination will never be same.
if source ≤ destination then assume this is a request in UP direction
if source ≥ destination then assume this is a request in DOWN direction
UP direction requests donot clash with DOWN direction requests and vice versa
Examples
Example 1 - maxStops Violation forces rejection
Config: floorsCount = 30, liftCapacity = 3, maxStops = 2

LiftSystem ls = new LiftSystem(30, 3, 2);
ls.requestPickup(0, 20) → true
ls.requestPickup(10, 15) → true
ls.requestPickup(12, 18) → false
Reasoning:

To accept C(12→18), we must include halts 12 and 18, producing at least {10, 12, 15, 18, 20}.
A(0→20): after 0 and before 20 → {10, 12, 15, 18} = 4 > 2 → violates maxStops.
Outcome: Third request rejected to preserve existing riders’ stop limits.

Example 2 - Capacity Rejection (UP) and Independent DOWN pass
Config: floorsCount = 15, liftCapacity = 2, maxStops = 5

LiftSystem ls = new LiftSystem(15, 2, 5);
Method calls:

ls.requestPickup(0, 12) → true (UP)
ls.requestPickup(1, 11) → true (UP)
ls.requestPickup(2, 10) → false (UP; capacity would hit 3 on segment 2..10)
ls.requestPickup(14, 3) → true (DOWN; handled on opposite pass, does not clash with UP)
With first two UP riders, a feasible halt set is {1, 11, 12}.
Adding Z(2→10) would make onboard overlap 3 across 2..10 > capacity 2 → reject.
DOWN rider W(14→3) can be planned on the independent DOWN pass (e.g., halts {9, 6, 3}).
W’s counted stops after 14 and before 3 → {9, 6} = 2 ≤ 5 → OK.
Outcome: Third UP request rejected due to capacity; DOWN request accepted independently per the problem’s UP/DOWN non-clash rule.

