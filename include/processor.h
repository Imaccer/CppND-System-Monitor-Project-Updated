#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long int prevuser_{0};
  long int prevnice_{0};
  long int prevsystem_{0};
  long int previdle_{0};
  long int previowait_{0};
  long int previrq_{0};
  long int prevsoftirq_{0};
  long int prevsteal_{0};
};

#endif
