#include "sources/file.hpp"

#include <iostream>


int main ()
{
    File file1("WASAPICaptureMy.wav", CASM::READ);
    std::cout << std::endl;
    File file2("WASAPICaptureTimerDriven-{010D14FD-6B89-4717-A6DA-AAADA8241F29}.wav", CASM::READ);

    return 0;
}