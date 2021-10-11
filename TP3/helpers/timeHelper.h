#include <chrono>
#include <iomanip>
using namespace std;

// Function to return current unix timestamp (milliseconds) as string
string getTimestamp()
{
    const auto now = chrono::system_clock::now();
    const auto nowAsTimeT = chrono::system_clock::to_time_t(now);
    const auto nowMs = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;
    stringstream nowSs;
    nowSs << put_time(localtime(&nowAsTimeT), "%T") << '.' << setfill('0') << setw(3) << nowMs.count();
    return nowSs.str();
}