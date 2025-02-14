#include "Timer.h"

Timer::Timer(){
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer(){
    std::chrono::duration<float> time = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Время выполнения: " << time.count() << " секунд\n"; 
}