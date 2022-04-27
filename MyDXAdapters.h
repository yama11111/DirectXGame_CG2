#pragma once
//#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
//#include <string>

//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")

class MyDXAdapters
{
private:
	// �A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmpAdapter = nullptr;
public:
	MyDXAdapters();
	~MyDXAdapters();
private:
	void List(IDXGIFactory7*& dxgiFactory);
	void Select();
};

