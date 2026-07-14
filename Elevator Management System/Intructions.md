24. Design Elevator Management System - Single Lift
Asked in
Microsoft
Adobe
Oracle
Amazon LLD
Salesforce LLD
MakeMyTrip
Alteryx
Tekion
PayPal
Remitly
Quince
Indihood
Coupa
+9 more
Write code for low level design of a simple elevator management system consisting of a single lift.
All the lifts are in the same building which has multiple floors.

Lifts are numbered 0 to lifts-1 and floors are numbered 0 to floors -1.
Each lift can carry at max liftsCapacity number of people at a single time.

Assumptions
- For simplicity lets assume that each lift takes exactly one tick() units of time (see tick() method below) to go to next floor while coming up or going down.
Also, when a lift stops on a floor, then time taken for one or more people to come in or go out of lift is zero (or 0 tick()).

Lift will be IDLE at ground floor i.e. 0th floor in the beginning.

When all passengers go out of the lift on a floor and there is no remaining requests in any direction then lift will remain idle on that floor.

You have to code below methods.
Constructor : LiftSystem(int floors, int liftsCapacity) :
- Initialize/reinitialize the elevator system and reset all your instance variables.
- liftsCapacity is the maximum number of people a lift can carry at any time
METHOD : boolean addRequest(int startFloor, int destinationFloor)
- startFloor is the floor user is currently at.
- destinationFloor is the floor user wants to reach.
- both start and destination floor will be from 0 to liftsCount-1
- startFloor will never be equal to destination floor.
- A request can be accepted only when there will be atleast 1 space left on the startFloor when request maker will board the lift.
- Your code should return true if lift can serve this request and false otherwise.
- Even when addRequest() returns true, person will board the lift only when lift reaches their floor in required travel direction.
  e.g. if a person wants to go from floor 4 to floor 1, lift is at floor 4 but it is serving up requests,
  then it will first serve all up requests and then it will come back and only then the person will board it.

ELIGIBILITY : Below are the reasons as to when a lift can't be assigned to a request. These are intuitive and are followed by most elevator systems.

Case 1: Lift has already passed the user e.g addRequest(2, 5) i.e. user is requesting to go up from floor 2 and lift is also going up and is at floor 4 right now.
Case 2: Going down: addRequest(18,12) i.e. User has requested to go down from floor 18 and lift is also going down and is at floor 16 right now, so it has passed floor 18.
In both above cases given lift can't be assigned to requests.
METHOD : String getLiftState()
- returns a string representing current floor, direction and number of people inside lift.
- directions: U for going up, D for going down and I for standing idle.
- e.g. "4-U-2" : lift is at Floor 4 going Up and has 2 people
Note:
One can't change move direction of lift i.e.
lets assume a lift with liftCapacity =5 , is at floor 3, moving UP and it is serving below requests (startFloor, destinationFloor)
- current requests: { (1, 4) , (2, 1) }
tick() -> lift is now at floor 4 , request remaining (2, 1) , direction immediately becomes D (down)
addRequest(6, 1) -> returns false, because lift is now at floor 4 , going down, so it has already passed floor 6.
addRequest(3, 1) -> returns true.
addRequest(4, 8) -> returns true. Although lift's is at floor 4, its direction is D (down),
so it will first go down and serve all down requests and then come back up and serve this request.
addRequest(6, 9) -> returns true. Same case, even though lift has passed floor 6 while coming down, but this is a request in opposite direction. So it will go in opposite queue.
currentFloor-direction-peopleCount
0 ≤ currentFloor ≤ this.floors-1
direction : U for up, D for down, I for idle

"4-U-10": lift is at floor 4, going UP and has 10 people inside,
"12-D-2": lift is at floor 12, going DOWN and has 2 people inside,
"0-I-0": lift is at floor 0, standing IDLE and has no one inside
 
METHOD : tick()
This method denotes 1 unit of time passed, it is called so that you can appropriately update lift states

Note :
You can practice this question in Java or Python
This question will be tested in a SINGLE THREADED environment.
2 ≤ floors ≤ 200
1 ≤ liftsCapacity ≤ 20
Examples
Example 1 : Already Passed While Going Up (Reject)
Config: floors = 10, liftsCapacity = 4

LiftSystem ls = new LiftSystem(10, 4);

ls.getLiftState();     // "0-I-0"  (starts idle at 0)

ls.addRequest(0, 7);   // true (at 0, idle -> sets direction Up; rider boards immediately)
ls.getLiftState();     // "0-U-1"

ls.tick();             // -> 1
ls.tick();             // -> 2
ls.tick();             // -> 3
ls.tick();             // -> 4
ls.getLiftState();     // "4-U-1"  (still carrying the 0→7 rider)

ls.addRequest(2, 5);   // false  (already passed 2 while going Up)
Outcome: Second request is rejected because the car, moving Up at floor 4, has already passed floor 2.

Example 2 : Already Passed While Going Down (Reject)
Config: floors = 8, liftsCapacity = 3

LiftSystem ls = new LiftSystem(8, 3);

ls.addRequest(0, 4);      // true (boards at 0; direction becomes Up)
ls.getLiftState();        // "0-U-1"

// Move Up to 4 (no loop; few ticks)
ls.tick();                // -> 1
ls.tick();                // -> 2
ls.tick();                // -> 3
ls.tick();                // -> 4  (arrives; 0→4 alights instantly)
ls.getLiftState();        // "4-I-0"  (no pending requests -> idle)

ls.addRequest(3, 1);      // true (Down request). Car at 4 sets direction Down to pick at 3.
ls.getLiftState();        // "4-D-0"  (not yet at 3, so nobody onboard yet)

ls.tick();                // -> 3 (arrives at start of 3→1 in Down direction)
ls.getLiftState();        // "3-D-1"  (rider boards now at 3)

// While going Down from 3, any Down request from a floor ABOVE (e.g., 6) is already passed.
ls.addRequest(6, 2);      // false (already passed 6 while going Down)
Outcome: The (6 → 2) request is rejected because the car, now moving Down from 3, has already passed floor 6.

Example 3 - Direction Consistency + Deferred Opposite Direction
Config: floors = 12, liftsCapacity = 3

LiftSystem ls = new LiftSystem(12, 3);

ls.addRequest(1, 4);   // true (Up)
ls.addRequest(2, 1);   // true (Down; will be served later)
ls.getLiftState();     // "0-U-0"  (moving Up to pick at 1)

ls.tick();             // -> 1 (arrives Up; 1→4 rider boards)
ls.getLiftState();     // "1-U-1"

ls.tick();             // -> 2 (passes 2 going Up; 2→1 is Down, so doesn't board now)
ls.tick();             // -> 3
ls.tick();             // -> 4 (arrives; 1→4 alights instantly; Down requests remain)
ls.getLiftState();     // "4-D-0"  (direction flips to Down to serve 2→1; no one onboard yet)

ls.addRequest(6, 1);   // false (already passed 6 while going Down)
ls.addRequest(3, 1);   // true  (not yet passed 3 while going Down)
ls.addRequest(4, 8);   // true  (Up request accepted and deferred to the next Up pass)
Outcome: (6 → 1) rejected; (3 → 1) accepted on current Down pass; (4 → 8) accepted and deferred until the car returns Up.