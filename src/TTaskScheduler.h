#pragma once

#include <iostream>
#include <vector>
#include <memory>


class __BaseHolder {
public:
    virtual ~__BaseHolder() = default;
};


template<typename T>
class __Holder : public __BaseHolder {
public:
    __Holder(T value) : value_(value) {}

    T Get() {
        return value_;
    }

private:
    T value_;
};


class MultiTypeStorage {
public:
    MultiTypeStorage() {};

    template<typename T>
    MultiTypeStorage(T value) {
        Set(value);
    }

    template<typename T>
    void Set(T value) {
        holder_ = std::make_unique<__Holder<T>>(value);
    }

    template<typename T>
    T Get() {
        __Holder<T>* specificHolder = dynamic_cast<__Holder<T>*>(holder_.get());
        if (!specificHolder) {
            throw std::runtime_error("Failed to cast value");
        }
        return specificHolder->Get();
    }
private:
    std::unique_ptr<__BaseHolder> holder_;
};


class __EmptyType {};
__EmptyType __empty_ref;


template <typename result_t, typename func_t, typename T1 = __EmptyType, typename T2 = __EmptyType>
class FutureResultNode;


class BasicFutureNode {
public:
    ~BasicFutureNode() = default;
};


class BasicNode {
public:
    ~BasicNode() = default;
    virtual void Calculate() {}
};


template <typename func_t, typename T1 = __EmptyType, typename T2 = __EmptyType>
class Node : public BasicNode {
public:
    Node(func_t func, T1 arg1 = __empty_ref, T2 arg2 = __empty_ref) :
            func_(func),
            arg1_(arg1),
            arg2_(arg2) {
        is_calculated_ = false;
    }

    template <typename result_t>
    result_t GetResult() {
        if (!is_calculated_) {
            Calculate();
            is_calculated_ = true;
        }
        return result_.Get<result_t>();
    }

    void Calculate() override {
        if (is_calculated_) {
            return;
        }
        if constexpr (std::is_same_v<T1, __EmptyType>) {
            result_.Set(func_());
        } else if constexpr (std::is_same_v<T2, __EmptyType>) {
            if constexpr (std::is_base_of_v<BasicFutureNode, T1>) {
                result_.Set(func_(arg1_.GetResult()));
            } else if constexpr (!std::is_base_of_v<BasicFutureNode, T1>) {
                result_.Set(func_(arg1_));
            }
        } else if constexpr (!std::is_same_v<T2, __EmptyType>) {
            if constexpr (std::is_base_of_v<BasicFutureNode, T1>) {
                if constexpr (std::is_base_of_v<BasicFutureNode, T2>) {
                    result_.Set(func_(arg1_.GetResult(), arg2_.GetResult()));
                } else if constexpr (!std::is_base_of_v<BasicFutureNode, T2>) {
                    result_.Set(func_(arg1_.GetResult(), arg2_));
                }
            } else if constexpr (!std::is_base_of_v<BasicFutureNode, T1>) {
                if constexpr (std::is_base_of_v<BasicFutureNode, T2>) {
                    result_.Set(func_(arg1_, arg2_.GetResult()));
                } else if constexpr (!std::is_base_of_v<BasicFutureNode, T2>) {
                    result_.Set(func_(arg1_, arg2_));
                }
            }
        }
    }
private:
    bool is_calculated_;
    func_t func_;
    T1 arg1_;
    T2 arg2_;
    MultiTypeStorage result_;
};


template <typename result_t, typename func_t, typename T1, typename T2>
class FutureResultNode : public BasicFutureNode {
public:
    FutureResultNode(Node<func_t , T1, T2>* par_node) {
        par_node_ = par_node;
    };

    virtual result_t GetResult() {
        return par_node_->template GetResult<result_t>();;
    };
private:
    Node<func_t, T1, T2>* par_node_;
};


class TTaskScheduler {
public:
    template <typename func_t, typename... Args>
    Node<func_t, Args...> add(func_t func, Args... args) {
        Node<func_t, Args...> node = Node<func_t, Args...>(func, args...);
        nodes_list_.push_back(&node);
        return node;
    }

    template <typename result_t, typename func_t, typename... Args>
    FutureResultNode<result_t, func_t, Args...> getFutureResult(Node<func_t, Args...>& node) {
        return FutureResultNode<result_t, func_t, Args...>(&node);
    }

    template <typename result_t, typename func_t, typename... Args>
    result_t getResult(Node<func_t, Args...>& node) {
        return node.template GetResult<result_t>();
    }

    void executeAll() {
        for (auto& i : nodes_list_) {
            i->Calculate();
        }
    }
private:
    std::vector<BasicNode*> nodes_list_;
};

