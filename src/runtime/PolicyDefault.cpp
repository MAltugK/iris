#include "PolicyDefault.h"
#include "Debug.h"
#include "Platform.h"
#include "Device.h"
#include "Task.h"


namespace iris {
namespace rt {

PolicyDefault::PolicyDefault(Scheduler* scheduler) {
  SetScheduler(scheduler);
}

PolicyDefault::~PolicyDefault() {
}

void PolicyDefault::GetDevices(Task* task, Device** devs, int* ndevs) {
  int selected = 0;
  char* DEVORDER = NULL;
  char* JOBS = NULL;

  Platform::GetPlatform()->EnvironmentGet("DEVORDER", &DEVORDER, NULL);
  const char* delim = ":";
  char devs_str[128];
  memset(devs_str, 0, 128);
  strncpy(devs_str, DEVORDER, strlen(DEVORDER));
  char* rest2 = devs_str;


  Platform::GetPlatform()->EnvironmentGet("JOBS", &JOBS, NULL);
  char job_str[128];
  memset(job_str, 0, 128);
  strncpy(job_str, JOBS, strlen(JOBS));
  char* rest = job_str;
  char* a = NULL;
  int temp = 0;
  int devselection = 0;
  int position = 0;
  int order = 0;

  while ((a = strtok_r(rest, delim, &rest))) {
    if (strcasecmp(a, task->name()) == 0) order=temp;
    temp++;
  }

  while ((a = strtok_r(rest2, delim, &rest2))) {
    if (position == order) devselection = atoi(a);
    position++;
  }

  for(selected=0; selected<ndevs_; selected++) {
    if (IsKernelSupported(task, devs_[selected]) && (devs_[selected]->devno() == devselection)) break;
  }
  if (selected == ndevs_) selected = 0;
  devs[0] = devs_[selected];
  *ndevs = 1;
}

} /* namespace rt */
} /* namespace iris */
