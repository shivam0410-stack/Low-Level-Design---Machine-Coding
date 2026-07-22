35. Design a customer support agent rating leaderboard
Asked in
Atlassian
Design an in-memory component to display Customer support agent rating leaderboard. Each interaction is rated on a scale of 1 to 5.
Implement a system that:

Records a rating for a given agent on a given date.
Returns all agents with their overall average ratings, sorted from highest to lowest.
Returns best agents for a given month (YYYY-MM) based on their average ratings.
Constraints
rating is an integer between 1 and 5.
agentName is a non-empty, case-sensitive string.
date is in YYYY-MM-DD format; month is YYYY-MM.
System should handle thousands of agents and millions of ratings efficiently.
Average ratings must be rounded to single decimal point,
4.33 becomes 4.3, 4.35 or 4.37 becomes 4.4
Methods
rateAgent
void rateAgent(String agentName, int rating, String date)
agentName: non-blank agent identifier.
rating: integer in [1, 5].
date: rating date in "YYYY-MM-DD" format.
Each call records a single rating event for that agent on that date.
getAverageRatings
List<String> getAverageRatings()
Returns overall average rating per agent.
Each entry is in the format "agentName,averageRating" (e.g., "Bob,4.5").
Sorted in descending order of averageRating.
If two agents have the same average rating, order them by agent name in lexicographically ascending order.
getBestAgentsByMonth
List<String> getBestAgentsByMonth(String month)
month: month string in "YYYY-MM" format (e.g., "2025-03").
Considers only ratings whose date falls in the given month.
Each entry is in the format "agentName,averageRating".
Sorted in descending order of averageRating for that month.
If two agents have the same average rating, order them by agent name in lexicographically ascending order.
Examples
Example 1 – Overall Average Ratings
Input:

rateAgent("Alice", 5, "2025-03-12")
rateAgent("Bob", 4, "2025-03-13")
rateAgent("Alice", 3, "2025-03-15")
rateAgent("Bob", 5, "2025-03-18")
getAverageRatings()

Output:

[
  "Bob,4.5",
  "Alice,4.0"
]

Explanation:
Bob → (4 + 5) / 2 = 4.5
Alice → (5 + 3) / 2 = 4.0
Sorted from highest to lowest average rating.

Example 2 – Best Agents by Month
Input:

rateAgent("Alice", 5, "2025-02-02")
rateAgent("Bob", 3, "2025-02-05")
rateAgent("Charlie", 4, "2025-02-10")
rateAgent("Bob", 5, "2025-03-12")
rateAgent("Alice", 2, "2025-03-15")
getBestAgentsByMonth("2025-02")

Output:

[
  "Alice,5.0",
  "Charlie,4.0",
  "Bob,3.0"
]

Explanation:
Only ratings with dates in February 2025 are considered.
Agents are ordered from highest to lowest average rating for that month.