#include "MyRandom.h"

float MyRandom::_0_1F()
{
    return QRandomGenerator::global()->generateDouble();
}

double MyRandom::_0_1()
{
    return QRandomGenerator::global()->generateDouble();
}
