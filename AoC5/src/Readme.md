# First Solution AoC5_P1.cpp
- The solution for the first part of day 5 is implemented in the `AoC5_P1.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5.txt` file.
  2. Insert each interval into the `Tree` using the `buildFromIntervals` method [check this in the Tree implementation documentation](../include/README.md) or if you just want to check the method [check it here](../include/README.md#balanced-build).
  3. For each ID in the input, check if it overlaps with any interval in the tree using the [searchRange](../include/README.md#search-operations) method.
  4. Count and output the number of IDs that overlap with any interval.

# Second Solution AoC5_P2.cpp
- The solution for the second part of day 5 is implemented in the `AoC5_P2.cpp` file. The main steps of the solution are:
  1. Read the input intervals from the `texts/AoC5.txt` file.
  2. Insert each interval into the `Interval Vector`.
  3. Now we have implemented a new method called `countUniqueIDs`, which counts how many IDs from a given range overlap with any interval in the vector.
  
  ## How do countUniqueIDs() works?
    1. First we sort using the same method as in `buildFromIntervals()`.
    2. Then we iterate through the sorted intervals and merge any overlapping intervals into a single interval.
    3. Finally, we calculate the total number of unique IDs covered by the merged intervals.

```cpp
    // Function to count unique IDs from a list of intervals
    long long countUniqueIDs(vector<Interval<long long>>& intervals) {
        if (intervals.empty()) return 0;

        // 1. Sort intervals by start value (same as in buildBalanced)
        sort(intervals.begin(), intervals.end(),
            [](const Interval<long long>& a, const Interval<long long>& b) {
                return a.start < b.start;
            });

        // 2. Merge overlapping intervals, for not double counting
        vector<Interval<long long>> merge;
        merge.push_back(intervals[0]);

        // Here we iterate though sorted intervals and merge them
        for (size_t i = 1; i < intervals.size(); i++) {
            Interval<long long>& last = merge.back();
            const Interval<long long>& current = intervals[i];

            // Check if current overlaps or is adjacent to last
            if (current.start <= last.end + 1) {
                // Merge: extend the end if needed
                last.end = max(last.end, current.end);
            } else {
                // No overlap: add as new interval
                merge.push_back(current);
            }
        }

        // 3. Here we count total unique IDs
        long long total = 0;
        for (const Interval<long long>& interval : merge) {
            total += (interval.end - interval.start + 1);
        }

        return total;
    }
``` 