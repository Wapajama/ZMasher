#include "BaseShader.h"
#include <D3DX11.h>
#include <d3dx11effect.h>
#include "ZMVertexTypes.h"
#include <fstream>
#include <Windows.h>

static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, wchar_t* shaderFilename)
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

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

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
							   const DirectX::XMMATRIX& world,
							   const DirectX::XMMATRIX& view,
							   const DirectX::XMMATRIX& projection )
{
	HRESULT infoResult = S_OK;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	infoResult = context->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	RETURNF_IF_FAILED(infoResult);

	MatrixBufferType* dataPtr = nullptr;
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	DirectX::XMMATRIX worldLocal = DirectX::XMMatrixTranspose(world);
	DirectX::XMMATRIX viewLocal = DirectX::XMMatrixTranspose(view);
	DirectX::XMMATRIX projectionLocal = DirectX::XMMatrixTranspose(projection);

	dataPtr->world = worldLocal;
	dataPtr->view = viewLocal;
	dataPtr->projection = projectionLocal;

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

bool BaseShader::Create(wchar_t* source_file, ID3D11Device* device)
{
	HRESULT infoResult = S_OK;

	ID3D10Blob* errorMessage = nullptr;

	unsigned int dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) | defined(_DEBUG)
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	D3D11_BUFFER_DESC matrixBufferDesc;
	ID3DBlob* shaderBuffer = nullptr;

	//fix some sort of PathManager to replace hardcoded L"../ZMasher/"
	const std::wstring effect = L"../ZMasher/" + std::wstring(source_file);

	infoResult = (D3DX11CompileFromFile(effect.c_str(), 0, 0, 0,
										"fx_5_0",
										dwShaderFlags,
										0, 0,
										&shaderBuffer,
										&errorMessage,
										0));
	if (errorMessage)
	{
		OutputShaderErrorMessage(errorMessage, 0, L"LOL");
	}
	RETURNF_IF_FAILED(infoResult);

	infoResult = D3DX11CreateEffectFromMemory(shaderBuffer->GetBufferPointer(),
											  shaderBuffer->GetBufferSize(),
											  NULL,
											  device,
											  &m_Effect);

	RETURNF_IF_FAILED(infoResult);

	shaderBuffer->Release();
	m_Technique = m_Effect->GetTechniqueByIndex(0);//dangerous, unorthodox, change to a more sustainable solution
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