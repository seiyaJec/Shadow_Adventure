#pragma once
//----------------------------------------------
//���y�Đ����C�u����
// DirectSound DirectShow �ėp�֐�
//      k.koyanagi
//----------------------------------------------
// 2022.07.21 �o�O�C����
//----------------------------------------------

#include <assert.h> 
#include <string>
#include <dshow.h>
#include <dsound.h>

/*
how to use

�v���W�F�N�g�ɗ��t�@�C����ǉ�

���p������CPP��
#include "sound.h"
��ǉ�

------------bgm------------------
�Ή��`����DirectShow�ɏ�����


�N������
bgm::Init();

�I������
bgm::Del();

�t�@�C���̓ǂݍ���
���p�O�Ɉ�񂾂�
bgm::LoadFile("testbgm","./sound/test.mp3");

�Đ�
bgm::Play("testbgm");

���t���[���s������
bgm::EndCheck();
------------------------------

------------se------------------
�Ή��`����DirectSound�ɏ�����


�N������
se::Init();

�I������
se::Del();

�t�@�C���̓ǂݍ���
���p�O�Ɉ�񂾂�
se::LoadFile("testse","./sound/test.wav");

�Đ�
se::Play("testbgm");

���t���[���s������
se::EndCheck();
------------------------------





*/






namespace bgm{
	//BGM�Đ��֐�
	void Init();
	void Del();
	void LoadFile(const std::string& resname, const std::string& filename);
	bool Play(const std::string& resname);
	void RateControl(const std::string& resname, float rate);
	void EndCheck();
	void Stop(const std::string& resname);
	void AllStop();
	LONGLONG GetCurrentPos(const std::string& resname);
	void SetStartPos(const std::string& resname);
	void VolumeControl(const std::string& resname, int volume);
	void Pause(const std::string& resname);
	LONGLONG GetEndPosition(const std::string& resname);
}
namespace se{
	//SE�Đ��֐�
	void Init(HWND);
	bool Create(HWND);
	void CreateSecondaryBuffer();
	bool CreatePrimaryBuffer();
	bool Del();
	void Play(const std::string& resname);
	void PlayLoop(const std::string& resname);
	bool Del();
	void LoadFile(const std::string& resname, const std::string& filename);
	void AllStop();
	void Stop(const std::string& resname);
	void EndCheck();
	void SetFrequency(const std::string& resname, int Fre);
	void SetVolume(const std::string& resname, int Vol);
}


