#include "Server.h"

int main()
{
    Server sr;
    sr.downloadData();
    sr.listenPort();
    sr.saveData();
    return 0;
}