72. Design a Kafka like Message Streaming Service with Multiple Topics

Design a working MVP for an in-memory message streaming service supporting multiple topics, producers, and consumers. Ensure that the messages maintain order within partitions.
Core Requirements
Multiple topics: Each topic is created with a fixed number of partitions.
Multiple producers: Producers can publish messages to any topic partition.
Multiple consumers: Multiple consumers can read messages. Each consumer maintains its own read position.
Ordering guarantee: Messages must be delivered in the same order they were published within a partition.
Topic
A topic has a unique name and a fixed partitionCount.
Topics are independent from each other.
Partition
Each topic has partitions numbered from 0 to partitionCount - 1.
Each partition is an append-only log of messages.
Each published message is assigned a monotonically increasing offset within its partition, starting at 0.
Consumer Cursor
For each (topicName, consumerId, partitionId), the service stores the next offset the consumer should read.
Different consumerId values are independent and can read the same partition from the beginning.
Ordering Guarantees
Publish order: If two messages are published to the same (topicName, partitionId), their assigned offsets must reflect the publish order.
Consume order: A consumer must receive messages from a partition strictly in increasing offset order.
If no new messages exist at the consumer's cursor, consume(...) will return empty list.
Method Signatures
1) Create a Topic
 boolean createTopic(String topicName, int partitionCount) 
Creates a new topic with partitionCount partitions.
Returns true if the topic was created, returns false if topicName already exists.
partitionCount ≥ 1
2) Publish a Message
 String publish(String topicName, int partitionId, String message) 
Appends message to the specified partition of the topic.
Returns a message identifier in the form "p<partitionId>:<offset>" (for example, "p0:12").
0 ≤ partitionId < partitionCount(topicName)
3) Consume Messages
 List<String> consume(String topicName, String consumerId, int partitionId, int maxMessages) 
Returns up to maxMessages messages from the consumer's current cursor for that partition.
If messages are available, returns immediately with up to maxMessages in partition order.
If no messages are available then returns an empty list.
maxMessages ≥ 1
Consumer cursor advances only for that (topicName, consumerId, partitionId).
Constraints
1 ≤ partitionCount ≤ 100
1 ≤ topicName.length() ≤ 100
topicName is non-empty and should be treated as case-sensitive.
1 ≤ message.length() ≤ 10_000
1 ≤ maxMessages ≤ 10_000
Assume the service is in-memory only (no persistence required).
Ordering is required only within a partition, no ordering is required across different partitions.
Examples
Example 1: Basic Topic Creation, Publish, and Consume
createTopic(topicName="payments", partitionCount=2) → true
publish(topicName="payments", partitionId=0, message="pay#A") → "p0:0"
publish(topicName="payments", partitionId=0, message="pay#B") → "p0:1"
publish(topicName="payments", partitionId=1, message="pay#C") → "p1:0"

consume(topicName="payments", consumerId="c1", partitionId=0, maxMessages=10) → List.of("pay#A", "pay#B")
consume(topicName="payments", consumerId="c1", partitionId=0, maxMessages=10) → List.of()  (no new messages at c1 cursor for partition 0)
consume(topicName="payments", consumerId="c1", partitionId=1, maxMessages=10) → List.of("pay#C")
consume(topicName="payments", consumerId="c1", partitionId=1, maxMessages=10) → List.of()  (cursor has reached the end of partition 1)
Example 2: Multiple Consumers Have Independent Cursors
createTopic(topicName="orders", partitionCount=1) → true
publish(topicName="orders", partitionId=0, message="o1") → "p0:0"
publish(topicName="orders", partitionId=0, message="o2") → "p0:1"

consume(topicName="orders", consumerId="c1", partitionId=0, maxMessages=1) → List.of("o1")
consume(topicName="orders", consumerId="c1", partitionId=0, maxMessages=5) → List.of("o2")
consume(topicName="orders", consumerId="c1", partitionId=0, maxMessages=5) → List.of()  (c1 has consumed all available messages)

consume(topicName="orders", consumerId="c2", partitionId=0, maxMessages=10) → List.of("o1", "o2")  (c2 starts from the beginning because its cursor is independent)
consume(topicName="orders", consumerId="c2", partitionId=0, maxMessages=10) → List.of()  (no new messages for c2 after consuming offsets 0 and 1)
Example 3: maxMessages Limits the Returned Batch Size
createTopic(topicName="metrics", partitionCount=1) → true
publish(topicName="metrics", partitionId=0, message="cpu=90") → "p0:0"
publish(topicName="metrics", partitionId=0, message="mem=70") → "p0:1"
publish(topicName="metrics", partitionId=0, message="disk=40") → "p0:2"

consume(topicName="metrics", consumerId="c1", partitionId=0, maxMessages=2) → List.of("cpu=90", "mem=70")  (returns only 2 messages due to maxMessages=2)
consume(topicName="metrics", consumerId="c1", partitionId=0, maxMessages=2) → List.of("disk=40")  (continues from the next offset)
consume(topicName="metrics", consumerId="c1", partitionId=0, maxMessages=10) → List.of()  (no new messages available)
