#ifndef bandit
#define bandit
#include "bandit.hpp"
#endif

class UCBBandit: public Bandit
{
    public:
        UCBBandit();
        UCBBandit(float);
        UCBBandit(float, bool);
        int action();
        void update(int, float);
    private:
        float c;
        float* ucb;
};


UCBBandit::UCBBandit()
{
    c = 1.;
    ucb = new float[k]();
}


UCBBandit::UCBBandit(float c)
{
    this->c = c;
    ucb = new float[k]();
}


UCBBandit::UCBBandit(float c, bool stationarity_)
{
    this->c = c;
    this->stationarity = stationarity_;
    ucb = new float[k]();
}


int UCBBandit::action()
{
    int action = -1;
    for (int i = 0; i < k; i++)
    {
        if (number_of_actions[i] == 0)
            ucb[i] = 10000000;
    }
    action = maximization_action(ucb);  
    return action;
}


void UCBBandit::update(int action, float reward)
{
    number_of_actions[action]++;
    step++;  
    Q[action] = Q[action] + (1/number_of_actions[action])*(reward-Q[action]);
    ucb[action] = Q[action] + c * std::sqrt(std::log(step)/number_of_actions[action]);
    if (not stationarity)
    {
        for (int i = 0; i < k; i++)
            q_star[i] += random_walk(gen);
    }
     
}




