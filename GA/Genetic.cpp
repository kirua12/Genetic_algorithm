#include "Genetic.h"


Genetic::Genetic() {
	m_x1_bit_size = 18;
	m_x2_bit_size = 15;
	m_min_x2 = 4.1;
	m_min_x1 = -3;
	m_max_x1 = 12.1;
	m_max_x2 = 5.8;


	m_pop_size = 500;
	m_p_c = 0.25;
	m_p_m = 0;

	m_epoch = 1000;

}
void Genetic::initializePopulation(std::vector<std::string> *bits)
{

	srand((unsigned int)time(NULL));

	int bit_size = m_x1_bit_size + m_x2_bit_size;
	for (int i = 0; i < m_pop_size;i++) {
		std::string bit = "";
		for (int j = 0; j < bit_size;j++) {
			int bit_int = rand() % 2;
			bit += std::to_string(bit_int);
		}		
		
		(*bits).push_back(bit);

	}


}
void Genetic::reProduction(std::vector<std::string>* bits, double* F, double* best_F)
{
	//eval의 모음
	std::vector<double> fs;
	double F_t = 0;
	getFitness((*bits), &fs, &F_t);
	//각 벡터를 저장하는 함수
	std::vector<double> ps;
	//각각 n번째의 확률을 선택할수 있는 확률값
	std::vector<double> qs;

	getProbability(fs, F_t, &ps, &qs);
	select(qs, bits);

	(*best_F) = *max_element(fs.begin(), fs.end());
	(*F) = F_t;
}
double Genetic::string2Double(std::string str)
{
	double x1 = 0;
	for (int i = 0; i < str.length();i++) {
		int order = str.length() - i-1;
		std::string temp = str.substr(i, 1);
		if (temp == "1")
			x1 += std::pow(2, order);
	}

	return x1;
}

void Genetic::getFitness(std::vector<std::string> bits, std::vector<double>* fs,double *F)
{
	//Fitness를 구하는 과정
	for (int i = 0; i < bits.size();i++) {
		//bit string
		std::string x1_s = bits.at(i).substr(0, m_x1_bit_size);
		std::string x2_s = bits.at(i).substr(m_x1_bit_size, m_x2_bit_size);
		double x1;
		double x2;
		decoding(x1_s, x2_s, &x1, &x2);
		//점수를 구한다
		double f = function(x1, x2);
		(*fs).push_back(f);
		(*F) += f;

	}
}
void Genetic::getProbability(std::vector<double> fs, double F, std::vector<double>* ps, std::vector<double>* qs)
{
	double sum = 0;
	for (int i = 0; i < fs.size();i++) {
		double probability = fs.at(i) / F;
		(*ps).push_back(probability);
		sum += probability;
		(*qs).push_back(sum);
	}

}
void Genetic::select(std::vector<double> qs, std::vector<std::string> *bits)
{
	std::vector<std::string> temp = (*bits);
	srand((unsigned int)time(NULL));

	for (int i = 0;i < qs.size();i++) {
		double probability = (double)std::rand() /RAND_MAX;
		for (int j = 0;j < qs.size();j++) {
			if (probability < qs.at(j)) {
				temp.at(i) = temp.at(j);
				break;
			}
		}
	}
	(*bits) = temp;

}
void Genetic::crossOver(std::vector<std::string>* bits)
{
	int bit_size = m_x1_bit_size + m_x2_bit_size;
	srand((unsigned int)time(NULL));

	std::vector<double> randoms;
	// 0~1까지의 랜덤 변수 선택
	for (int i = 0;i < m_pop_size;i++) {
		double random = (double)std::rand() / RAND_MAX;
		randoms.push_back(random);
	}

	std::vector<int> p_cs;
	std::vector<int> poses;


	//섞어줄 벡터를 p_c보다 작은 random으로 결정
	for (int i = 0; i < randoms.size();i++) {
		if (randoms.at(i) < m_p_c)
			p_cs.push_back(i);
	}

	//어느 부분부터 섞어줄 random부분 선택
	for (int i = 0; i < p_cs.size()/2; i++) {
		int pos = std::rand() % (bit_size - 1) + 1;
		poses.push_back(pos);
	}

	cutAndReplace(bits, p_cs, poses);
}
void Genetic::cutAndReplace(std::vector<std::string>* bits, std::vector<int> p_cs, std::vector<int> poses)
{
	std::vector<std::string> temp = (*bits);


	//cut and Replace
	for (int i = 0; i+1 < p_cs.size();i += 2) {
		int pos = poses.at(i / 2);
		std::string v_1 = temp.at(p_cs.at(i));
		std::string v_2 = temp.at(p_cs.at(i+1));
		std::string v_1_1 = v_1.substr(0, pos);
		std::string v_1_2 = v_1.substr(pos);
		std::string v_2_1 = v_2.substr(0, pos);
		std::string v_2_2 = v_2.substr(pos);

		v_1 = v_1_1 + v_2_2;
		v_2 = v_2_1 + v_1_2;
		temp.at(p_cs.at(i)) = v_1;
		temp.at(p_cs.at(i+1)) = v_2;

	}


	(*bits) = temp;
}
void Genetic::mutate(std::vector<std::string>* bits)
{

	int bit_size = m_x1_bit_size + m_x2_bit_size;
	int total_size = bit_size * m_pop_size;
	srand((unsigned int)time(NULL));

	//총 비트수만큼 random변수 찾기
	std::vector<double> randoms;
	for (int i = 0;i < total_size;i++) {
		double random = (double)std::rand() / RAND_MAX;
		randoms.push_back(random);
	}

	//threshold보다 작은 변종 비트수를 저장한다.
	std::vector<int> mutation;
	for (int i = 0;i < total_size;i++) {
		if (randoms.at(i) < m_p_m)
			mutation.push_back(i);
	}

	for (int i = 0;i < mutation.size();i++) {
		
		int index = mutation.at(i) / bit_size;
		int pos = mutation.at(i) - bit_size *index;
		std::string m_string = (*bits).at(index);

		std::string replace_string = "1";
		if(m_string.substr(pos,1)=="1")
			replace_string = "0";
		m_string.replace(pos, 1, replace_string);
		(*bits).at(index) = m_string;

	}



}

double Genetic::function(double x1, double x2)
{
	double Pi = std::acos(-1);
	double result = 21.5 + x1 * std::sin(4 * Pi * x1) + x2 * std::sin(20 * Pi * x2);
	return result;
}
void Genetic::decoding(std::string x1_s, std::string x2_s, double *x1, double *x2)
{
	double decimal_x1 = string2Double(x1_s);
	double decimal_x2 = string2Double(x2_s);

	(*x1) = m_min_x1 + decimal_x1 * (m_max_x1 - m_min_x1) / (std::pow(2, m_x1_bit_size) - 1);
	(*x2) = m_min_x2 + decimal_x2 * (m_max_x2 - m_min_x2) / (std::pow(2, m_x2_bit_size) - 1);

}
double Genetic::getX1Bit()
{
	return m_x1_bit_size;
}
double Genetic::getX2Bit()
{
	return m_x2_bit_size;
}
void Genetic::run()
{
	std::vector<std::string> bits;
	initializePopulation(&bits);
	std::vector<double> totals;
	std::vector<double> bests;

	for (int i = 0;i < m_epoch;i++) {

		double total_F;
		double best_F;

		reProduction(&bits,&total_F,&best_F);
		totals.push_back(total_F/m_pop_size);
		bests.push_back(best_F);

		crossOver(&bits);
		mutate(&bits);
	}

	makeTxt(totals, "total");
	makeTxt(bests, "best");
}