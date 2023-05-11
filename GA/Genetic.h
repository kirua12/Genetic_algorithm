#pragma once
#include <cmath>
#include<string>
#include<vector>
#include <iostream>
#include<random>
#include<ctime>
#include<fstream>
#include <cstdlib>
class Genetic
{

	double m_x1_bit_size;
	double m_x2_bit_size;
	double m_min_x1;
	double m_max_x1;
	double m_max_x2;
	double m_min_x2;
	double m_p_c;
	double m_p_m;

	int m_epoch;


	//pop size
	int m_pop_size;
	//���� ���� ����
	void initializePopulation(std::vector<std::string>* bits);
	// ������ ���� ����
	void reProduction(std::vector<std::string> *bits, double *F, double *best_F);
	//string 2���� ���¸� double������
	double string2Double(std::string str);
	//Fitness�� ���ϴ� �Լ�
	void getFitness(std::vector<std::string> bits, std::vector<double> *fs, double *F);
	void getProbability(std::vector<double> fs, double F, std::vector<double>* ps, std::vector<double>*qs);
	//����� �����ڸ� �����ؼ� �����.
	void select(std::vector<double> qs, std::vector<std::string> *bits);
	// ���� �ֱ�
	void crossOver(std::vector<std::string>* bits);
	//�ڸ��� ����
	void cutAndReplace(std::vector<std::string>* bits, std::vector<int> p_cs, std::vector<int> poses);
	//�������� �����
	void mutate(std::vector<std::string>* bits);
	template<typename T>
	void makeTxt(std::vector<T> const& v, std::string textname);

public:
	double function(double x1, double x2);
	void decoding(std::string x1_s, std::string x2_s, double *x1, double *x2);

	//Get
	double getX1Bit();
	double getX2Bit();
	void run();


	Genetic();
};


template<typename T>
inline void Genetic::makeTxt(std::vector<T> const& v, std::string textname)
{

	std::string f_name = textname + ".txt";
	std::ofstream file(f_name);

	if (file.is_open()) {
		for (int i = 0;i < v.size();i++) {

			file << v.at(i);
			file << "\n";


		}
	}


	file.close();

}