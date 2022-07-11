#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
   float utilization =((currTotal_ - prevTotal_)  - (currentIdle_ - prevIdle_)) / (currTotal_ - prevTotal_);
    return utilization;
 }

 void Processor::setValues(float &total, float &idle){
    prevTotal_ = currTotal_;
    prevIdle_ = currentIdle_;

    currentIdle_ = idle;
    currTotal_ = total;
 }