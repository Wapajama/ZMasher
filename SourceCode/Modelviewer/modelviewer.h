#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include "modelviewer_global.h"
#include <QMainWindow>

namespace Ui {
class ModelViewerWindow;
}

class MODELVIEWERSHARED_EXPORT ModelViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit ModelViewer(QWidget* parent = nullptr);
    ~ModelViewer();

    Ui::ModelViewerWindow *ui;
private:
};

#endif // MODELVIEWER_H
