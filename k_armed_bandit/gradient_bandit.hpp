#ifndef bandit
#define bandit
#include "bandit.hpp"
#endif

std::uniform_real_distribution<float> uniform(0, 1);

class GradientBandit: public Bandit
{
    public:
        GradientBandit();
        GradientBandit(float);
        GradientBandit(float, bool);
        GradientBandit(float, bool, bool);
        void update(int, float);
        int action();
    private:
        float* Ht;
        float* p;
        float step_size;
        bool baseline;
        float Rt;
        int steps;
        float mean;
};


GradientBandit::GradientBandit(): mean(4.), step_size(0.1)
{
    std::normal_distribution<float> high_start(mean, 1);
    baseline = true;
    Rt = 0;
    steps = 0;
    Ht = new float[k]();
    p = new float[k];
    for (int i = 0; i < k; i++)
        p[i] = 1./k;
    for(int i = 0; i < k; i++)
        q_star[i] = high_start(gen);
}


GradientBandit::GradientBandit(float alpha): mean(4.), step_size(alpha)
{
    std::normal_distribution<float> high_start(mean, 1);
    baseline = true;
    Rt = 0;
    steps = 0;
    Ht = new float[k]();
    p = new float[k];
    for (int i = 0; i < k; i++)
        p[i] = 1./k;
    for(int i = 0; i < k; i++)
        q_star[i] = high_start(gen);
        
}


GradientBandit::GradientBandit(float alpha, bool baseline_): mean(4.), step_size(alpha), baseline(baseline_)
{
    std::normal_distribution<float> high_start(mean, 1);
    Rt = 0;
    steps = 0;
    Ht = new float[k]();
    p = new float[k];
    for (int i = 0; i < k; i++)
        p[i] = 1./k;
    for(int i = 0; i < k; i++)
        q_star[i] = high_start(gen);
}


GradientBandit::GradientBandit(float alpha, bool baseline_, bool stationarity_): step_size(alpha), baseline(baseline_)
{
    this->mean = 4.;
    this->stationarity = stationarity_;
    std::normal_distribution<float> high_start(mean, 1);
    Rt = 0;
    steps = 0;
    Ht = new float[k]();
    p = new float[k];
    for (int i = 0; i < k; i++)
        p[i] = 1./k;
    for(int i = 0; i < k; i++)
        q_star[i] = high_start(gen);
}


void GradientBandit::update(int action, float reward)
{
    float base;
    step += 1;   
    baseline ? base = Rt : base = .0;
    for (int i = 0; i < k; i++)
    {
        if (action != i)
            Ht[i] = Ht[i] - step_size * (reward - base) * p[i];
        else
            Ht[i] = Ht[i] + step_size * (reward - base) * (1. - p[i]);            
    }
    if (not stationarity)
    {
        for (int i = 0; i < k; i++)
            q_star[i] += random_walk(gen);
    }
    Rt = (1./step) * (reward - Rt) + Rt;
    float max_power = *std::max_element(Ht, Ht+k);
    float denominator = .0;
    for (int i = 0; i < k; i++)
        denominator += std::exp(Ht[i] - max_power);
    for (int i = 0; i < k; i++)
        p[i] = std::exp(Ht[i] - max_power)/denominator;
}


int GradientBandit::action()
{
    int action = -1;
    float eps = uniform(gen);
    float curr_prob = p[0];
    for (int i = 0; i < k; i++)
    {
        if (curr_prob > eps)
        {
            action = i;
            break;
        }
        curr_prob += p[i + 1];
    }
    return action;
}