#include <spdlog/spdlog.h>

int main() 
{
    spdlog::set_pattern("%^[%T %l]: %v%$");
    spdlog::info("Initializing PaintGame v0.0.1");
    
    return 0;
}