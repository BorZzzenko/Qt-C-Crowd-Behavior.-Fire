#ifndef WALL_H
#define WALL_H

#include "objectonroomplan.h"

class Wall : public ObjectOnRoomPlan
{
public:
    explicit Wall(int x, int y, RoomPlan* roomPlan);

    ~Wall();
};

#endif // WALL_H
