#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <fstream>

#define M_PI 3.14159
using namespace std;

void noise();
string hamming_code(string s);
string hamming_detected(string d);
string hamming_decode(string deco, string noise);
string demo();
void ask(string hamming, int freqmp, int time);
void dft();
bool blokada = false;
vector <float> klucz;

template<class T>
void save_v(vector<T> Wektor, string name)
{
	fstream plik(name, ios_base::out);
	if (plik.good())
	{
		for (unsigned int i = 0; i < Wektor.size(); i++)
		{
			plik << Wektor[i] << std::endl;
		}
		plik.close();
	}
}


void main()
{
	srand(time(0));
	clock_t begin, end;
	double time_spent;
	begin = clock();

	string s;
	cout << "witaj, ";
	bool sem = true;
	do {
		cout << "wpisz 4-cyfrowy kod, ktory zostanie zamieniony na kod hamminga (7,4):\t";
		cin >> s;
		if (s.size() == 4) {
			for (int i = 0; i < s.size(); ++i) {
				if (s[i] == '1' || s[i] == '0') {
					sem = false;
				}
				else {
					sem = true;
					break;
				}
			}
		}
		if (sem) {
			cout << "Twoj kod jest niepoprawny...\n";
		}
	} while (sem);




	string h = hamming_code(s); // zwraca string z kodem hamminga (7,4)

	ask(h, 50, 50);
	dft();
	noise(); // zwraca zmieniony jeden bit
	dft();
	string n = demo();
	string d = hamming_detected(n); // zwraca string po detekcji b³êdów
	string deco = hamming_decode(d, n);

	cout << "kod wejscia:\t" << s << endl
		<< "kod hamminga:\t" << h << endl
		<< "szum demo:\t" << n << endl
		<< "detekcja:\t" << d << endl
		<< "dekodowany:\t" << deco << endl;

	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	cout << "\nCZAS:\t\t" << time_spent << endl;

	getchar();
	getchar();
}


string hamming_code(string s)
{
	string hamming_code;
	string tmp;
	int P[3];

	P[0] = (s[0] + s[1] + s[3]) % 2;
	P[1] = (s[0] + s[2] + s[3]) % 2;
	P[2] = (s[1] + s[2] + s[3]) % 2;

	for (int i = 0, j = 0, m = 0; i < 7; ++i)
	{
		if (i == 0 || i == 1 || i == 3)
		{
			ostringstream ss;
			ss << P[j];
			hamming_code += ss.str();
			++j;
		}
		else
		{
			hamming_code += s[m];
			++m;
		}
	}
	reverse(begin(hamming_code), end(hamming_code));
	return hamming_code;
}

void noise()
{
	float rnd = (rand() % 125)*0.01;
	for (unsigned int i = 0; i < klucz.size(); ++i)
	{
		short int tmp = rand() % 2;
		if (tmp)
			klucz[i] += rnd;
		else
			klucz[i] -= rnd;
	}
}

string hamming_detected(string d)
{
	string hamming_detected;
	int result = 0;
	int hamming_table[3][7] = { { 1,0,1,0,1,0,1 } ,{ 0,1,1,0,0,1,1 } ,{ 0,0,0,1,1,1,1 } };
	int h[3] = { 0,0,0 };

	vector <int> hamm; // size = 7
	for (unsigned int i = 0; i < d.size(); ++i)
	{
		if (d[i] == '1')
			hamm.push_back(1);
		else
			hamm.push_back(0);
	}
	reverse(begin(hamm), end(hamm));
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 7; ++j)
			h[i] += (hamming_table[i][j] * hamm[j]);

	for (int i = 0; i < 3; ++i)
	{
		h[i] = h[i] % 2;
		ostringstream ss;
		ss << h[i];
		hamming_detected += ss.str();
	}
	reverse(begin(hamming_detected), end(hamming_detected));
	return hamming_detected;
}

string hamming_decode(string deco, string noise)
{
	string tmp = deco;
	string klucz = noise;
	reverse(begin(klucz), end(klucz));
	for (unsigned int i = 0; i < tmp.size(); ++i)
		if (tmp[i] == '1')
		{
			if (tmp == "001")
			{
				if (klucz[0] == '1')
					klucz[0] = '0';
				else
					klucz[0] = '1';
				break;
			}
			if (tmp == "010")
			{
				if (klucz[1] == '1')
					klucz[1] = '0';
				else
					klucz[1] = '1';
				break;
			}
			if (tmp == "011")
			{
				if (klucz[2] == '1')
					klucz[2] = '0';
				else
					klucz[2] = '1';
				break;
			}
			if (tmp == "100")
			{
				if (klucz[3] == '1')
					klucz[3] = '0';
				else
					klucz[3] = '1';
				break;
			}
			if (tmp == "101")
			{
				if (klucz[4] == '1')
					klucz[4] = '0';
				else
					klucz[4] = '1';
				break;
			}
			if (tmp == "110")
			{
				if (klucz[5] == '1')
					klucz[5] = '0';
				else
					klucz[5] = '1';
				break;
			}
			if (tmp == "111")
			{
				if (klucz[6] == '1')
					klucz[6] = '0';
				else
					klucz[6] = '1';
				break;
			}
			break;
		}
	reverse(begin(klucz), end(klucz));
	return klucz;
}

void ask(string hamming, int freqmp, int time)
{
	int freq = 25000;
	int samples = freqmp * time;
	int spb = samples / hamming.size();
	vector <int> tmp;
	int tmp_int = 0;
	for (int i = 0; i < samples; ++i)
	{
		if (i%spb == 0 && i != 0)
			++tmp_int;
		if (hamming[tmp_int] == '1')
			tmp.push_back(1);
		else
			tmp.push_back(0);
	}
	for (int i = 0; i < samples; ++i)
	{
		klucz.push_back(tmp[i] * sin(2 * M_PI*time*i*freq / freqmp));
	}
	save_v(klucz, "ask.csv");
}

void dft()
{
	vector<float> real(klucz.size(), 0);
	vector<float> img(klucz.size(), 0);

	vector<float> widA(klucz.size(), 0);
	vector<float> widF(klucz.size(), 0);

	int N = klucz.size();

	for (unsigned int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			real[i] += klucz[j] * cos(2 * M_PI*i*j / N);
			img[i] += klucz[j] * sin(2 * M_PI*i*j / N)*(-1);
		}
	}
	for (unsigned int i = 0; i < N - 1; i++)
	{
		float R = real[i + 1];
		float I = img[i + 1];

		widA[i] = sqrt(R*R + I*I);

		if (real[i] != 0)
			widF[i] = atan(img[i] / real[i]);
	}
	if (!blokada)
	{
		save_v(widA, "Widmo_amp_przed.csv");
		save_v(widF, "Widmo_faz_przed.csv");
		blokada = !blokada;
	}
	else
	{
		save_v(widA, "Widmo_amp_po.csv");
		save_v(widF, "Widmo_faz_po.csv");
		blokada = !blokada;
	}
}

string demo()
{
	string str;
	vector<double> probkowanie;
	float probek_na_bit = klucz.size() / 7;
	int progowanie = 30;
	int numer_probki = 0;
	for (unsigned int i = 0; i < 7; ++i)
	{
		double suma = 0;
		for (unsigned int j = 0; j < probek_na_bit; j++)
		{
			suma += klucz[numer_probki] * klucz[numer_probki + 1];
			probkowanie.push_back(suma);
			numer_probki++;
		}
		if (suma >= progowanie)
		{
			str += "1";
		}
		else
		{
			str += "0";
		}
	}
	save_v(probkowanie, "demodulacja.csv");
	return str;
}