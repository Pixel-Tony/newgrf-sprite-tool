#include "Mytec/model/editor.hpp"

#include <algorithm>


namespace mytec::model
{
void editor::swap_colors()
{
    std::swap(primary, secondary);
}
}
