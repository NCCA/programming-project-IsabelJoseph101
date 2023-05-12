#include "BlockSelector.h"
#include <ngl/VAOPrimitives.h>

BlockSelector::BlockSelector()
{  
}

void BlockSelector::move()
{
    m_position = m_position + m_displacement;   
}

void BlockSelector::moveNextPosition()
{
    m_nextPosition = m_position + m_displacement;   
}
