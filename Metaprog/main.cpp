#include <iostream>

struct A {
    void update(double time) {
        std::cout << "A::update " << time << std::endl;
    }
};

struct B {
    //void update(double time) {
    //    std::cout << "B::update " << time << std::endl;
    //}
};

struct C {
    void update(double time) {
        std::cout << "C::update " << time << std::endl;
    }
};

struct nil {};

template <class H, class T = nil>
struct cons {
    typedef H head;
    typedef T tail;
};

typedef cons<A, cons <B, cons<C> > > MyList;

template <class A, class B>
struct base : A, B {};

template <class T>
struct has_update {
    typedef char true_type;
    typedef struct {
        true_type m[2];
    } false_type;

    template <typename V, V v>
    struct Helper{};

    template <class V>
    static false_type test(...);
    template <class V>
    static true_type test(Helper<void (V::*)(double), &V::update> *p);
    static const bool value = sizeof(test<T>(0)) == sizeof(true_type);
};

template <class L>
struct inherit : base <typename L::head, inherit <typename L::tail> > {
    template <class L>
    void update_l(double time) {
        has_update<L::head> hasUpdate;
        updater<typename L::head, hasUpdate.value> updaterA;
        updaterA.do_that(static_cast<L::head&>(*this), time);
        update_l<L::tail>(time);
    }

    template <>
    void update_l<nil>(double time) {
    }
};

template <class L, bool B>
struct updater{
        void do_that(L& that, double time) {
    }    

};

template <class L>
struct updater<L, true> {
    void do_that(L& that, double time) {
        that.update(time);
    }    
};

template <>
struct inherit<nil> {};

struct MyClass : inherit<MyList> {
    typedef MyList MyList;

    void update(double time) {
        update_l<MyList>(time);
    }
};

int main() {
    MyClass a;
    a.update(3.14);

    return 0;
}