#ifndef COLLISION_H
#define COLLISION_H

typedef struct
{
    float x;
    float y;

    float width;
    float height;
} Collider;

int CheckCollision(Collider a, Collider b);

#endif