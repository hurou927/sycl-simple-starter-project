#include <CL/sycl.hpp>
#include <CL/sycl/event.hpp>
#include <iostream>
#include <vector>

template <typename T> class TaggedEvent {
public:
  T tag;
  sycl::event event;
  TaggedEvent(T tag, sycl::event event) : tag(tag), event(event){};

  double get_submission_ms() {
    auto end =
        event.get_profiling_info<sycl::info::event_profiling::command_end>();
    auto start =
        event.get_profiling_info<sycl::info::event_profiling::command_start>();
    return (end - start) / 1.0e6;
  };
};

template <typename T> class EventHolder {
private:
  const size_t capacity = 20;
  std::vector<TaggedEvent<T>> events;

public:
  EventHolder() { events.reserve(capacity); };
  void push(T tag, sycl::event &ev) { events.push_back(TaggedEvent(tag, ev)); }
  std::vector<TaggedEvent<T>> data() { return events; }
};
