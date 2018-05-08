#include "BaseShader.h"

#ifdef ZMASHER_DX12

#else
#include <D3D11.h>  
#endif // ZMASHER_DX12

#include "ZMVertexTypes.h"
#include <fstream>
#include <Windows.h>
#include "D3Dcompiler.h"
#include <FX11\inc\d3dx11effect.h>

static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const wchar_t* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;
	std::wstring wmsg = std::wstring(shaderFilename);
	std::string msg = std::string(wmsg.begin(), wmsg.end());
	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", msg.c_str(), MB_OK);

	return;
}

BaseShader::BaseShader()
{
	m_Effect = nullptr;
	m_Layout = nullptr;
	m_MatrixBuffer = nullptr;
}

BaseShader::~BaseShader()
{
}


bool BaseShader::Apply(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_Layout);
	return true;
}

bool BaseShader::SetShaderVars(ID3D11DeviceContext* context,
							   const MatrixBufferType& constant_buffer)
{
	HRESULT infoResult = S_OK;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	infoResult = context->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	RETURNF_IF_FAILED(infoResult);

	MatrixBufferType* dataPtr = nullptr;
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	DirectX::XMMATRIX worldLocal = DirectX::XMMatrixTranspose(constant_buffer.world);
	DirectX::XMMATRIX viewLocal = DirectX::XMMatrixTranspose(constant_buffer.view);
	DirectX::XMMATRIX projectionLocal = DirectX::XMMatrixTranspose(constant_buffer.projection);

	dataPtr->world = worldLocal;
	dataPtr->view = viewLocal;
	dataPtr->projection = projectionLocal;
	dataPtr->cam_pos = constant_buffer.cam_pos;
	dataPtr->d_time = constant_buffer.d_time;
	context->Unmap(m_MatrixBuffer, 0);

	unsigned int bufferNumber = 0;

	if (this->GetDX11Effect()->GetConstantBufferByName("MatrixBuffer"))
	{
		//actually only needs to be set if there has been any other shader used recently, fix this when implementing instancing
		this->GetDX11Effect()->GetConstantBufferByName("MatrixBuffer")->SetConstantBuffer(m_MatrixBuffer);
	}
	else
	{
		return false;//couldn't find Matrixbuffer!
	}
	m_Technique->GetPassByIndex(0)->Apply(0, context);//fix more advanced system for this
	return true;
}

bool BaseShader::Create(const wchar_t* source_file, ID3D11Device* device)
{
	HRESULT infoResult = S_OK;

	ID3DBlob* errorMessage = nullptr;

	unsigned int dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) | defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	D3D11_BUFFER_DESC matrixBufferDesc;
	ID3DBlob* shaderBuffer = nullptr;

	//fix some sort of PathManager to replace hardcoded L"../ZMasher/"
	const std::wstring effect = L"../Shaders/" + std::wstring(source_file);

	infoResult = D3DX11CompileEffectFromFile(effect.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, 0, 0, device, &m_Effect, &errorMessage);

	if (errorMessage)
	{
		OutputShaderErrorMessage(errorMessage, 0, L"Shader failed to compile");
	}

	RETURNF_IF_FAILED(infoResult);
	m_Technique = m_Effect->GetTechniqueByIndex(0);//TODO: dangerous, unorthodox, change to a more sustainable solution
	D3DX11_PASS_DESC passDesc;
	infoResult = m_Technique->GetPassByIndex(0)->GetDesc(&passDesc);
	auto numElements = sizeof(g_PosNormUv) / sizeof(g_PosNormUv[0]);
	infoResult = device->CreateInputLayout(g_PosNormUv, numElements,
										   passDesc.pIAInputSignature,
										   passDesc.IAInputSignatureSize, &m_Layout);
	RETURNF_IF_FAILED(infoResult);

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	infoResult = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	m_Effect->GetConstantBufferByName("MatrixBuffer")->SetConstantBuffer(m_MatrixBuffer);
	if (FAILED(infoResult))
	{
		return false;
	}

	return true;
}