#pragma once
#include "GameState.h"
#include <ZMasherGfxDX11/ZMAPIStructs.h>
#include "modelviewer_global.h"
#include "modelviewer.h"


class ModelViewerWindow;
class ModelViewerState :
	public GameState, public ModelViewer
{
public:
	ModelViewerState();
	~ModelViewerState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	bool Destroy()override { return true; }

	inline void SetWinVals(ZMWinApiCointainer* winVals){m_WinVals = winVals;}
	
protected:
	bool event(QEvent* e)override;
	void OnLoadFile(const std::string& path)override;

private:

	ZMWinApiCointainer* m_WinVals;
	ModelViewerWindow* m_ModelViewerWindow;
	
	bool m_ExitCalled;
};

