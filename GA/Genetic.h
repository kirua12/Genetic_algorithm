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
	//랜덤 벡터 생성
	void initializePopulation(std::vector<std::string>* bits);
	// 점수에 따라 선택
	void reProduction(std::vector<std::string> *bits, double *F, double *best_F);
	//string 2진수 형태를 double값으로
	double string2Double(std::string str);
	//Fitness를 구하는 함수
	void getFitness(std::vector<std::string> bits, std::vector<double> *fs, double *F);
	void getProbability(std::vector<double> fs, double F, std::vector<double>* ps, std::vector<double>*qs);
	//우수한 유전자만 선택해서 남긴다.
	void select(std::vector<double> qs, std::vector<std::string> *bits);
	// 섞어 주기
	void crossOver(std::vector<std::string>* bits);
	//자르고 섞기
	void cutAndReplace(std::vector<std::string>* bits, std::vector<int> p_cs, std::vector<int> poses);
	//돌연변이 만들기
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