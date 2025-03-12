// ref:https://zhuanlan.zhihu.com/p/352723264
struct Node{
    void *data;
    Node *next;
};

void Enqueue(Node *node){
    m_Tail->next = node;
    m_Tail = node;
}

Node* Dequeue(){
    Node *res = m_Head;
    m_Head = m_Head->next;
    return res;
}

// atomic operation

// Load/Store

// Test and Set
bool TestAndSet(bool * flag){
    bool res = *flag;
    *flag = true;
    return res;
}

// Clear

// Exchange
template <typename T>
T Exchange(T* addr, const T& newVal){
    T oldVal = *addr;
    *addr = newVal;
    return oldVal;
}

// Compare And Swap
template <typename T>
bool CompareAndSwap(T* addr, T& expected, const T& desired){
    if(*addr == expected){
        *addr = desired;
        return true;
    }
    expected = *addr;
    return false;
}

// Fetch And +/-/*//

/*
int i = 0;
i++;

mov         eax, dword ptr [i] // load i to eax寄存器
add         eax, 1 // eax += 1
mov         dword ptr [i], eax // &i = eax
*/

// Race condition
/*
int i = 0;
_InterlockedIncrement((volatile long *)&i);

mov         eax,1   // load 1 to eax
lock  xadd  dword ptr [i], eax  // xadd
*/

// using cas to implement nolock_queue

#pragma once
#include <windows.h>
#include <windef.h>
#include <intrin.h>
#include <emmintrin.h>

using AtomicWord = intptr_t;

struct AtomicNode
{
    volatile AtomicWord _next;
    void* data;
};

class AtomicQueue
{
    volatile AtomicWord _tail;
    volatile AtomicWord _head;

public:
    AtomicQueue();
    ~AtomicQueue();
    void Enqueue(AtomicNode* node);
    AtomicNode* Dequeue();
}

static inline AtomicWord AtomicExchangeExplicit(volatile AtomicWord* p, AtomicWord val)
{
    return (AtomicWord)_InterlockedExchange64((volatile long long*)p, (long long)val);
}

static inline bool AtomicCompareExchangeStrongExplicit(volatile AtomicWord* p, AtomicWord* oldval, AtomicWord newval)
{
    long long res = _InterlockedCompareExchange64((volatile long long*)p, (long long)newval, (long long)*oldval);
    if(res == *oldval)
        return true;

    *oldval = res;
    return false;
}

AtomicQueue() {
    AtomicNode* dummy = new AtomicNode();
    dummy->_next = 0;
    _tail = (AtomicWord)dummy;
    _head = (AtomicWord)dummy;
}
~AtomicQueue() {
    AtomicNode* dummy = (AtomicNode*)_head;
    delete dummy;
}

// Enqueue use Exchange, Dequeue use CAS
void Enqueue(AtomicNode* node){
    AtomicNode* prev;
    node->_next = 0;
    prev = (AtomicNode*)AtomicExchangeExplicit(&_tail, (AtomicWord)node);
    prev->_next = (AtomicWord)node;
}

AtomicNode* Dequeue() {
    AtomicNode* res, * next;
    void* data;
    AtomicWord head = _head;
    AtomicWord newHead;
    do
    {
        res = (AtomicNode*)head;
        next = (AtomicNode*)res->_next;
        if (next == nullptr)
            return nullptr;
        data = next->data;
        newHead = (AtomicWord)next;   
    } while (!AtomicCompareExchangeStrongExplicit(&_head, &head, newHead));

    res->data = data;
    return res;
}

// ABA problem: 1. circle buffer 2. double CAS

// Double CAS nolock_queue
struct alignas(16) AtomicWord2
{
    AtomicWord lo, hi;
};

struct inline bool AtomicCompareExchangeStrongExplicit(volatile AtomicWord2* p, AtomicWord2* oldval, AtomicWord2 newval)
{
    return _InterlockedCompareExchange128((volatile long long*)p, (long long)newval.hi, (long long)newval.lo, (long long*)oldval) != 0;
}

static inline AtomicWord2 AtomicExchangeExplicit(volatile AtomicWord2* p, AtomicWord2 newval)
{
    AtomicWord2 oldval;
    oldval.lo = 0;
    oldval.hi = newval.hi - 1;
    while (!AtomicCompareExchangeStrongExplicit(p, &oldval, newval));
    return oldval;
}

class AtomicQueue
{
    volatile AtomicWord _tail;
    volatile AtomicWord _head;

public:
    AtomicQueue();
    ~AtomicQueue();
    void Enqueue(AtomicNode* node);
    AtomicNode* Dequeue();
}

static inline AtomicWord AtomicExchangeExplicit(volatile AtomicWord* p, AtomicWord val)
{
    return (AtomicWord)_InterlockedExchange64((volatile long long*)p, (long long)val);
}

static inline bool AtomicCompareExchangeStrongExplicit(volatile AtomicWord* p, AtomicWord* oldval, AtomicWord newval)
{
    long long res = _InterlockedCompareExchange64((volatile long long*)p, (long long)newval, (long long)*oldval);
    if(res == *oldval)
        return true;

    *oldval = res;
    return false;
}

// updated version
AtomicQueue() {
    AtomicNode* dummy = new AtomicNode();
    dummy->_next = 0;
    _tail = (AtomicWord)dummy;
    _head = (AtomicWord)dummy;
}
~AtomicQueue() {
    AtomicNode* dummy = (AtomicNode*)_head;
    delete dummy;
}

// Enqueue use Exchange, Dequeue use CAS
void Enqueue(AtomicNode* node){
    AtomicNode* prev;
    node->_next = 0;
    prev = (AtomicNode*)AtomicExchangeExplicit(&_tail, (AtomicWord)node);
    prev->_next = (AtomicWord)node;
}

AtomicNode* Dequeue() {
    AtomicNode* res, * next;
    void* data;
    AtomicWord head = _head;
    AtomicWord newHead;
    do
    {
        res = (AtomicNode*)head;
        next = (AtomicNode*)res->_next;
        if (next == nullptr)
            return nullptr;
        data = next->data;
        newHead = (AtomicWord)next;   
    } while (!AtomicCompareExchangeStrongExplicit(&_head, &head, newHead));

    res->data = data;
    return res;
}
