//----------------------------------------------
//���y�Đ����C�u����
// DirectSound DirectShow �ėp�֐�
//      k.koyanagi
//----------------------------------------------
// 2022.07.21 �o�O�C����
//----------------------------------------------
#pragma	comment(lib,"winmm")
#pragma	comment(lib,"dinput8.lib")
#pragma comment(lib,"strmiids")
#pragma	comment(lib,"dsound.lib")

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <string>

#include "sound.h"
#include "unordered_map"

//-------------------------------------------------------
//BGM�Đ��֘A
struct DxShow{
	IGraphBuilder	*pBuilder;
	IMediaControl	*pMediaControl;
	IMediaEvent		*pMediaEvent;
	IMediaSeeking	*pMediaSeeking;
	IBasicAudio		*pBasicAudio;

	int				use;
};
//BGM�Ǘ��\����
struct BgmObj
{
	DxShow				bgm;
	std::string		filename;
};
std::unordered_map< std::string, BgmObj>	BgmTable;

//-------------------------------------------------------
//SE�Đ��֘A
// DirectSound8
LPDIRECTSOUND8          lpDS;
// �v���C�}���T�E���h�o�b�t�@
LPDIRECTSOUNDBUFFER		lpPrimary;

//���������ł���ő吔	
enum					{ DSNONE, DSSTART, DSPLAY, DSSTOP, DSEND };
struct PlayBuffer{
	LPDIRECTSOUNDBUFFER     pBuffer;		// �v���C�}���T�E���h�o�b�t�@
	int						ID;				//�x�[�X�ɂȂ����o�b�t�@��ID
	WORD					BufferSize;		//�x�[�X�ɂȂ����o�b�t�@�̃T�C�Y
	int						State;			//�o�b�t�@�̏��
	bool					Loop;			//���[�v�t���O
};

const int				DSPLAYMAX = 10;
struct SoundBuffer{
	LPDIRECTSOUNDBUFFER     sBuffer;	//�Z�J���_���o�b�t�@
	WORD										BufferSize;		//�o�b�t�@�̃T�C�Y
	DWORD										Fre;
	struct PlayBuffer	PlayBuffer[DSPLAYMAX];
};
struct SeObj
{
	SoundBuffer		se;
	std::string		filename;
};

std::unordered_map< std::string, SeObj>		SeTable;



//--------------------------------------------------------------------------------------------
// DirectShow�̏�����
//--------------------------------------------------------------------------------------------
void ReleaseObj(DxShow& bgm){
	bgm.pMediaControl->Release();
	bgm.pMediaEvent->Release();
	bgm.pMediaSeeking->Release();
	bgm.pBuilder->Release();
	bgm.pBasicAudio->Release();
}

void	bgm::LoadFile(const std::string& resname, const std::string& filename)
{
	//���ɓǂݍ��܂�Ă��郊�\�[�X���Ȃ�A�G���[
	if (BgmTable.count(filename) != 0){
		assert(!"�������O�̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
	}
	//for (const auto& bgm: BgmTable){
	//	if (bgm.second.filename == filename){
	//		return;
	//	}
	//}
	BgmObj bgmObj;
	GUID format = TIME_FORMAT_FRAME;
	wchar_t name[256];
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)filename.c_str(), -1, name, 256);

	HRESULT	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
																IID_IGraphBuilder, (void **)&bgmObj.bgm.pBuilder);
	if (FAILED(hr)){
		assert(!"DShow:GraphBuilder�I�u�W�F�N�g�̐����Ɏ��s���܂���");
		goto error;
	}

	if (FAILED(bgmObj.bgm.pBuilder->RenderFile(name, NULL))){
		assert(!"DShow\n�t�@�C���ǂݍ��݂Ɏ��s���܂���\n�t�@�C�����A�R�[�f�b�N�̊m�F�����肢���܂��B");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaControl,
		(void**)&bgmObj.bgm.pMediaControl))){
		assert(!"DShow\nMediaControl�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaEvent,
		(void**)&bgmObj.bgm.pMediaEvent))){
		assert(!"DShow\nMediaEvent�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IMediaSeeking,
		(void**)&bgmObj.bgm.pMediaSeeking))){
		assert(!"DShow\nMediaSeeking�̐ݒ�Ɏ��s���܂���");
		goto error;
	}
	if (FAILED(bgmObj.bgm.pBuilder->QueryInterface(IID_IBasicAudio,
		(void**)&bgmObj.bgm.pBasicAudio))){
		assert(!"DShow\nBasicAudio�̐ݒ�Ɏ��s���܂���");
		goto error;
	}

	bgmObj.bgm.pMediaSeeking->SetTimeFormat(&format);
	bgmObj.bgm.use = 1;
	bgmObj.filename = filename;
	BgmTable.insert(std::unordered_map<std::string, BgmObj>::value_type(resname, bgmObj));
	return;

error:
	ReleaseObj(bgmObj.bgm);

}
//--------------------------------------------------------------------------------------------
// DirectShow�̌�n��
//--------------------------------------------------------------------------------------------
void	bgm::Del()
{
	bgm::AllStop();
	for (auto& bgm : BgmTable){
		if (bgm.second.bgm.use == 1){
			ReleaseObj(bgm.second.bgm);
			bgm.second.bgm.use = 0;
		}
	}
	CoUninitialize();
}

//--------------------------------------------------------------------------------------------
// GraphBuilder�̐���
//--------------------------------------------------------------------------------------------
void bgm::Init()
{
	CoInitialize(NULL);
}

//--------------------------------------------------------------------------------------------
// �t�@�C���̍Đ�
//--------------------------------------------------------------------------------------------
bool	bgm::Play(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return false;
	HRESULT	hr = BgmTable.at(resname).bgm.pMediaControl->Run();
	if (FAILED(hr)){
		assert(!"DShow:�Đ��Ɏ��s���܂���1");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------
// �Đ����x�̍Đ�
//--------------------------------------------------------------------------------------------
void bgm::RateControl(const std::string& resname, float rate)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (rate < 0)	return;
	BgmTable.at(resname).bgm.pMediaSeeking->SetRate(rate);
}
//--------------------------------------------------------------------------------------------
// �Đ����ʂ̐ݒ�
// 0����100�Őݒ�@0�͖��� 100�͍ő� 
//--------------------------------------------------------------------------------------------
void bgm::VolumeControl(const std::string& resname, int volume)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (volume < 0 || volume > 100)	return;
	//0����-10000�Őݒ� 0���ő�@-10000�͖���
	//100�Ŋ������l���f�V�x��
	long vol = (long)(-10000 + (volume * 100));
	BgmTable.at(resname).bgm.pBasicAudio->put_Volume(vol);
}
//--------------------------------------------------------------------------------------------
// ��~
//--------------------------------------------------------------------------------------------
void bgm::Stop(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	LONGLONG	start = 0;
	BgmTable.at(resname).bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
																											 NULL, AM_SEEKING_NoPositioning);
	BgmTable.at(resname).bgm.pMediaControl->Stop();
}

//--------------------------------------------------------------------------------------------
// �ꎞ��~
//--------------------------------------------------------------------------------------------
void bgm::Pause(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	if (BgmTable.at(resname).bgm.use == 1){
		BgmTable.at(resname).bgm.pMediaControl->Stop();
	}
}

//--------------------------------------------------------------------------------------------
// �S��~
//--------------------------------------------------------------------------------------------
void bgm::AllStop()
{
	for (auto& bgm : BgmTable){
		if (bgm.second.bgm.use == 1){
			bgm.second.bgm.pMediaControl->Stop();
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���m�F
//--------------------------------------------------------------------------------------------
void bgm::EndCheck()
{
	for (auto& bgm : BgmTable){
		if (bgm.second.bgm.use == 1){
			//			long code;
			//�Đ��ʒu�擾
			LONGLONG endPos = bgm::GetEndPosition(bgm.first);
			LONGLONG nowPos = bgm::GetCurrentPos(bgm.first);
			//�u���b�L���O����悤�Ȃ̂ł��
			//bgm.second.bgm.pMediaEvent->WaitForCompletion(0, &code);
			//�Đ��I�����̓��[�v���s���B
			//			if(code == EC_COMPLETE){
			if (endPos <= nowPos){
				LONGLONG	start = 0;
				bgm.second.bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
																									 NULL, AM_SEEKING_NoPositioning);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �Đ��I���ʒu�擾
//--------------------------------------------------------------------------------------------
LONGLONG bgm::GetEndPosition(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return -1L;
	LONGLONG	end;
	//�I���ʒu�̎擾
	BgmTable.at(resname).bgm.pMediaSeeking->GetStopPosition(&end);
	return end;
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu�擾
//--------------------------------------------------------------------------------------------
LONGLONG bgm::GetCurrentPos(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return -1L;
	LONGLONG	Current;
	BgmTable.at(resname).bgm.pMediaSeeking->GetCurrentPosition(&Current);
	return Current;
}

//--------------------------------------------------------------------------------------------
// �Đ��ʒu������
//--------------------------------------------------------------------------------------------
void bgm::SetStartPos(const std::string& resname)
{
	if (BgmTable.at(resname).bgm.use == 0) return;
	LONGLONG	start = 0;
	BgmTable.at(resname).bgm.pMediaSeeking->SetPositions(&start, AM_SEEKING_AbsolutePositioning,
																											 NULL, AM_SEEKING_NoPositioning);
}


//********************************************************************//
//
//				BGM�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//

//********************************************************************//
//
//				SE�Đ��@�֘A�֐�
//
//********************************************************************//
//--------------------------------------------------------------------------------------------
// DirectSound�̏�����
//--------------------------------------------------------------------------------------------
void se::Init(HWND hwnd_)
{
	if (se::Create(hwnd_)){
		if (!CreatePrimaryBuffer()){
			assert(!"�v���C�}���o�b�t�@�̐����Ɏ��s���܂���");
		}
		CreateSecondaryBuffer();
	}
	else{
		assert(!"DSound�̐����Ɏ��s���܂���");
	}

}

//--------------------------------------------------------------------------------------------
// DirectSound�̍쐬
//--------------------------------------------------------------------------------------------
bool se::Create(HWND hwnd_)
{
	HRESULT ret;

	// DirectSound8���쐬
	ret = DirectSoundCreate8(NULL, &lpDS, NULL);
	if (FAILED(ret)) {
		assert(!"�T�E���h�I�u�W�F�N�g�쐬���s\n");
		return false;
	}

	// �������[�h
	ret = lpDS->SetCooperativeLevel(hwnd_, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
	if (FAILED(ret)) {
		assert(!"�������x���ݒ莸�s");
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
// �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
void se::CreateSecondaryBuffer()
{
	SeTable.clear();
}

//--------------------------------------------------------------------------------------------
// �T�E���h�Đ�
//--------------------------------------------------------------------------------------------
void se::Play(const std::string& resname)
{
	int i;
	auto& se = SeTable.at(resname).se;
	for (i = 0; i < DSPLAYMAX; i++){
		if (se.PlayBuffer[i].State == DSNONE){
			//�Đ��J�n
			//			se.PlayBuffer[i].ID = SoundNo;
			se.PlayBuffer[i].State = DSPLAY;
			se.PlayBuffer[i].pBuffer->Play(0, 0, 0);
            se.PlayBuffer[i].Loop = 0;
            se.PlayBuffer[i].pBuffer->SetCurrentPosition(0);
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h���[�v�Đ�
//--------------------------------------------------------------------------------------------
void se::PlayLoop(const std::string& resname)
{
	int i;
	auto& se = SeTable.at(resname).se;
	for (i = 0; i < DSPLAYMAX; i++){
		if (se.PlayBuffer[i].State == DSNONE){
			//�Đ��J�n
			//			se.PlayBuffer[i].ID = SoundNo;
			se.PlayBuffer[i].State = DSPLAY;
			se.PlayBuffer[i].pBuffer->Play(0, 0, 0);
			se.PlayBuffer[i].Loop = 1;
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
// �T�E���h��~
//	�������ʉ������ׂĒ�~����
//--------------------------------------------------------------------------------------------
void se::Stop(const std::string& resname)
{
	auto& se = SeTable.at(resname).se;
	for (int j = 0; j < DSPLAYMAX; j++){
		if (se.PlayBuffer[j].State == DSPLAY){
			se.PlayBuffer[j].pBuffer->Stop();
			se.PlayBuffer[j].State = DSNONE;
		}
	}
}

//--------------------------------------------------------------------------------------------
// ���ׂẴT�E���h��~
//--------------------------------------------------------------------------------------------
void se::AllStop()
{
	for (auto& seObj : SeTable){
		for (int j = 0; j < DSPLAYMAX; j++){
			if (seObj.second.se.PlayBuffer[j].State == DSPLAY){
				seObj.second.se.PlayBuffer[j].pBuffer->Stop();
				seObj.second.se.PlayBuffer[j].State = DSNONE;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------
// �I���`�F�b�N
//--------------------------------------------------------------------------------------------
void se::EndCheck()
{
	DWORD Status;
	int a = 0;
	for (auto& seObj : SeTable){
		for (int j = 0; j < DSPLAYMAX; j++){
			if (seObj.second.se.PlayBuffer[j].State == DSPLAY){		//���ݍĐ����̕����o�b�t�@
				//�I���ƔF������
				seObj.second.se.PlayBuffer[j].pBuffer->GetStatus(&Status);
				if ((Status & DSBSTATUS_PLAYING) != 1){
					if (seObj.second.se.PlayBuffer[j].Loop){
						seObj.second.se.PlayBuffer[j].pBuffer->SetCurrentPosition(0);
						seObj.second.se.PlayBuffer[j].pBuffer->Play(0, 0, 0);
					}
					else{
						seObj.second.se.PlayBuffer[j].State = DSNONE;
						seObj.second.se.PlayBuffer[j].BufferSize = 0;
						seObj.second.se.PlayBuffer[j].pBuffer->Stop();
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
// �v���C�}���T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
bool se::CreatePrimaryBuffer(void)
{
	HRESULT ret;
	WAVEFORMATEX wf;

	// �v���C�}���T�E���h�o�b�t�@�̍쐬
	DSBUFFERDESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));
	dsdesc.dwSize = sizeof(DSBUFFERDESC);
	dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsdesc.dwBufferBytes = 0;
	dsdesc.lpwfxFormat = NULL;
	ret = lpDS->CreateSoundBuffer(&dsdesc, &lpPrimary, NULL);
	if (FAILED(ret)) {
		assert(!"�v���C�}���T�E���h�o�b�t�@�쐬���s\n");
		return false;
	}

	// �v���C�}���o�b�t�@�̃X�e�[�^�X������
	wf.cbSize = sizeof(WAVEFORMATEX);
	wf.wFormatTag = WAVE_FORMAT_PCM;
	wf.nChannels = 2;
	wf.nSamplesPerSec = 44100;
	wf.wBitsPerSample = 16;
	wf.nBlockAlign = wf.nChannels * wf.wBitsPerSample / 8;
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
	ret = lpPrimary->SetFormat(&wf);
	if (FAILED(ret)) {
		assert(!"�v���C�}���o�b�t�@�̃X�e�[�^�X���s\n");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------------
//DSound�̌�n��
//--------------------------------------------------------------------------------------------
bool se::Del(void)
{
	for (auto& seObj : SeTable){
		if (seObj.second.se.BufferSize != 0){
			seObj.second.se.sBuffer->Release();
		}
	}
	if (lpPrimary) {
		lpPrimary->Release();
		lpPrimary = NULL;
	}
	if (lpDS) {
		lpDS->Release();
		lpDS = NULL;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
// �T�E���h�o�b�t�@�̍쐬
//--------------------------------------------------------------------------------------------
void se::LoadFile(const std::string& resname, const std::string& filename)
{
	HRESULT ret;
	MMCKINFO mSrcWaveFile;
	MMCKINFO mSrcWaveFmt;
	MMCKINFO mSrcWaveData;
	LPWAVEFORMATEX wf;

	//���ɓǂݍ��܂�Ă��郊�\�[�X���Ȃ�A�G���[
	if (SeTable.count(filename) != 0){
		assert(!"DSound:�������O�̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
	}
	for (const auto& se : SeTable){
		if (se.second.filename == filename){
			assert(!"DSound:�����t�@�C�����̃��\�[�X���o�^�ς݂ł��B���O�̊m�F�����Ă��������B");
		}
	}

	// WAV�t�@�C�������[�h
	HMMIO hSrc;
	hSrc = mmioOpen((LPSTR)filename.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
	if (!hSrc) {
		assert(!"WAV�t�@�C�����[�h�G���[\n");
		return;
	}

	// 'WAVE'�`�����N�`�F�b�N
	ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));

	mSrcWaveFile.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	ret = mmioDescend(hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
	if (ret != MMSYSERR_NOERROR) {
		assert(!"WAVE�`�����N�`�F�b�N�G���[");
		mmioClose(hSrc, 0);
		return;
	}

	// 'fmt '�`�����N�`�F�b�N
	ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
	mSrcWaveFmt.ckid = mmioFOURCC('f', 'm', 't', ' ');
	ret = mmioDescend(hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
	if (ret != MMSYSERR_NOERROR) {
		assert(!"fmt �`�����N�`�F�b�N�G���[");
		mmioClose(hSrc, 0);
		return;
	}


	// �w�b�_�T�C�Y�̌v�Z
	int iSrcHeaderSize = mSrcWaveFmt.cksize;
	if (iSrcHeaderSize<sizeof(WAVEFORMATEX))
		iSrcHeaderSize = sizeof(WAVEFORMATEX);

	// �w�b�_�������m��
	wf = (LPWAVEFORMATEX)malloc(iSrcHeaderSize);
	if (!wf) {
		assert(!"�������m�ۃG���[");
		mmioClose(hSrc, 0);
		return;
	}
	ZeroMemory(wf, iSrcHeaderSize);

	// WAVE�t�H�[�}�b�g�̃��[�h
	ret = mmioRead(hSrc, (char*)wf, mSrcWaveFmt.cksize);
	if (FAILED(ret)) {
		assert(!"WAVE�t�H�[�}�b�g���[�h�G���[");
		free(wf);
		mmioClose(hSrc, 0);
		return;
	}

	// fmt�`�����N�ɖ߂�
	mmioAscend(hSrc, &mSrcWaveFmt, 0);

	// data�`�����N��T��
	while (1) {
		// ����
		ret = mmioDescend(hSrc, &mSrcWaveData, &mSrcWaveFile, 0);
		if (FAILED(ret)) {
			assert(!"data�`�����N��������Ȃ�");
			free(wf);
			mmioClose(hSrc, 0);
			return;
		}
		if (mSrcWaveData.ckid == mmioStringToFOURCC("data", 0)){
			break;
		}
		// ���̃`�����N��
		ret = mmioAscend(hSrc, &mSrcWaveData, 0);
	}

	SeObj seObj;
	// �T�E���h�o�b�t�@�̍쐬
	DSBUFFERDESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));
	dsdesc.dwSize = sizeof(DSBUFFERDESC);
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;
	dsdesc.dwBufferBytes = mSrcWaveData.cksize;
	dsdesc.lpwfxFormat = wf;
	dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = lpDS->CreateSoundBuffer(&dsdesc, &seObj.se.sBuffer, NULL);
	seObj.se.BufferSize = (WORD)dsdesc.dwBufferBytes;
	if (FAILED(ret)) {
		assert(!"�T�E���h�o�b�t�@�̍쐬�G���[");
		free(wf);
		mmioClose(hSrc, 0);
		return;
	}

	// ���b�N�J�n
	LPVOID pMem1, pMem2;
	DWORD dwSize1, dwSize2;
	ret = (seObj.se.sBuffer)->Lock(0, mSrcWaveData.cksize, &pMem1, &dwSize1, &pMem2, &dwSize2, 0);
	seObj.se.sBuffer->GetFrequency(&seObj.se.Fre);
	if (FAILED(ret)) {
		assert(!"���b�N���s");
		free(wf);
		mmioClose(hSrc, 0);
		return;
	}

	// �f�[�^��������
	mmioRead(hSrc, (char*)pMem1, dwSize1);
	mmioRead(hSrc, (char*)pMem2, dwSize2);

	// ���b�N����
	(seObj.se.sBuffer)->Unlock(pMem1, dwSize1, pMem2, dwSize2);
	// �w�b�_�p���������J��
	free(wf);
	// WAV�����
	mmioClose(hSrc, 0);

	seObj.se.sBuffer->SetVolume(-50);

	//�����J�n
	for (int t = 0; t < 10; t++){
		if (lpDS->DuplicateSoundBuffer(seObj.se.sBuffer,
			&(seObj.se.PlayBuffer[t].pBuffer)) != DS_OK){
			assert(!"�Đ��p�o�b�t�@�쐬���s");
			break;
		}
		else{
			seObj.se.PlayBuffer[t].State = DSNONE;
			seObj.se.PlayBuffer[t].BufferSize = seObj.se.BufferSize;
			seObj.se.PlayBuffer[t].Loop = 0;
		}
	}
	SeTable.insert(std::unordered_map<std::string, SeObj>::value_type(resname, seObj));
}


//--------------------------------------------------------------------------------------------
// ���g���ύX �Đ����x���ς��
//--------------------------------------------------------------------------------------------
void se::SetFrequency(const std::string& resname, int Fre)
{
	auto se = SeTable.at(resname).se;
	se.sBuffer->SetFrequency(se.Fre + Fre);
}

void se::SetVolume(const std::string& resname, int Vol)
{
	long volume = DSBVOLUME_MIN;
	if (Vol < 0) {
		Vol = 0;
	}
	if (Vol >= 100) {
		Vol = 100;
	}
	volume = (long)(1000.0 * log10((Vol) / 100.0));
	if (volume < DSBVOLUME_MIN) {
		volume = DSBVOLUME_MIN;
	}
	else {
		if (volume > DSBVOLUME_MAX) {
			volume = DSBVOLUME_MAX;
		}
	}
	auto se = SeTable.at(resname).se;
	for (auto& s : se.PlayBuffer)
	{
		s.pBuffer->SetVolume(volume);
	}
}
//********************************************************************//
//
//				SE�Đ��@�֘A�֐� �����܂�
//
//********************************************************************//


