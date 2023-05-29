#pragma once

#include <random>
#include <math.h>

extern void InitRandom();

int  SubGetRandom(int min_, int max_);
short  SubGetRandom(short min_, short max_);
float  SubGetRandom(float min_, float max_);
double  SubGetRandom(double min_, double max_);

// [min, max]�̈�l�ȗ������擾
//�e���v���[�g�̓��ꉻ��
template <typename Type> extern  Type GetRandom(Type minNum_, Type maxNum_){

	Type real_min = (minNum_ < maxNum_ ? minNum_ : maxNum_);
	Type real_max = (minNum_ > maxNum_ ? minNum_ : maxNum_);

	return SubGetRandom(real_min, real_max);
}






