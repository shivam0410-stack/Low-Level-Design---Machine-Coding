10362. Design Hit Counter

Create a system that tracks the number of "clicks" received within the last 300 seconds (5 minutes). Each method will receive a timestamp parameter (represented in seconds).

You can assume that for recordClick(), timestamps are always provided in increasing order. However getRecentClicks() can be called for any timestamp. The earliest timestamp will always be 1.

Note that multiple clicks may be recorded at the same timestamp.

Examples
ClickCounter tracker = new ClickCounter();

register a click at time 1
tracker.recordClick(1);

register a click at time 2
tracker.recordClick(2);

register a click at time 3
tracker.recordClick(3);

retrieve clicks at time 4, expect 3
tracker.getRecentClicks(4);

register a click at time 300
tracker.recordClick(300);

retrieve clicks at time 300, expect 4
tracker.getRecentClicks(300);

retrieve clicks at time 301, expect 3
tracker.getRecentClicks(301);
Constraints
All timestamps are positive integers and start at 1.
Timestamps are provided in a strictly increasing sequence.
At most 10,000 operations will be performed.
There may be multiple clicks at the same timestamp.