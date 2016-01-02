#include "TextureShaderClass.h"
#include <D3DX11async.h>
#include "ZMasherUtilities.h"
#include <d3dx11async.h>
#include "ZMVertexTypes.h"

#define RETURNF_IF_FAILED(result) if (FAILED(result))\
	{\
		return false;\
	}\

TextureShaderClass::TextureShaderClass()
{
	m_VertexShader = nullptr;
	m_PixelShader = nullptr;
	m_Layout = nullptr;
	m_MatrixBuffer = nullptr;

}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& copy)
{

}

TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Init(ID3D11Device* device, HWND windowHandle)
{
	bool result = false;

	result = this->InitShader(device, windowHandle, L"../ZMasher/texture.vs", L"../ZMasher/texture.ps");
	if (result == false)
	{
		return false;
	}
	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
}


bool TextureShaderClass::SetShaderVars(	ID3D11DeviceContext* context, 
								const DirectX::XMMATRIX& world,
								const DirectX::XMMATRIX& view,
								const DirectX::XMMATRIX& projection,
								ID3D11ShaderResourceView* texture)
{
	bool success = false;

	success = SetShaderParameters(context, world, view, projection, texture);
	if (success == false)
	{
		return false;
	}

	SetVariables(context);

	return true;
}

bool TextureShaderClass::InitShader(ID3D11Device* device, HWND windowHandle, 
								  FileType vsFileName, FileType psFileName)
{
	HRESULT infoResult = S_OK;

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	

	std::wstring lengthTest = vsFileName;
	unsigned int dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) | defined(_DEBUG)
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	infoResult = D3DX11CompileFromFile(vsFileName, 0, 0, "TextureVertexShader",
									   "vs_5_0",
									   dwShaderFlags,
									   0,
									   0,
									   &vertexShaderBuffer, &errorMessage,
									   0);
	if (FAILED(infoResult))
	{
		
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, windowHandle, vsFileName);
		}
		else
		{
			MessageBox(windowHandle, vsFileName, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	ID3DBlob* pixelShaderBuffer = nullptr;

	lengthTest = psFileName;


	infoResult = D3DX11CompileFromFile(psFileName, 0, 0, "TexturePixelShader",
									   "ps_5_0",
									   dwShaderFlags,
									   0,
									   0,
									   &pixelShaderBuffer, &errorMessage,
									   0);
	if (FAILED(infoResult))
	{

		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, windowHandle, psFileName);
		}
		else
		{
			MessageBox(windowHandle, psFileName, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	infoResult = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
											vertexShaderBuffer->GetBufferSize(),
											NULL,
											&m_VertexShader);
	RETURNF_IF_FAILED(infoResult);
	infoResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), 
										   pixelShaderBuffer->GetBufferSize(), 
										   NULL, 
										   &m_PixelShader);
	RETURNF_IF_FAILED(infoResult);


	auto numElements = sizeof(g_PosUv) / sizeof(g_PosUv[0]);


	infoResult = device->CreateInputLayout(g_PosUv, numElements, vertexShaderBuffer->GetBufferPointer(),
										   vertexShaderBuffer->GetBufferSize(), &m_Layout);

	if (FAILED(infoResult))
	{
		return false;
	}


	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	infoResult = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);

	if (FAILED(infoResult))
	{
		return false;
	}

	return true;
}

void TextureShaderClass::ShutdownShader()
{
	if (m_MatrixBuffer != nullptr)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	if (m_Layout != nullptr)
	{
		m_Layout->Release();
		m_Layout = 0;
	}
	
	if (m_PixelShader != nullptr)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	if (m_VertexShader != nullptr)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}

	return;
}

//copy+pasted
void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, FileType shaderFilename)
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

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* context,
										   const DirectX::XMMATRIX& world,
										   const DirectX::XMMATRIX& view,
										   const DirectX::XMMATRIX& projection,
										   ID3D11ShaderResourceView* texture)
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

	context->VSSetConstantBuffers(bufferNumber, 1, &m_MatrixBuffer);

	context->PSSetShaderResources(0,1, &texture);

	return true;
}

void TextureShaderClass::SetVariables(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_Layout);
	context->VSSetShader(m_VertexShader, NULL, 0);
	context->PSSetShader(m_PixelShader, NULL, 0);
}