#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "modelviewer_global.h"
#include <QMainWindow>
#include <string>
namespace Ui {
	class ModelViewerWindow;
}

class MODELVIEWERSHARED_EXPORT ModelViewer : public QMainWindow
{
	Q_OBJECT
public:
	explicit ModelViewer(QWidget* parent = nullptr);
	~ModelViewer();

	inline Ui::ModelViewerWindow *GetUI() { return ui; }

protected:
	Ui::ModelViewerWindow *ui;
protected slots:
	virtual void OnLoadFile() = 0;
private slots:
    void on_actionLoad_Model_triggered();

private:
	virtual void UpdateCameraPos(float x, float y, float z) = 0;
};

#endif // MODELVIEWER_H
