#include <iostream>
struct job{
    struct job* j_next;
    struct job* j_prev;
    pthread_t j_id;
};

struct queue{
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

///initialize a queue
int queue_init(struct queue* qp){
    int err;
    qp->q_head = nullptr;
    qp->q_tail = nullptr;
    err = pthread_rwlock_init(&qp->q_lock, nullptr);
    if (err != 0){
        return err;
    }
    return 0;
}

///insert a job at the head of the queue
void job_insert(struct queue* qp, struct job *jp){
    pthread_rwlock_wrlock(&qp->q_lock);

    jp->j_next = qp->q_head;
    jp->j_prev = nullptr;
    if (qp->q_head != nullptr){
        qp->q_head->j_prev = jp;
    }
    else{
        qp->q_tail = jp;    ///list was empty
    }
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

///remove a given job from a queue
void job_remove(struct queue* qp, struct job* jp){
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head){
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp){
            qp->q_tail = nullptr;
        }
        else{
            jp->j_next->j_prev = jp->j_prev;
        }
    }
    else if (jp == qp->q_tail){
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    }
    else{
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

///find a job for the given thread ID
struct job* job_find(struct queue* qp, pthread_t id){
    struct job *jp;
    if (pthread_rwlock_rdlock(&qp->q_lock) != 0){
        return nullptr;
    }
    for (jp = qp->q_head; jp != nullptr; jp = jp->j_next){
        if (pthread_equal(jp->j_id, id)){
            break;
        }
    }
    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
