#ifndef NANOQUANT_TASKTRACKER_H
#define NANOQUANT_TASKTRACKER_H

namespace nq {

    struct TaskTracker {
        static int diagnosticCount;

        TaskTracker() {
            ++diagnosticCount;
        }
        ~TaskTracker() {
            --diagnosticCount;
        }
    };

} // namespace nq

#endif //NANOQUANT_TASKTRACKER_H
