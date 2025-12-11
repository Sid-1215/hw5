#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShifts,
    size_t day,
    size_t slot
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    // setup the schedule with empty rows for each day
    size_t numDays = avail.size();
    for (size_t i = 0; i < numDays; i++) {
        sched.push_back(vector<Worker_T>());
    }
    
    // track how many shifts each worker has been assigned
    size_t numWorkers = avail[0].size();
    vector<size_t> workerShifts(numWorkers, 0);
    
    // start recursive backtracking from day 0, slot 0
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, 0, 0);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<size_t>& workerShifts,
    size_t day,
    size_t slot
)
{
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();
    
    // base case - filled all days successfully
    if (day == numDays) {
        return true;
    }
    
    // if we filled all slots for this day, move to next day
    if (slot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day + 1, 0);
    }
    
    // try assigning each worker to this slot
    for (size_t worker = 0; worker < numWorkers; worker++) {
        // check if this worker is available on this day
        if (!avail[day][worker]) {
            continue;
        }
        
        // check if this worker has already hit max shifts
        if (workerShifts[worker] >= maxShifts) {
            continue;
        }
        
        // check if this worker is already scheduled for this day
        bool alreadyScheduled = false;
        for (size_t i = 0; i < sched[day].size(); i++) {
            if (sched[day][i] == worker) {
                alreadyScheduled = true;
                break;
            }
        }
        if (alreadyScheduled) {
            continue;
        }
        
        // try scheduling this worker
        sched[day].push_back(worker);
        workerShifts[worker]++;
        
        // recurse to fill the next slot
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, workerShifts, day, slot + 1)) {
            return true;
        }
        
        // backtrack - didn't work out
        sched[day].pop_back();
        workerShifts[worker]--;
    }
    
    // couldn't find a valid worker for this slot
    return false;
}