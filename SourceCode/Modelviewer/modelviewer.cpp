#include "modelviewer.h"
#include "ui_modelviewerwindow.h"

ModelViewer::ModelViewer(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ModelViewerWindow)
{
    ui->setupUi(this);
    m_ButtonClickedLamda = nullptr;
}

ModelViewer::~ModelViewer()
{

}

void ModelViewer::SetPushButtonClicked(void (*zmasher_function)(bool))
{
    m_ButtonClickedLamda = zmasher_function;
}

void ModelViewer::on_pushButton_clicked(bool checked)
{
    if (m_ButtonClickedLamda != nullptr)
    {
        m_ButtonClickedLamda(checked);
    }
}
