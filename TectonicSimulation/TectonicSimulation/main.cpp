#include <iostream>

#include "Simulation.h"

int main (int argc, char * const argv[]) {
  
  Simulation sim;
  sim.addPlate(Vec3f(0,1,0), Vec3f(1,0,0));
  sim.addPlate(Vec3f(1.5,1,0), Vec3f(2.5,0,0));
  sim.setVelocity(Vec3f(0.1, 0, 0));
  
  char input;
  
  for (int i=0; i<100; i++)
  {
    std::cin.ignore();
    sim.run();
    sim.printSimulation();
  }
  
  return 0;
}
