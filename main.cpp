
#include <CL/sycl.hpp>
#include <iostream>

#include <helpers/timestamp.hpp>

using namespace cl::sycl;
using namespace std;

constexpr int NUM_THREADS_PER_GROUP = 64;

auto t = TimeStamp<std::string>();

template <typename T>
void run_kernel(queue &q, T *host_input_buf, size_t num_items) {
  assert(num_items % NUM_THREADS_PER_GROUP == 0);
  /* property_list properties{sycl::property::queue::enable_profiling()}; */
  T *DEVICE_RESULT = static_cast<T *>(malloc_device(num_items * sizeof(T), q));
  t.stamp("malloc");
  q.memset(DEVICE_RESULT, 0, sizeof(T) * num_items).wait();
  t.stamp("memcpy h->d");
  q.memcpy(DEVICE_RESULT, host_input_buf, sizeof(T) * num_items).wait();
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
  kernel_event.wait();
  t.stamp("memcpy d -> h");
  q.memcpy(host_input_buf, DEVICE_RESULT, sizeof(T) * num_items).wait();
  t.stamp();
  free(DEVICE_RESULT, q);
  t.print();

  auto end =
      kernel_event.get_profiling_info<info::event_profiling::command_end>();
  auto start =
      kernel_event.get_profiling_info<info::event_profiling::command_start>();
  std::cout << "kernel execution time without submission: "
            << (end - start) / 1.0e6 << " ms\n";
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

  size_t num_items = 1024;
  if (argc == 2) {
    num_items = atoi(argv[1]) * 1024;
  }
  cout << "num_items: " << num_items << "\n";
  int *host_data = static_cast<int *>(malloc(num_items * sizeof(int)));
  for (int i = 0; i < num_items; i++)
    host_data[i] = -1;

  run_kernel<int>(q, host_data, num_items);

  show_data(host_data, num_items);

  free(host_data);
  return 0;
}
