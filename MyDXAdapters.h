#pragma once
#include <dxgi1_6.h>
#include <vector>
#pragma comment(lib, "dxgi.lib")

class MyDXAdapters
{
private:
	IDXGIFactory7* dxgiFactory = nullptr;
	// �A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmpAdapter = nullptr;
public:
	void Init();
	IDXGIAdapter4* Template();
	IDXGIFactory7* Factory();
private:
	void List();
	void Select();
};

