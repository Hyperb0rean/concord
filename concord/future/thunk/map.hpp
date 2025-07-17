#pragma once

#include <functional>
#include <utility>

#include "axis/core/incel.hpp"
#include "concord/future/concepts.hpp"
#include "concord/runtime/intrusive_task.hpp"
#include "concord/runtime/runtime.hpp"

namespace concord::future::thunk {

template<Thunk Producer, typename F>
struct [[nodiscard]] Map {
    [[no_unique_address]] Producer producer;
    [[no_unique_address]] F map;

    using InputValueType = value_of<Producer>;
    using ValueType = std::invoke_result_t<F, InputValueType>;

    using State = state_of<Producer>;

    // Non-copyable
    Map(const Map&) = delete;
    auto operator=(const Map&) -> Map& = delete;

    Map(Map&&) = default;
    auto operator=(Map&&) -> Map& = default;

    explicit Map(Producer&& p, F f) :
        producer(std::move(p)),
        map(std::move(f)) {}

    template<Continuation<ValueType, State> Consumer>
    struct Demand: rt::IntrusiveTask {
        [[no_unique_address]] Consumer consumer;
        [[no_unique_address]] F map;

        Demand(Consumer&& c, F&& m) :
            consumer(std::move(c)),
            map(std::move(m)) {}

        axis::InitializationCell<InputValueType> value;
        State state;

        auto resume(InputValueType v, State s) {
            value.emplace(std::move(v));
            state = std::move(s);
            std::get<rt::IRuntime*>(s)->spawn(this);
        }

        // ITask
        auto run() noexcept -> void final {
            consumer.resume(
                map(std::move(std::move(value).get())),
                std::move(state)
            );
        }
    };

    template<Continuation<ValueType, State> Consumer>
    auto materialize(Consumer&& c) -> Computation auto {
        return producer.materialize(Demand {std::move(c), std::move(map)});
    }
};

} // namespace concord::future::thunk