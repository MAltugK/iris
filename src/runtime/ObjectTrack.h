#pragma once

#include <map>
#include <pthread.h>
#include "Debug.h"

using namespace std;

namespace iris {
    namespace rt {
        class ObjectTrack
        {
            public:
                ObjectTrack( ) {
                    freed_objects = 0;
                    pthread_mutex_init(&track_lock_, NULL);
                }
                ~ObjectTrack() {
                    pthread_mutex_destroy(&track_lock_);
                }
                bool IsObjectExists(void *p, unsigned long uid) { 
                    bool flag = false;
                    if (allocated_objects_.find(p) != allocated_objects_.end()) 
                        flag = allocated_objects_[p];
                        flag = flag & (allocated_objects_[p] == uid);
                    _trace("object:%p exists flag:%d", p, flag);
                    return flag;
                }
                void UntrackObject(void *p) {
                    pthread_mutex_lock(&track_lock_);
                    allocated_objects_[p] = 0;
                    pthread_mutex_unlock(&track_lock_);
                    //freed_objects+=1; 
                }
                void TrackObject(void *p, unsigned long uid) {
                    pthread_mutex_lock(&track_lock_);
                    if (allocated_objects_.find(p) != allocated_objects_.end()) 
                        allocated_objects_[p] = uid;
                    else
                        allocated_objects_.insert(pair<void *, unsigned long>(p, uid));
                    pthread_mutex_unlock(&track_lock_);
#if 0
                    if (freed_objects > 2048) {
                        _trace("Freeing uncleared objects size before: %lu", allocated_objects_.size());
                        for (auto i = allocated_objects_.begin(), last = allocated_objects_.end(); i != last; ) {
                            if (! (*i).second) i = allocated_objects_.erase(i);
                            else ++i;
                        }
                        _trace("Size after free: %lu", allocated_objects_.size());
                        freed_objects = 0;
                    }
#endif
                }
                void Clear() {
                    allocated_objects_.clear();
                }

            private:
                int freed_objects;
                std::map<void *, bool> allocated_objects_;
                pthread_mutex_t track_lock_;
        };
    }
}
