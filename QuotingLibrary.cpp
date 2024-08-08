#include <iostream>
#include <unordered_map>
#include <queue>

using namespace std;

int main() {
    unsigned long long N;
    int K;
    cin >> N >> K;

    unordered_map<string, queue<unsigned long long>> lastRequestTime;

    for (int i = 0; i < K; ++i) {
        unsigned long long timestamp;
        string token;
        cin >> timestamp >> token;

        auto &q = lastRequestTime[token];

        // Удаляем все запросы старше 1000 миллисекунд
        while (!q.empty() && timestamp - q.front() >= 1000) {
            q.pop();
        }

        // Если количество запросов меньше N, пропускаем текущий запрос
        if (q.size() < N) {
            cout << timestamp << " " << token << endl;
            q.push(timestamp);
        }
    }

    return 0;
}
