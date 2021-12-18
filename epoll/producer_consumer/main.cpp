#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <csignal>
#include <iostream>
#include <cstring>

struct thread_info {
    pthread_t thread_id;
    int rank;
    int epfd;
};

constexpr auto MAX_EVENTS_SIZE = 1024;
static void *consumer_routine(void *data) {
    auto *c = (struct thread_info *)data;
    struct epoll_event *events;
    int epfd = c->epfd;
    int nfds{0};
    int i {0};
    uint64_t result;
    std::cout << "Greetings from [consumer-" << c->rank << "]\n";
    events = static_cast<epoll_event *>(calloc(MAX_EVENTS_SIZE, sizeof(struct epoll_event)));
    if (events == nullptr){
        std::cout << "calloc epoll events\n";
    }

    for (;;) {
        nfds = epoll_wait(epfd, events, MAX_EVENTS_SIZE, 1000); // poll every second
        for (i = 0; i < nfds; i++) {
            if (events[i].events & EPOLLIN) {
                std::cout << "[consumer-" << c->rank << "] got event from fd-" << events[i].data.fd << '\n';
                // consume events (reset eventfd)
                read(events[i].data.fd, &result, sizeof(uint64_t));
                close(events[i].data.fd);   // NOTE: need to close here
            }
        }
    }
}

static void *producer_routine(void *data) {
    auto *p = (struct thread_info *)data;
    struct epoll_event event{};
    int epfd = p->epfd;
    int efd = -1;
    int ret = -1;

    std::cout << "Greetings from [producer-" << p->rank << "]\n";
    while (true) {
        sleep(1);
        // create eventfd (no reuse, create new every time)
        efd = eventfd(1, EFD_CLOEXEC|EFD_NONBLOCK);
        if (efd == -1) {
            std::cout << "eventfd create: " << strerror(errno);
        }
        // register to poller
        event.data.fd = efd;
        event.events = EPOLLIN | EPOLLET;    // Edge-Triggered
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &event);
        if (ret != 0) {
            std:: cout << "epoll_ctl\n";
        }
        // trigger (repeatedly)
        write(efd, (void *)0xffffffff, sizeof(uint64_t));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3){
        std::cout << "error\n";
        return -1;
    }
    char *end;
    long NUM_PRODUCERS = strtol(argv[1], &end, 10);
    struct thread_info *p_list = nullptr, *c_list = nullptr;
    int epfd = -1;
    int ret = -1, i = -1;
    // create epoll fd
    epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) {
        std::cout << "epoll_create1::" << strerror(errno);
        return -1;
    }
    // producers
    p_list = static_cast<thread_info *>(calloc(NUM_PRODUCERS, sizeof(struct thread_info)));
    if (!p_list) {
        std::cout << "calloc";
    }
    for (i = 0; i < NUM_PRODUCERS; i++) {
        p_list[i].rank = i;
        p_list[i].epfd = epfd;
        ret = pthread_create(&p_list[i].thread_id, nullptr, producer_routine, &p_list[i]);
        if (ret != 0) {
            std::cout << "pthread_create\n";
        }
    }
    // consumers
    long NUM_CONSUMERS = strtol(argv[2], &end, 10);
    c_list = static_cast<thread_info *>(calloc(NUM_CONSUMERS, sizeof(struct thread_info)));
    if (!c_list) {
        std::cout << ("calloc\n");
    }
    for (i = 0; i < NUM_CONSUMERS; i++) {
        c_list[i].rank = i;
        c_list[i].epfd = epfd;
        ret = pthread_create(&c_list[i].thread_id, nullptr, consumer_routine, &c_list[i]);
        if (ret != 0) {
            std::cout << ("pthread_create");
        }
    }
    // join and exit
    for (i = 0; i < NUM_PRODUCERS; i++) {
        ret = pthread_join(p_list[i].thread_id, nullptr);
        if (ret != 0) {
            std::cout << ("pthread_join");
        }
    }
    for (i = 0; i < NUM_CONSUMERS; i++) {
        ret = pthread_join(c_list[i].thread_id, nullptr);
        if (ret != 0) {
            std::cout << ("pthread_join");
        }
    }
    free(p_list);
    free(c_list);
    return EXIT_SUCCESS;
}