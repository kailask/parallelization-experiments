# Parallel Schedules

Comparing different types of work partitioning in DOALL loops with a linear load imbalance between loop iterations.

1. Static work partitioning - Assumes each iteration takes constant amount of time.
2. Dynamic global worklist - Threads collaborate using global worklist to share work. Allows more even distribution of work for imbalanced iterations than static.
3. Dynamic local worklists - Threads complete all work in their local worklists before stealing work from others. Local worklists must be pre-populated. Reduces contention and false sharing of global schedule.

## Setup

### Dependencies

* C++

### Running
