#include <iris/iris.h>
#include <stdio.h>

char a[12] = "heLLo wOrLd";
char b[12];
char c[12] = "heLLo wOrLd";
char d[12];
size_t size = 12;

int main(int argc, char** argv) {
  iris_init(&argc, &argv, 1);

  iris_mem mem_a;
  iris_mem mem_b;
  iris_mem_create(size, &mem_a);
  iris_mem_create(size, &mem_b);

  iris_task task;
  iris_task_create(&task);
  iris_task_h2d(task, mem_a, 0, size, a);
  void* params[2] = { &mem_b, &mem_a };
  int params_info[2] = { iris_w, iris_r };
  iris_task_kernel(task, "uppercase", 1, NULL, &size, NULL, 2, params, params_info);
  iris_task_d2h(task, mem_b, 0, size, b);

  iris_mem mem_c;
  iris_mem mem_d;
  iris_mem_create(size, &mem_c);
  iris_mem_create(size, &mem_d);

  iris_task task2;
  iris_task_create(&task2);
  iris_task_h2d(task2, mem_c, 0, size, c);
  void* params2[2] = { &mem_d, &mem_c };
  int params_info2[2] = { iris_w, iris_r };
  iris_task_kernel(task2, "lowercase", 1, NULL, &size, NULL, 2, params2, params_info2);
  iris_task_d2h(task2, mem_d, 0, size, d);

  iris_task_submit(task, iris_default, NULL, 1); 

  printf("New text %s\n", b);

  iris_task_release(task);
  iris_mem_release(mem_a);
  iris_mem_release(mem_b);

  iris_task_submit(task2, iris_default, NULL, 1);
  
  printf("New text %s\n", d);

  iris_task_release(task2);
  iris_mem_release(mem_c);
  iris_mem_release(mem_d);
  iris_finalize();

  return 0;
}

