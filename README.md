*This project has been created as part of the 42 curriculum by [alcristo](https://github.com/alcristo)*

# Codexion

## Description

This project consists of a multiple thread program based on Dijkstra's *dining philosophers problem*, where 5 philosohers sit on a round table and have to eat spaghetti while sharing forks. In this version, a certain number of coders (philosophers) sit around a quantum compiler (round table) and need to grab two dongles (forks/chopsticks) to compile (eat). Then they start debugging and refactoring (sleeping/thinking). If a coder doesn't compile since the last time, they burn out (die of starvation).

For this project a priority queue had to be implemented, as well as dongle cooldown, the amount of time during which will not be available after it's released.

## Thread synchronization mechanisms

Thread synchronization is crucial for the dining philosophers problem. If that weren't the case, a thread may write a variable value while another thread is reading it, making a serious bug defined as a *data race*. The C POSIX thread library *pthread.h* has two useful synchronization primitives: mutexes and condition variables.

### Mutexes

A mutex is essencilly a lock that protects variables when they are read and written. When a thread locks a mutex such thread holds the mutex, becoming its owner. While the mutex is being held, all other threads wait until the mutex is released.

In this project, the most used mutex is the simulation mutex, which is used by the monitor thread to stop the simulation when all coders have finished or when one of them has burned out. In both cases, the monitor locks the simulation mutex, sets the `stop` flag and unlocks that mutex. The rest of the threads lock that same mutex to read the`stop` flag and unlock it afterwards. Therefore, data races are prevented.

### Condition variables

Condition variables are variables in which threads wait until they're told to continue with their routine. Condition variables are normally associated to a mutex. When a thread reaches a condition variable while holding that mutex, it releases the mutex and waits on the condition variable. When another thread broadcasts a signal to that condition variable, all threads waiting on it try to grab the associated mutex and keep their flow. The waiting act on condition variables is usually inside a while loop conditioning one of the variables associated with the mutex. For example:

```c
pthread_mutex_lock(&mutex);
while (variable != "get out of the loop")
    pthread_cond_wait(&cond, &mutex);
pthread_mutex_unlock(&mutex);
```

In this project, all threads wait on a the simulation condition variable when holding the simulation mutex after being created. Then the simulation broadcasts a signal to its condition variable, so all threads start at the same time. All the threads are waiting while the simulation has not started and has not stopped due to a failed thread creation.

Another condition variable is used by the waiter to give permission to a coder to start compiling. After sending the request, the coder thread waits on its condition variable while holding their `go` mutex, waiting for the waiter's permission. The thread is waiting for an amount of time. If the time reaches the deadline, the waiting coder will acquaire its mutex and send a burnout log to the logger, making it stop printing logs.

## Blocking cases handled

### Deadlock prevention: Coffman's mechanisms

A deadlock, also known as a *lock-order inversion*, is a case in which a thread is holding resources while waiting for other resources that are held by other threads that need the resources held by the first thread. In the classic dining philosophers problem, a deadlock is illustrated as all philosophers grabbing the chopstick on their right hand while waiting for the left one to be available.

Deadlocks happen if all four Coffman's conditions are true:

1. Mutual exclusion: Multiple processes can't use the same resource at the same time.
2. Resource holding: When a thread has grabbed a resource, it doesn't release it until its job has been done.
3. No preemption: No process can take a resource to any other process.
4. Circular wait: Each process must wait for the resources that are holding other processes.

In practically all problems, negating mutual exclusion is unreliable because it would make data races, while allowing preemption may corrupt the data a certain process is writing. Negating resource holding usually means that the threads can actually communicate with each other, but the project doesn't allow that. Therefore, the solution to prevent deadlocks is the negation of circular waiting.

This project negates circular waiting by letting the odd coders compile first, while even coders wait until all resources are available. This way the waiter marks the dongles as reserved tells the coders to take both dongles at the same time.

### Starvation prevention

Starvation happens when a thread has its resources denied for a large amount of time. To prevent it, a EDF (earliest deadline first) heap was implemented. Permission will be given to coders with the earliest deadline, then to coders who have compiled the least, and finally to the coder with highest ID.

### Cooldown handling

When a dongle is released, its time to be cool is calculated before the coder unlocks the dongle mutex. When a coder requests such dongle, the waiter will check if it's reserved and if the current time has surpassed the cooldown timestamp, and allow its use if so.

### Precise burnout detection

### Log serialization

## Instructions

## Resources

### AI usage

AI was used for learning the key concepts of this project and debugging during the first steps of the code.
