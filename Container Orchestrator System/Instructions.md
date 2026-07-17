28. Design a Container Orchestrator System
Asked in
Microsoft
Build an in-memory container orchestrator with multiple cloud server machines (like amazon EC2, Azure virtual machines etc).

Orchestrator assigns machines on which containers will run. Each machine can start, stop and manage multiple containers running concurrently .
The system will manage machine resources i.e. CPU units, memory MB .
Each machine's capacity is provided at initialization: total CPU units and total memory in MB.
Containers have: name (unique), imageUrl, cpuUnits, memoryMB
Resource reservation :
While RUNNING or PAUSED: Container reserves CPU, memory, and all bound host ports.
While STOPPED: Container reserves nothing (compute and memory are freed).
Constructor & Methods
1) Constructor: ContainerManager(List <String> machines)
It takes a list of machine metadata
Each row is of the format "machineId,totalCpuUnits,totalMemoryInMB"
machineId will always be globally unique and non blank.
0 < totalCpuUnits for each row
0 < totalMemoryInMB for each machine
2)  String assignMachine(int criteria, String containerName, String imageUrl, int cpuUnits, int memMb)
This method returns the assigned machineId on which this container will be hosted or it returns empty string if no container can be assigned.
criteria will decide the algorithm based on which machine will be selected.
If criteria ==0 : Choose machine with maximum amount of spare CPU units left which is not used by any other container.
If criteria ==1 : Choose machine with maximum amount of spare memory left which is not used by any other container.
If multiple machines fit the criteria, then choose the one with lexicographically smallest machineId.
free CPU units on chosen machine ≥ cpuUnits and free memory on machine ≥ memMb
i.e. sufficient free cpu and memory units must be available on the machine.
Validations:
containerName will always be be unique and non-blank.
cpuUnits > 0, memMb > 0.
3) boolean stop(String name)
Transitions a container from RUNNING to STOPPED.
Frees its CPU/memory and unbinds all ports.
Returns true if the container is assigned to some machine and was not already STOPPED, else false.
Examples
Common Set-up (for all scenarios) Constructor:

ContainerManager([
  "mA,8,16000",
  "mB,8,8000",
  "mC,4,32000"
])
mA: 8 CPU, 16000 MB
mB: 8 CPU, 8000 MB
mC: 4 CPU, 32000 MB
Scenario 1 - CPU-first (criteria=0) with tie-break, then another placement
Call: assignMachine(0, "c1", "img://a", 2, 2000)

Why: Free CPU is mA=8, mB=8, mC=4 → max=8; tie between mA and mB → pick lexicographically smallest → mA.

Expected return: "mA"

Post-state: mA free: CPU 6, Mem 14000; mB free: CPU 8, Mem 8000; mC free: CPU 4, Mem 32000.


Call: assignMachine(0, "c2", "img://b", 6, 1000)

Why: Free CPU now mA=6, mB=8, mC=4 → pick mB.

Expected return: "mB"

Post-state: mA: CPU 6, Mem 14000; mB: CPU 2, Mem 7000; mC: CPU 4, Mem 32000.


Call: assignMachine(0, "c3", "img://c", 3, 1000)

Why: Free CPU mA=6, mB=2, mC=4 → pick mA.

Expected return: "mA"

Post-state: mA: CPU 3, Mem 13000; mB: CPU 2, Mem 7000; mC: CPU 4, Mem 32000.


Scenario 2 - Memory-first (criteria=1) and a failure due to insufficient memory
Call: assignMachine(1, "c4", "img://d", 2, 9000)

Why: Free Mem mA=13000, mB=7000, mC=32000 → pick mC.

Expected return: "mC"

Post-state: mC: CPU 2, Mem 23000 (after reserving 2 CPU and 9000 MB).


Call: assignMachine(1, "c5", "img://e", 2, 25000)

Why: Free Mem mA=13000, mB=7000, mC=23000 → none has ≥ 25000 MB.

Expected return: "" (empty string)


Scenario 3 - stop(...) frees resources; retry succeeds
Call: stop("c4")

Effect: c4 was RUNNING on mC → now STOPPED; CPU and Mem released.

Expected return: true

Post-state: mC back to CPU 4, Mem 32000.


Call: assignMachine(1, "c5", "img://e", 2, 25000)

Why: Free Mem now mA=13000, mB=7000, mC=32000 → pick mC.

Expected return: "mC"


Call: stop("doesNotExist") → Expected: false

Call: stop("c4") again (already STOPPED) → Expected: false