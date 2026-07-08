33. Design a Single-Queue Publish Subscribe System

Design an in-memory publish/subscribe system with exactly one global FIFO (first in first out) queue.
Multiple publishers can publish messages to this queue. Many subscribers can subscribe to the same queue.
When a message is appended, all subscribers are notified, and each subscriber consumes at its own pace.
A subscriber can consume only those messages which were sent while it was subscribed.
PS: Use the Observer pattern (Queue Manager = Subject, Subscribers = Observers).

Scope & Constraints
In-memory only, no persistence or external systems.
Exactly one global queue (no multiple topics).
Subscribers filter by eventType (only process configured types, ignore others).
Methods
addSubscriber
void addSubscriber(String subscriberId, List[String] eventTypesToProcess)
Registers a subscriber with a non-blank subscriberId.
The subscriber will process a message only if its eventType is in eventTypesToProcess, other types are ignored.
If a subscriber is removed and added again then events in its older eventTypesToProcess list will be replaced by events from new eventTypesToProcess list.
removeSubscriber
void removeSubscriber(String subscriberId)
If an active subscriber with this subscriberId exists, unsubscribe it.
After removal, the subscriber stops receiving and processing further messages.
A removed subscriber may be added again (with same subscriberId) later via addSubscriber
sendMessage
void sendMessage(String eventType, String message)
Appends the message (with its eventType) to the global FIFO queue.
Notifies all currently subscribed subscribers.
Each subscriber consumes at its own pace, processing only if eventType exists in their eventTypesToProcess list.
countProcessedMessages
int countProcessedMessages(String subscriberId)
Returns the total number of messages that this subscriber has actually processed so far.
If subscriber has subscribed -> removed -> subscribed again, then add all their past processed message counts as well.
Only messages with matching eventType count as processed, ignored types do not increment this count.
Examples
sendMessage("ORDER", "pre-1")
No subscribers yet ⇒ message is appended to the global FIFO queue, nobody processes it (no retroactive delivery).

addSubscriber("S1", ["ORDER","PAYMENT"])
addSubscriber("S2", ["PAYMENT"])
Register two subscribers with overlapping but not identical filters.

sendMessage("ORDER", "o-1")
S1 processes (filter matches). S2 ignores (ORDER not in its filter). Demonstrates filtering.

addSubscriber("S3", ["SHIPMENT"])
Late joiner: S3 will only see messages from now on.

sendMessage("PAYMENT", "p-1")
S1 and S2 both process (both filter PAYMENT), S3 ignores.

sendMessage("SHIPMENT", "s-1")
S3 processes (matches filter, arrived after S3 subscribed). S1/S2 ignore (type not in filters).

removeSubscriber("S2")
Unsubscribing immediately stops further processing for S2.

sendMessage("PAYMENT", "p-2")
Only S1 processes (S2 is unsubscribed, S3 ignores). Demonstrates unsubscribe stops processing.

removeSubscriber("S1")
S1 also unsubscribes.

sendMessage("ORDER", "o-2")
Active subscribers: only S3 (but filter is SHIPMENT). Therefore nobody processes this message. Demonstrates “message with no eligible subscribers”.

addSubscriber("S1", ["ORDER","REFUND"])
Re-subscribe the same subscriberId with an expanded filter. Processed counts must accumulate across sessions.

sendMessage("REFUND", "r-1")
S1 processes (new filter covers REFUND). S3 ignores.

countProcessedMessages("S1") → 4
S1 processed: o-1, p-1, p-2, r-1 ⇒ cumulative across unsubscribe/resubscribe.

countProcessedMessages("S2") → 1
S2 processed only p-1 (was unsubscribed before p-2).

countProcessedMessages("S3") → 1
S3 processed only s-1 (late subscriber, nothing retroactive, filters enforced).