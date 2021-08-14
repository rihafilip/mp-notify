#include "mpdclient.h"

#include <iostream>
#include <csignal>

int main ()
{
    try
    {
        MpdClient client{};
        client.listen();
    }
    catch(const std::runtime_error & e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << "Terminating" << std::endl;
        return 1;
    }

}
