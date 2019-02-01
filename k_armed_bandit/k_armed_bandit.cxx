#include "optimistic_bandit.hpp"
#include "gradient_bandit.hpp"
#include "ucb_bandit.hpp"
#include <fstream>

int main(int argc, const char* argv[])
{
	// std::ofstream stats;
	// stats.open(argv[1]);
	// int k = k = std::stoi(argv[2]);
	// int stationarity = std::stoi(argv[3]);
	// float alpha;
	// std::vector<float> param;
	// if (std::strcmp(argv[1], "Bandit")==0)
	// {
	// 	std::vector<float> foo{1./128, 1./64, 1./32, 1./16, 1./8, 1./4, 1./2, 1., 2., 4.};
	// 	param = foo;
	// 	alpha = std::stof(argv[4]);
	// }
	// else if (argv[1] == "Gradient")
	// {
	// 	std::cout<<"grad\n";
	// 	GradientBandit b;
	// }
	// else if (std::strcmp(argv[1], "Optimistic")==0)
	// {
	// 	std::vector<float> foo{1./16, 1./8, 1./4, 1./2, 1., 2., 4., 8., 16.};
	// 	param = foo;
	// 	alpha = std::stof(argv[4]);
	// }
	// else if (argv[1] == "UCB")
	// {
	// 	std::cout<<"ucb\n";
	// 	UCBBandit b;
	// }
	// else
	// {
	// 	std::cout<<"nothing\n"<<argv[1]<<std::endl;
	// }

	int action;
	float reward;
	int epochs = 1000;
	int steps = 1000;
	std::vector<float>::iterator it;
	float** results = new float*[2];
	results[0] = new float[steps]();
	results[1] = new float[steps]();

	float average_reward = .0;
	float optimal_actions = .0;
	for (int i = 0; i < epochs; i++)
	{
		// OptimisticBandit b(it, stationarity, alpha);
		// Bandit b(k, it, stationarity, alpha);
		UCBBandit b(1, false);
		for (int j = 0; j < steps; j++)
		{
			action = b.action();
			reward = b.reward(action);
			results[0][j] += reward;
			results[1][j] += b.optimal_action(action);
			b.update(action, reward);
		}
	}
	for (int i = 0; i < steps; i++)
	{
		average_reward += results[0][i];
		optimal_actions += results[1][i];
	}
	average_reward /= (steps)*epochs;
	optimal_actions /= (steps)*epochs;
	std::cout << average_reward<<", "<<optimal_actions<<std::endl;
	std::cout<<average_reward<<"\t"<<optimal_actions<<std::endl;









	// for (auto it: param)
	// {	
	// 	std::cout<<it<<std::endl;
	// 	float average_reward = .0;
	// 	float optimal_actions = .0;
	// 	for (int i = 0; i < epochs; i++)
	// 	{
	// 		// OptimisticBandit b(it, stationarity, alpha);
	// 		// Bandit b(k, it, stationarity, alpha);
	// 		GradientBandit b;
	// 		for (int j = 0; j < steps; j++)
	// 		{
	// 			action = b.action();
	// 			reward = b.reward(action);
	// 			results[0][j] += reward;
	// 			results[1][j] += b.optimal_action(action);
	// 			b.update(action, reward);
	// 		}
	// 	}
	// 	for (int i = steps/2; i < steps; i++)
	// 	{
	// 		average_reward += results[0][i];
	// 		optimal_actions += results[1][i];
	// 	}
	// 	average_reward /= (steps)*epochs;
	// 	optimal_actions /= (steps)*epochs;
	// 	stats << it << ", "<<average_reward<<", "<<optimal_actions<<std::endl;
	// 	std::cout<<it<<"\t"<<average_reward<<"\t"<<optimal_actions<<std::endl;
	// 	for (int i = 0; i < steps; i++)
	// 	{
	// 		results[0][i] = .0;
	// 		results[1][i] = .0;
	// 	}
	// }
	// delete[] results[0];
	// delete[] results[1];
	// delete[] results;
	// stats.close();
	return 0;
}