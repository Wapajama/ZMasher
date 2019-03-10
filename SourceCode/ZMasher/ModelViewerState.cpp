#include "ModelViewerState.h"
#include <ZMUtils/Debugging/ZMDebugger.h>
#include <iostream>
#include "ui_modelviewerwindow.h" 
#include <ZMasher\InputManager.h>


ModelViewerState::ModelViewerState(): ModelViewer(nullptr)
{
}

ModelViewerState::~ModelViewerState()
{
}

bool ModelViewerState::Init(const char* args)
{
	this->show();
	return true;
}

bool ModelViewerState::Update(const float dt)
{

	return !m_ExitCalled;
}

void ModelViewerState::OnLoadFile(const std::string& path)
{

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
	default:
		break;
	}

	return QMainWindow::event(e);
}