#include "vertex.h"

void Vertex::displace(double y_dis) {
  if (position.y() + y_dis > 1.0)
    position.sety(1.0);
  else if (position.y() + y_dis < -1.0)
    position.sety(-1.0);
  else
    position.sety(position.y() + y_dis);
}
