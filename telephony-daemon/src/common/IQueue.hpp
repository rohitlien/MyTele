#ifndef IQUEUE_HPP
#define IQUEUE_HPP

template <typename T>
class IQueue {
public:
    virtual ~IQueue() = default;
    virtual void push(T val) = 0;
    virtual T pop() = 0;
};

#endif