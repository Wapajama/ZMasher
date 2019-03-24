#include "ModelViewerState.h"
#include <ZMUtils/Debugging/ZMDebugger.h>
#include <iostream>
#include <Modelviewer/GeneratedFiles/ui_modelviewerwindow.h>
#include <ZMasher\InputManager.h>
#include <project_defines.h>
#ifdef ZMASHER_DX11
#include <ZMasherGfxDX11\ZMModelFactory.h>
#include <ZMasherGfxDX11/Camera.h>
#include <ZMasherGfxDX11\ZMModelInstanceNode.h>
#elif defined(ZMASHER_DX12)
#include <ZMasherGfxDX12\ZMModelFactory.h>
#include <ZMasherGfxDX12/Camera.h>
#include <ZMasherGfxDX12\ZMModel.h> 
#endif // ZMASHER_DX11
#include <ZMUtils/File/PathManager.h>
#include <qfiledialog.h>


ModelViewerState::ModelViewerState(Camera* camera): ModelViewer(nullptr), m_Model(nullptr), m_Camera(camera)
{
}

ModelViewerState::~ModelViewerState()
{
}

std::string ModelViewerState::FileBrowserLoadPath(const std::string& dir)
{
	return QFileDialog::getOpenFileName(this, "ZMasher File Selector (cutting edge lol)", QString(dir.c_str()), "*.model").toStdString();
}

bool ModelViewerState::Init(const char* args)
{
	this->show();
	return true;
}

bool ModelViewerState::Update(const float dt)
{
	ZMasher::Vector3f cam_pos = m_Camera->GetPosition();
	UpdateCameraPos(cam_pos.x, cam_pos.y, cam_pos.z);
	return !m_ExitCalled;
}

void ModelViewerState::OnLoadFile()
{
	if (m_Model)
	{
		m_Model->MarkForDelete();
	}
	std::string model_path = this->FileBrowserLoadPath(PathManager::Instance()->GetDataPath());
	if (model_path.length() > 1)
	{
		m_Model = ZMModelFactory::Instance()->LoadModelInstance(model_path.c_str());
	}
}

void ModelViewerState::UpdateCameraPos(float x, float y, float z) 
{
	ui->cameraPosX->setText("X: " + QString::number(x));
	ui->cameraPosY->setText("Y: " + QString::number(y));
	ui->cameraPosZ->setText("Z: " + QString::number(z));
}

bool ModelViewerState::event(QEvent* e)
{
	switch (e->type())
	{
	case QEvent::WindowActivate : 
	{
		InputManager::Instance()->ClientInFocus(true);
	}
	break;
	case QEvent::WindowDeactivate : 
	{
		InputManager::Instance()->ClientInFocus(false);
	}
	break;
	case QEvent::Leave: 
	{
		InputManager::Instance()->CursorInsideClientWindow(false);
	}
	break;
	case QEvent::Enter:
	{
		InputManager::Instance()->CursorInsideClientWindow(true);
	}
	break;
	case QEvent::MouseMove:
	{
		if (ui->graphicsView->underMouse())
		{
			InputManager::Instance()->CursorInsideClientWindow(true);
		}
		else
		{
			InputManager::Instance()->CursorInsideClientWindow(false);
		}
	}
	break;
	default:
		break;
	}

	return QMainWindow::event(e);
}