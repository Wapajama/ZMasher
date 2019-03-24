#include "modelviewer.h"
#include "ui_modelviewerwindow.h"

ModelViewer::ModelViewer(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::ModelViewerWindow)
{
    ui->setupUi(this);


}

ModelViewer::~ModelViewer()
{

}
void ModelViewer::on_actionLoad_Model_triggered()
{
	OnLoadFile();
}
