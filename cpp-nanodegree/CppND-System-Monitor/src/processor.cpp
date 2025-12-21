#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long total_jiffies = LinuxParser::Jiffies();
  long active_jiffies = LinuxParser::ActiveJiffies();
  long delta_total = total_jiffies - previous_total_;
  long delta_active = active_jiffies - previous_active_;
  previous_total_ = total_jiffies;
  previous_active_ = active_jiffies;
  if (delta_total > 0) {
    return static_cast<float>(delta_active) / static_cast<float>(delta_total);
  }
  return 0.0;
}