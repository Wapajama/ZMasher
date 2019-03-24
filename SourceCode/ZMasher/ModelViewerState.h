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
	ModelViewerState(class Camera* camera);
	~ModelViewerState();

	bool Init(const char* args)override;
	bool Update(const float dt)override;
	bool Destroy()override { return true; }

	inline void SetWinVals(ZMWinApiCointainer* winVals){m_WinVals = winVals;}
	
protected:
	bool event(QEvent* e)override;
	void OnLoadFile()override;
	std::string FileBrowserLoadPath(const std::string& dir);

private:

	void UpdateCameraPos(float x, float y, float z) override;
	ZMWinApiCointainer* m_WinVals;
	ModelViewerWindow* m_ModelViewerWindow;
	class ZMModelInstanceNode* m_Model;
	Camera* m_Camera;
	bool m_ExitCalled;
};

