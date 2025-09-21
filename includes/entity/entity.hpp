#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity 
{
protected:
    int x, y;
    char symbol;
    unsigned int r, g, b;
    bool alive;

public:
    Entity(int x, int y, char symbol, unsigned int r, unsigned int g, unsigned int b);

    virtual ~Entity() = default;

    virtual void Update(int targetX, int targetY) {}

    virtual void Render(struct ncplane* plane);
    
    virtual void Move(int dx, int dy, int maxX, int maxY);

    void Kill() { alive = false; }
    bool IsAlive() const { return alive; }

    int GetX() const { return x; }
    int GetY() const { return y; }
};

#endif