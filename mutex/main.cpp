#include <iostream>
struct foo{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo* foo_alloc(int id){
    struct foo *fp;
    if (fp = static_cast<foo *>(malloc(sizeof(struct foo))); fp != nullptr){
        fp->f_count = 1;
        fp->f_id = id;
        if (pthread_mutex_init(&fp->f_lock, nullptr) != 0){
            free(fp);
            return nullptr;
        }
    }
    return fp;
}

void foo_hold(struct foo* fp){
    pthread_mutex_lock(&fp->f_lock);
    ++fp->f_count;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo* fp){
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0){///last reference
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
    else{
        pthread_mutex_unlock(&fp->f_lock);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
