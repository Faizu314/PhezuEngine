#pragma once

#include <memory>
#include "Phezu.hpp"

enum RotateDirection {
    ClockWise,
    AntiClockWise
};

class TetrominoBase {
public:
    TetrominoBase(std::weak_ptr<Phezu::Entity> m_Blocks[]);
public:
    void MoveDown();
    void Rotate(RotateDirection direction);
private:
    int m_X, m_Y;
    std::weak_ptr<Phezu::Entity> m_Blocks[4];
};
