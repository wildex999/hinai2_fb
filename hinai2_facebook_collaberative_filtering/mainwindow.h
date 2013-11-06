#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collaberativefiltering.h"
#include "person.h"

class FBGraph_Parser;
class NetworkManager;
class QWidget;
class QStandardItemModel;
namespace Ui {
        class MainWindow;
    }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_predictButton_clicked();

    void on_setTokenButton_clicked();

    void on_updateButton_clicked();

private:
    Ui::MainWindow *ui;
    CollaberativeFiltering cf;
    FBGraph_Parser* parser;
    NetworkManager* net;
    QWidget* newTab;
    QStandardItemModel* model;
    void            convertPersonToGroup(Person person, QList< GROUP>& groups);
    void            addPersonWithProducts(Person person);
    void            makeTable(CollaberativeFiltering cf, QObject *parent=0);
};

#endif // MAINWINDOW_H
