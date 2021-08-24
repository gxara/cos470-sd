#include <vector>

using namespace std;

vector<vector<int>> splitVectorIntoChunks(const vector<int> &source, size_t chunkSize)
{
    vector<vector<int>> result;
    result.reserve((source.size() + chunkSize - 1) / chunkSize);

    auto start = source.begin();
    auto end = source.end();

    while (start != end)
    {
        auto next = distance(start, end) >= chunkSize
                        ? start + chunkSize
                        : end;

        result.emplace_back(start, next);
        start = next;
    }

    return result;
}