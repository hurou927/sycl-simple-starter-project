#include <CL/sycl.hpp>
#include <iostream>
#include <iomanip>

#include <helpers/timestamp.hpp>
#include <helpers/event_holder.hpp>


using namespace cl::sycl;
using namespace std;

constexpr int NUM_THREADS_PER_GROUP = 64;

auto t = TimeStamp<std::string>();
auto events = EventHolder<std::string>();

template <typename T>
void run_kernel(queue &q, T *host_input_buf, size_t num_items) {
  assert(num_items % NUM_THREADS_PER_GROUP == 0);
  /* property_list properties{sycl::property::queue::enable_profiling()}; */

  t.stamp("dummy malloc to avoid some overhead(?)");
  T *_DEVICE_DATA = static_cast<T *>(malloc_device(sizeof(T), q));
  t.stamp("malloc 1");
  T *DEVICE_RESULT = static_cast<T *>(malloc_device(num_items * sizeof(T), q));
  t.stamp("malloc 2");
  T *DEVICE_RESULT_COPIED = static_cast<T *>(malloc_device(num_items * sizeof(T), q));
  t.stamp("memset");
  q.memset(DEVICE_RESULT, 0, sizeof(T) * num_items).wait();
  t.stamp("memcpy h->d");
  q.memcpy(DEVICE_RESULT, host_input_buf, sizeof(T) * num_items).wait();
  t.stamp("memcpy d->d");
  q.memcpy(DEVICE_RESULT_COPIED, DEVICE_RESULT, sizeof(T) * num_items).wait();
  t.stamp("kernel");
  event kernel_event = q.submit([&](handler &cgh) {
    auto localRange = range<1>(NUM_THREADS_PER_GROUP);
    accessor<T, 1, access::mode::read_write, access::target::local>
        LOCAL_SCAN_SPACE(localRange, cgh);

    auto kernel = [=](nd_item<1> it) {
      auto localX = it.get_local_id(0);
      auto globalX = it.get_global_id(0);

      LOCAL_SCAN_SPACE[localX] = DEVICE_RESULT[globalX] - globalX;
      it.barrier(access::fence_space::local_space);
      DEVICE_RESULT[globalX] = LOCAL_SCAN_SPACE[localX];
    };
    cgh.parallel_for<class pm>(nd_range<1>{range<1>(num_items), localRange},
                               kernel);
  });
  events.push("kernel", kernel_event);
  kernel_event.wait();
  t.stamp("memcpy d->h");
  q.memcpy(host_input_buf, DEVICE_RESULT, sizeof(T) * num_items).wait();
  t.stamp();
  free(_DEVICE_DATA, q);
  free(DEVICE_RESULT, q);
  free(DEVICE_RESULT_COPIED, q);

  return;
}

template <typename T> void show_data(T *data, size_t num_items) {
  if (num_items < 20) {
    for (int i = 0; i < 10; i++)
      cout << data[i] << ",";

  } else {
    for (int i = 0; i < 10; i++)
      cout << data[i] << ",";
    cout << "...,";
    for (int i = num_items - 10; i < num_items; i++)
      cout << data[i] << ",";
  }
  cout << "\n";
}

int main(int argc, char *argv[]) {

  gpu_selector d_selector;
  queue q(d_selector);
  cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

  size_t num_items = 1024;
  if (argc == 2) {
    num_items = atoi(argv[1]) * 1024 * 1024;
  }
  cout << "num_items: " << num_items << "\n";
  int *host_data = static_cast<int *>(malloc(num_items * sizeof(int)));
  for (int i = 0; i < num_items; i++)
    host_data[i] = -1;

  run_kernel<int>(q, host_data, num_items);

  for(auto &e: t.data()) {
    auto mbytes = static_cast<double> (sizeof(int)) * num_items / 1024 / 1024 / 1024;
    printf("%-15s\t%7.3f ms\t%7.3f GB/s\n",e.tag.c_str(),e.get_ms(),mbytes/e.get_sec());
  }

  for (auto &e: events.data()) {
    printf("sycl event: %s,%f ms\n", e.tag.c_str(), e.get_submission_ms());
  }
  show_data(host_data, num_items);

  free(host_data);
  return 0;
}
