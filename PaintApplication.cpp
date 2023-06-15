#include "PaintApplication.h"
#include <qmessagebox.h>
#include <QPushButton>
#include<QMouseEvent>
#include <QHBoxLayout>
#include <QTreeView>
#include <QSplitter>
#include <QStandardItemModel>
#include <qlabel.h>

PaintApplication::PaintApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    openglWindow = new OpenGLWindow(this);

    // Create the QTreeView widget
    treeView = new QTreeView(this); 

    // Create the model for the QTreeView
    QStandardItemModel* model = new QStandardItemModel(this);
    treeView->setModel(model);

    // Set the minimum width of the QTreeView
    treeView->setMinimumSize(200,400); 

    treeView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    // Create a QSplitter to split the openglwindow and treeview
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(treeView); 
    splitter->addWidget(openglWindow); 

    // Set the initial sizes of the splitter's widgets
    splitter->setSizes(QList<int>() << 50 << 950); // Adjust the sizes as needed

    QHBoxLayout* layout = new QHBoxLayout; 

    QLabel* label = new QLabel("Select Geometry");

    label->setMaximumWidth(100);

    // Set a maximum width for the buttons
    ui.Line_Button->setMaximumWidth(40);
    ui.Circle_Button->setMaximumWidth(40);
    ui.Rectangle_Button->setMaximumWidth(40);

    layout->addWidget(label);
    layout->addWidget(ui.Line_Button);
    layout->addWidget(ui.Circle_Button);
    layout->addWidget(ui.Rectangle_Button);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(layout);
    vLayout->addWidget(splitter);
   
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(vLayout);
    setCentralWidget(centralWidget);

    connect(ui.Line_Button, &QPushButton::clicked, this, [this]() {
        OpenGLWindow::drawLineMode = !OpenGLWindow::drawLineMode; 
        if (OpenGLWindow::drawLineMode)
            ui.Line_Button->setStyleSheet("background-color: yellow;");
        else
            ui.Line_Button->setStyleSheet("");
        });

    connect(ui.Circle_Button, &QPushButton::clicked, this, [this]() {
        OpenGLWindow::drawCircleMode = !OpenGLWindow::drawCircleMode; 
        if (OpenGLWindow::drawCircleMode)
            ui.Circle_Button->setStyleSheet("background-color: yellow;");
        else
            ui.Circle_Button->setStyleSheet("");
        });


    connect(ui.Rectangle_Button, &QPushButton::clicked, this, [this]() {
        OpenGLWindow::drawRectangleMode = !OpenGLWindow::drawRectangleMode;
        if (OpenGLWindow::drawRectangleMode)
            ui.Rectangle_Button->setStyleSheet("background-color: yellow;");
        else
            ui.Rectangle_Button->setStyleSheet("");
        });

    connect(openglWindow, &OpenGLWindow::lineCreated, this, [model](QString nameList) {
        QStandardItem* item = new QStandardItem(nameList);
        model->appendRow(item);
        });

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        QModelIndexList selectedIndexes = treeView->selectionModel()->selectedIndexes();
        if (!selectedIndexes.isEmpty()) {
            QModelIndex selectedIndex = selectedIndexes.first();
            int lineId = selectedIndex.row();
            OpenGLWindow::highlightMode = true;
            openglWindow->highLight.push_back(openglWindow->geometryBase[lineId]);
            openglWindow->update();
        }
        });
    
}

PaintApplication::~PaintApplication()
{}
