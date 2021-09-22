//
// Created by qiangwang on 2021/9/22.
//

#ifndef FILETYPE_ARGV_RANGE_H
#define FILETYPE_ARGV_RANGE_H

class argv_range {
public:
    argv_range(int argc, const char * const argv[])
            : argc_(argc), argv_(argv)
    {
    }

    const char * const *begin() const { return argv_; }
    const char * const *end() const { return argv_ + argc_; }

private:
    const int argc_;
    const char * const *argv_;
};


#endif //FILETYPE_ARGV_RANGE_H
