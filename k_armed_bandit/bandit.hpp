#include <iostream>
#include <random> 

std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution<float> d(0, 1);
std::normal_distribution<float> random_walk(0, 0.01);


class Bandit
{
	public:
		Bandit();
		Bandit(int k, float eps);
        Bandit(int k, float eps, bool stationarity);
        Bandit(int k, float eps, bool stationarity, float step_size);
        virtual int action();
        float reward(int);
        virtual void update(int, float);
        int optimal_action(int);
        int maximization_action(float*);
		void print();
        int k;
        float eps;
        float* q_star;
        float* Q;
        float* number_of_actions;
        bool stationarity;
        int step;
        float non_bias_alpha;
};


Bandit::Bandit(): k(10), eps(0.0), stationarity(true)
{
    step = 0;
	q_star = new float[k];
    Q = new float[k]();
    number_of_actions = new float[k]();
	for (int i = 0; i < k; i++)
		q_star[i] = d(gen);
}


Bandit::Bandit(int k_, float eps_): k(k_), eps(eps_), stationarity(true)
{
    step = 0;
	q_star = new float[k];
    Q = new float[k]();
    number_of_actions = new float[k]();
	for (int i = 0; i < k; i++)
		q_star[i] = d(gen);
}


Bandit::Bandit(int k_, float eps_, bool stationarity_): k(k_), eps(eps_), stationarity(stationarity_)
{
    step = 0;
    q_star = new float[k]();
    Q = new float[k]();
    number_of_actions = new float[k]();
    if (stationarity)
    {
        for (int i = 0; i < k; i++)
		    q_star[i] = d(gen);
    }
    else
        non_bias_alpha = 0.;
}


Bandit::Bandit(int k_, float eps_, bool stationarity_, float step_size): k(k_), eps(eps_), stationarity(stationarity_)
{
    step = 0;
    q_star = new float[k]();
    Q = new float[k]();
    number_of_actions = new float[k]();
    if (stationarity)
    {
        for (int i = 0; i < k; i++)
		q_star[i] = d(gen);
    }
    if (not stationarity)
        non_bias_alpha = step_size;
}


int Bandit::action()
{	
    int action = -1;
    std::uniform_real_distribution<float> real_d(0,1);
    float prob = real_d(gen);
    if (prob < eps)
    {
        std::uniform_int_distribution<int> random_action(0, k-1);
        action = random_action(gen);
    }
    else
        action = maximization_action(Q);
    if (action < 0  || action > k)
    {
        std::cout<<"ACTION IS WRONG!\t"<<action<<std::endl;
        return -1;
    }
    return action;
}


float Bandit::reward(int action)
{
    float mean = q_star[action];
    std::normal_distribution<float> reward_dist(mean, 1.);
    return reward_dist(gen);
}


void Bandit::update(int action, float reward)
{
    number_of_actions[action]++;
    if (not stationarity)
    {
        for (int i = 0; i < k; i++)
            q_star[i] += random_walk(gen);
        if (non_bias_alpha == .0)
            Q[action] = Q[action] + (1/number_of_actions[action]) * (reward-Q[action]);
        else
            Q[action] = Q[action] + non_bias_alpha * (reward - Q[action]);
    }
    else
        Q[action] = Q[action] + (1/number_of_actions[action]) * (reward-Q[action]);
    step++;
}


int Bandit::optimal_action(int action)
{
    int optimal = 0;
    float max_elem = *std::max_element(q_star, q_star+k);
    int argmax = -1;
    for (int i = 0; i < k; i++)
    {
        if (q_star[i] == max_elem)
        {
            argmax = i;
            break;
        }
    }
    if (action == argmax)
        optimal = 1;
    return optimal;
}


int Bandit::maximization_action(float* arr)
{
    int action = -1;
    int number_of_maxs = 0;
    float max_elem = *std::max_element(arr, arr+k);
    for (int i = 0; i < k; i++)
    {
        if (max_elem == arr[i])
            number_of_maxs++;
    }
    int* ties = new int[number_of_maxs]();
    if (number_of_maxs == 1)
    {
        for (int i = 0; i < k; i++)
        {
            if (max_elem == arr[i])
            {
                action = i;
                break;
            }
        }
    }
    else
    {
        int ind = 0;
        for (int i = 0; i < k; i++)
        {
            if (arr[i] == max_elem)
            {
                ties[ind] = i;
                ind++;
            }
        }
        std::uniform_int_distribution<int> ties_d(0, number_of_maxs-1);
        action = ties[ties_d(gen)];
    }
    delete[] ties;
    return action;
}


void Bandit::print()
{
    for (int i = 0; i < k; i++)
        std::cout<<Q[i]<<"\t";
    std::cout<<std::endl;
    for (int i = 0; i < k; i++)
        std::cout<<q_star[i]<<"\t";
    std::cout<<std::endl;
    for (int i = 0; i < k; i++)
        std::cout<<number_of_actions[i]<<"\t";
	std::cout<<"\n--------------------------------------------------\n";
}