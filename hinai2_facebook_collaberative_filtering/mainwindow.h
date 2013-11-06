#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collaberativefiltering.h"
#include "person.h"
#include "product.h"

class FBGraph_Parser;

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

    void on_UpdatePeople_clicked();

private:
    Ui::MainWindow *ui;
    CollaberativeFiltering cf;
     FBGraph_Parser* parser;
    void            convertPersonToGroup(Person person, QList< GROUP>& groups);
    void            convertProductStringToProduct(Product product, PRODUCT& productenum );
    void            addPersonWithProducts(Person person, bool train);
};

#endif // MAINWINDOW_H
