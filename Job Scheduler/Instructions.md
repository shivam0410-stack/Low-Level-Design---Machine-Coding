22. Design a Job Scheduler
Asked in
Microsoft
Salesforce LLD
Design a scheduler for a massively parallel distributed system. The scheduler assigns incoming jobs to machines that it controls.
Each machine has a set of capabilities. Each job requires a set of required capabilities. A job may only run on a machine that has all required capabilities.
Methods
1) void addMachine(String machineId, String capabilities[])
machineId will always be unique and non-blank.
capabilities list of unique and non blank tokens, e.g.
pdf thumbnail creator
plain text compression
image compression
video thumbnail generation
save file as chunks to file storage
find duplicate chunk
audio extraction
speech to text conversion
capabilities are case insensitive . Treat both upper and lowercase characters as same.
2) String assignMachineToJob(String jobId, String capabilitiesRequired[], int criteria)
Returns the selected machineId, or an empty string "" if no compatible machine exists.
If multiple machines exist then return machine with lexicographically smallest machineId among them.
All values in capabilitiesRequired are valid and non blank.
jobId is unique and non-blank.
criteria chooses the machine selection algorithm:
0: Find machine with Least number of unfinished jobs.
1: Find machine with Most number of finished jobs.
Extensibility requirement: Your solution must make it easy to add new assignment criteria (algorithms).
3) void jobCompleted(String jobId)
jobId is always valid and correspond to a previously submitted and assigned job.
Marks the job as finished and updates the associated machine’s counters.
Example 1 - Multi-Capability Match + Criteria 0 (Least Unfinished) + Deterministic Tie
addMachine("m-10", ["image compression", "audio extraction", "video thumbnail generation"])
addMachine("m-2", ["image compression", "audio extraction"])
assignMachineToJob("job-A", ["image compression", "audio extraction"], 0)
Candidates: m-2, m-10 (both are supersets of required capabilities)
Both have unfinishedCount = 0 → tie → choose lexicographically smaller m-10 vs m-2: “m-10” is lexicographically smaller than “m-2” because it compares character-by-character and '1' < '2'.
Result: returns "m-10"
State: m-10.unfinished = 1, m-10.finished = 0; job-A → ASSIGNED to m-10
Example 2 - Completion Updates → Criteria 1 (Most Finished)
jobCompleted("job-A") → m-10.unfinished: 1 → 0, m-10.finished: 0 → 1
assignMachineToJob("job-B", ["image compression"], 1)
Candidates: m-2, m-10 (both have “image compression”)
Finished counts: m-10 = 1, m-2 = 0 → choose m-10
Result: returns "m-10"
State: m-10.unfinished: 0 → 1
Example 3 - No Compatible Machine → Empty String
assignMachineToJob("job-C", ["speech to text conversion"], 0)
Candidates: none (no machine provides “speech to text conversion”)
Result: returns "" (empty string); no job or counters updated
Constraints
There will be at max 100 machines.
There will be at max 100 different capabilities.