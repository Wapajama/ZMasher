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

    void SetPushButtonClicked(void(*)(bool));

private:
    void(*m_ButtonClickedLamda)(bool args);
private slots:
    void on_pushButton_clicked(bool checked);


private:
};

#endif // MODELVIEWER_H
