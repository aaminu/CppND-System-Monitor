#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  void setValues(float &total, float &idle);

  // TODO: Declare any necessary private members
 private:
  float currTotal_  = 0.0;
  float prevTotal_ = 0.0;
  float prevIdle_ = 0.0;
  float currentIdle_ = 0.0;
};

#endif