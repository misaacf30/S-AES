// Miguel Fernandez
// Project: S-AES
// Program can encrypt and decrypt using S-AES. Test data: A binary plaintext of
// 0110 1111 0110 1011 encrypted with a binary key of 1010 0111 0011 1011 should give a binary
// ciphertext of 0000 0111 0011 1000. Decryption should work correspondingly.

#include <iostream>
#include <string>
#include <vector>
using namespace std;

std::vector<int> XOR(vector<int> st1, vector<int> st2);
void addRoundKey(vector<int>& st1, vector<int>& st2, vector<int>& st3, vector<int>& st4, vector<int> key, int round);
std::vector<int> sBox(vector<int> intext);
std::vector<int> invsBox(vector<int> intext);
std::vector<int> gFunction(vector<int> word);
std::vector<int> keyExpand(vector<int> key);
void shiftRows(vector<int>& st2, vector<int>& st3);
std::vector<int> mixColMult(vector<int> win);
std::vector<int> invMixColMult9(vector<int> win);
std::vector<int> invMixColMult2(vector<int> win);
void mixColumns(vector<int>& win1, vector<int>& win2, vector<int>& win3, vector<int>& win4);
void invMixColumns(vector<int>& win1, vector<int>& win2, vector<int>& win3, vector<int>& win4);


int main()
{
	string bKey = "1010 0111 0011 1011";
	vector<int> key{ 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1 };
	vector<int> keye = keyExpand(key);

	string pt = "0110 1111 0110 1011";
	vector<int>	pt1{ 0, 1, 1, 0 };
	vector<int>	pt2{ 1, 1, 1, 1 };
	vector<int>	pt3{ 0, 1, 1, 0 };
	vector<int>	pt4{ 1, 0, 1, 1 };

	cout << "FINAL PROJECT: S-AES\n\n";

	cout << "Binary plaintext:                        " << pt << endl;
	cout << "Binary key:                              " << bKey << endl;



	// ENCRYPTION	
	addRoundKey(pt1, pt2, pt3, pt4, keye, 0);		// Round 0

	pt1 = sBox(pt1);
	pt2 = sBox(pt2);
	pt3 = sBox(pt3);
	pt4 = sBox(pt4);

	shiftRows(pt2, pt4);
	mixColumns(pt1, pt2, pt3, pt4);
	addRoundKey(pt1, pt2, pt3, pt4, keye, 1);		// Round 1

	pt1 = sBox(pt1);
	pt2 = sBox(pt2);
	pt3 = sBox(pt3);
	pt4 = sBox(pt4);

	shiftRows(pt2, pt4);
	addRoundKey(pt1, pt2, pt3, pt4, keye, 2);		// Round 2

	cout << "\nCiphertext after S-AES encryption:       ";
	for (int x : pt1)
		cout << x;
	cout << " ";
	for (int x : pt2)
		cout << x;
	cout << " ";
	for (int x : pt3)
		cout << x;
	cout << " ";
	for (int x : pt4)
		cout << x;
	cout << endl;


	// DECRYPTION
	vector<int> ct1, ct2, ct3, ct4;
	ct1 = pt1;
	ct2 = pt2;
	ct3 = pt3;
	ct4 = pt4;

	addRoundKey(ct1, ct2, ct3, ct4, keye, 2);		// Round 2
	shiftRows(ct2, ct4);

	ct1 = invsBox(ct1);
	ct2 = invsBox(ct2);
	ct3 = invsBox(ct3);
	ct4 = invsBox(ct4);

	addRoundKey(ct1, ct2, ct3, ct4, keye, 1);		// Round 1
	invMixColumns(ct1, ct2, ct3, ct4);
	shiftRows(ct2, ct4);

	ct1 = invsBox(ct1);
	ct2 = invsBox(ct2);
	ct3 = invsBox(ct3);
	ct4 = invsBox(ct4);

	addRoundKey(ct1, ct2, ct3, ct4, keye, 0);		// Round 0

	cout << "\nPlaintext after S-AES decryption:        ";
	for (int x : ct1)
		cout << x;
	cout << " ";
	for (int x : ct2)
		cout << x;
	cout << " ";
	for (int x : ct3)
		cout << x;
	cout << " ";
	for (int x : ct4)
		cout << x;
	cout << endl << endl;


	system("pause");
	return 0;
}

// XOR function
std::vector<int> XOR(vector<int> st1, vector<int> st2) {
	vector<int> st3;
	for (int i = 0; i < st1.size(); i++)
		st3.push_back(0);

	for (int i = 0; i < st1.size(); i++)
	{
		int sum = st1[i] + st2[i];
		if (sum != 1)
			st3[i] = 0;
		else
			st3[i] = 1;
	}
	return st3;
}

// Add Round Key function
void addRoundKey(vector<int>& st1, vector<int>& st2, vector<int>& st3, vector<int>& st4, vector<int> key, int round)
{
	vector<int> stf, keyf;

	for (int i = 0; i < 4; i++)
		stf.push_back(st1[i]);
	for (int i = 0; i < 4; i++)
		stf.push_back(st2[i]);
	for (int i = 0; i < 4; i++)
		stf.push_back(st3[i]);
	for (int i = 0; i < 4; i++)
		stf.push_back(st4[i]);

	for (int i = 0; i < 16; i++)
		keyf.push_back(key[i + (round * 16)]);

	stf = XOR(stf, keyf);

	for (int i = 0; i < 4; i++)
		st1[i] = stf[i];
	for (int i = 0; i < 4; i++)
		st2[i] = stf[i + 4];
	for (int i = 0; i < 4; i++)
		st3[i] = stf[i + 8];
	for (int i = 0; i < 4; i++)
		st4[i] = stf[i + 12];
}

// Shift Rows function
void shiftRows(vector<int>& st2, vector<int>& st3)
{
	vector<int> temp;
	temp = st3;
	st3 = st2;
	st2 = temp;
}

// Mix Columns Mult function
std::vector<int> mixColMult(vector<int> win)
{
	int col = (win[0] * 8) + (win[1] * 4) + (win[2] * 2) + win[3];
	vector<int> wout;
	vector<vector<int>> mult = { {0,0,0,0}, {0,1,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,1,1},
								{0,1,1,1}, {1,0,1,1},{1,1,1,1},{0,1,1,0},{0,0,1,0},
								{1,1,1,0}, {1,0,1,0},{0,1,0,1},{0,0,0,1},{1,1,0,1},
								{1,0,0,1} };
	wout = mult[col];
	return wout;
}

// Mix Columns Mult9 function
std::vector<int> invMixColMult9(vector<int> win)
{
	int col = (win[0] * 8) + (win[1] * 4) + (win[2] * 2) + win[3];
	vector<int> wout;
	vector<vector<int>> mult = { {0,0,0,0}, {1,0,0,1}, {0,0,0,1}, {1,0,0,0}, {0,0,1,0},
								{1,0,1,1}, {0,0,1,1},{1,0,1,0},{0,1,0,0},{1,1,0,1},
								{0,1,0,1}, {1,1,0,0},{0,1,1,0},{1,1,1,1},{0,1,1,1},
								{1,1,1,0} };
	wout = mult[col];
	return wout;
}

// Mix Columns Mult2 function
std::vector<int> invMixColMult2(vector<int> win)
{
	int col = (win[0] * 8) + (win[1] * 4) + (win[2] * 2) + win[3];
	vector<int> wout;
	vector<vector<int>> mult = { {0,0,0,0}, {0,0,1,0}, {0,1,0,0}, {0,1,1,0}, {1,0,0,0},
								{1,0,1,0}, {1,1,0,0},{1,1,1,0},{0,0,1,1},{0,0,0,1},
								{0,1,1,1}, {0,1,0,1},{1,0,1,1},{1,0,0,1},{1,1,1,1},
								{1,1,0,1} };
	wout = mult[col];
	return wout;
}

// Mix Columns function
void mixColumns(vector<int>& win1, vector<int>& win2, vector<int>& win3, vector<int>& win4)
{
	vector<int> fwin1, fwin2, fwin3, fwin4;

	fwin1 = mixColMult(win1);
	fwin2 = mixColMult(win2);
	fwin3 = mixColMult(win3);
	fwin4 = mixColMult(win4);

	win4 = XOR(win4, fwin3);
	win3 = XOR(win3, fwin4);
	win2 = XOR(win2, fwin1);
	win1 = XOR(win1, fwin2);
}

// Mix Columns function
void invMixColumns(vector<int>& win1, vector<int>& win2, vector<int>& win3, vector<int>& win4)
{
	vector<int> fwin1, fwin2, fwin3, fwin4;
	vector<int> wout1, wout2, wout3, wout4;

	fwin1 = invMixColMult2(win1);
	fwin2 = invMixColMult2(win2);
	fwin3 = invMixColMult2(win3);
	fwin4 = invMixColMult2(win4);

	win1 = invMixColMult9(win1);
	win2 = invMixColMult9(win2);
	win3 = invMixColMult9(win3);
	win4 = invMixColMult9(win4);

	win3 = XOR(win3, fwin4);
	win4 = XOR(win4, fwin3);
	win1 = XOR(win1, fwin2);
	win2 = XOR(win2, fwin1);
}

// S-box function
std::vector<int> sBox(vector<int> intext)
{
	int vRow = (intext[0] * 2) + intext[1];
	int vCol = (intext[2] * 2) + intext[3];

	vector<int> rt;
	vector<int> vRow0{ 1,0,0,1,0,1,0,0,1,0,1,0,1,0,1,1 };
	vector<int> vRow1{ 1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1 };
	vector<int> vRow2{ 0,1,1,0,0,0,1,0,0,0,0,0,0,0,1,1 };
	vector<int> vRow3{ 1,1,0,0,1,1,1,0,1,1,1,1,0,1,1,1 };

	if (vRow == 0)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow0[vCol * 4 + i]);
	}
	if (vRow == 1)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow1[vCol * 4 + i]);
	}
	if (vRow == 2)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow2[vCol * 4 + i]);
	}
	if (vRow == 3)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow3[vCol * 4 + i]);
	}

	return rt;
}

// inv S-Box function
std::vector<int> invsBox(vector<int> intext)
{
	int vRow = (intext[0] * 2) + intext[1];
	int vCol = (intext[2] * 2) + intext[3];

	vector<int> rt;
	vector<int> vRow0{ 1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,1 };
	vector<int> vRow1{ 0,0,0,1,0,1,1,1,1,0,0,0,1,1,1,1 };
	vector<int> vRow2{ 0,1,1,0,0,0,0,0,0,0,1,0,0,0,1,1 };
	vector<int> vRow3{ 1,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0 };

	if (vRow == 0)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow0[vCol * 4 + i]);
	}
	if (vRow == 1)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow1[vCol * 4 + i]);
	}
	if (vRow == 2)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow2[vCol * 4 + i]);
	}
	if (vRow == 3)
	{
		for (int i = 0; i < 4; i++)
			rt.push_back(vRow3[vCol * 4 + i]);
	}

	return rt;
}

// g function
std::vector<int> gFunction(vector<int> word)
{
	vector<int> n0, n1, wp;

	for (int i = 0; i < 4; i++)
		n0.push_back(word[i]);
	for (int i = 4; i < 8; i++)
		n1.push_back(word[i]);

	n1 = sBox(n1);
	n0 = sBox(n0);

	for (int i = 0; i < 4; i++)
		wp.push_back(n1[i]);
	for (int i = 0; i < 4; i++)
		wp.push_back(n0[i]);

	return wp;
}

// Key Expand function
std::vector<int> keyExpand(vector<int> key)
{
	vector<int> rc1{ 1,0,0,0,0,0,0,0 };
	vector<int> rc2{ 0,0,1,1,0,0,0,0 };

	vector<int> w0, w1, w2, w3, w4, w5, temp, temp1, keyf;

	for (int i = 0; i < 8; i++)
		w0.push_back(key[i]);
	for (int i = 8; i < 16; i++)
		w1.push_back(key[i]);
	for (int i = 0; i < 16; i++)
		keyf.push_back(key[i]);

	temp = gFunction(w1);
	temp = XOR(temp, rc1);
	w2 = XOR(temp, w0);

	for (int i = 0; i < 8; i++)
		keyf.push_back(w2[i]);

	w3 = XOR(w2, w1);

	for (int i = 0; i < 8; i++)
		keyf.push_back(w3[i]);

	temp = gFunction(w3);
	temp = XOR(temp, rc2);

	w4 = XOR(temp, w2);
	w5 = XOR(w4, w3);
	for (int i = 0; i < 8; i++)
		keyf.push_back(w4[i]);
	for (int i = 0; i < 8; i++)
		keyf.push_back(w5[i]);

	return keyf;
}
