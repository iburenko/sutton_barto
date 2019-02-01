#ifndef bandit
#define bandit
#include "bandit.hpp"
#endif

class OptimisticBandit: public Bandit
{
    public:
        OptimisticBandit();
        OptimisticBandit(float);
        OptimisticBandit(float, bool, float);
        void update(int, float);
    private:
        float Q1;

};


OptimisticBandit::OptimisticBandit(): Q1(5.)
{
    for (int i = 0; i < k; i++)
        Q[i] = Q1;
}


OptimisticBandit::OptimisticBandit(float Q1): Q1(Q1)
{
    for (int i = 0; i < k; i++)
        Q[i] = Q1;
}


OptimisticBandit::OptimisticBandit(float Q1, bool stationarity, float alpha): Bandit(10, 0., stationarity)
{
    for (int i = 0; i < k; i++)
        Q[i] = Q1;
    non_bias_alpha = alpha;
}


void OptimisticBandit::update(int action, float reward)
{
    number_of_actions[action]++;
    Q[action] = Q[action] + non_bias_alpha * (reward-Q[action]);
    if (not stationarity)
    {
        for (int i = 0; i < k; i++)
            q_star[i] += random_walk(gen);
    }
}