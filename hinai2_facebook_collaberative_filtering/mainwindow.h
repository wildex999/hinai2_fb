#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collaberativefiltering.h"
#include "person.h"

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
    void on_setTolkenButton_clicked();

private:
    Ui::MainWindow *ui;
    CollaberativeFiltering cf;
    void            convertPersonToGroup(Person person, QList< GROUP>& groups);
    void            addPersonWithProducts(Person person);
};

#endif // MAINWINDOW_H
